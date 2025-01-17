/*
 * \brief  Generic implementation parts of the signaling framework
 * \author Norman Feske
 * \date   2008-09-16
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/signal.h>
#include <base/thread.h>
#include <base/trace/events.h>
#include <signal_session/connection.h>

using namespace Genode;


/**
 * Return process-wide signal session used for signal allocation and submission
 */
static Signal_connection *signal_connection()
{
	static Signal_connection sc;
	return &sc;
}


/******************************************************
 ** Process-wide connection to core's signal service **
 ******************************************************/

enum { STACK_SIZE = 4*1024*sizeof(addr_t) };

class Signal_handler_thread : Thread<STACK_SIZE>, Lock
{
	private:

		/**
		 * Return process-wide signal source used for signal reception
		 *
		 * This function must be called from the context of the signal handler
		 * thread because on some platforms (e.g., Fiasco.OC), the calling
		 * thread context is used for implementing the signal-source protocol.
		 */
		static Signal_source *signal_source();

		void entry()
		{
			Signal_source *source = signal_source();
			unlock();
			Signal_receiver::dispatch_signals(source);
		}

	public:

		/**
		 * Constructor
		 */
		Signal_handler_thread()
		: Thread<STACK_SIZE>("signal handler"), Lock(Lock::LOCKED)
		{
			start();

			/*
			 * Make sure to have initialized the 'signal_source()' channel
			 * before proceeding with the use of signals. Otherwise, signals
			 * that occurred until the construction of 'signal_source' is
			 * completed may get lost.
			 */
			lock();
		}
};


Signal_source *Signal_handler_thread::signal_source()
{
	static Signal_source_client sigsrc(signal_connection()->signal_source());
	return &sigsrc;
}


/**
 * Return process-wide signal source used for signal reception
 */
static Signal_handler_thread *signal_handler_thread()
{
	static Signal_handler_thread signal_handler_thread;
	return &signal_handler_thread;
}


/*****************************
 ** Signal context registry **
 *****************************/

namespace Genode {

	/**
	 * Facility to validate the liveliness of signal contexts
	 *
	 * After dissolving a 'Signal_context' from a 'Signal_receiver', a signal
	 * belonging to the context may still in flight, i.e., currently processed
	 * within core or the kernel. Hence, after having received a signal, we
	 * need to manually check for the liveliness of the associated context.
	 * Because we cannot trust the signal imprint to represent a valid pointer,
	 * we need an associative data structure to validate the value. That is the
	 * role of the 'Signal_context_registry'.
	 */
	class Signal_context_registry
	{
		private:

			/*
			 * Currently, the registry is just a linked list. If this becomes a
			 * scalability problem, we might introduce a more sophisticated
			 * associative data structure.
			 */
			Lock mutable                        _lock;
			List<List_element<Signal_context> > _list;

		public:

			void insert(List_element<Signal_context> *le)
			{
				Lock::Guard guard(_lock);
				_list.insert(le);
			}

			void remove(List_element<Signal_context> *le)
			{
				Lock::Guard guard(_lock);
				_list.remove(le);
			}

			bool test_and_lock(Signal_context *context) const
			{
				Lock::Guard guard(_lock);

				/* search list for context */
				List_element<Signal_context> const *le = _list.first();
				for ( ; le; le = le->next()) {

					if (context == le->object()) {
						/* lock object */
						context->_lock.lock();
						return true;
					}
				}
				return false;
			}
	};
}


/**
 * Return process-wide registry of registered signal contexts
 */
Genode::Signal_context_registry *signal_context_registry()
{
	static Signal_context_registry inst;
	return &inst;
}


/************
 ** Signal **
 ************/

void Signal::_dec_ref_and_unlock()
{
	if (_data.context) {
		Lock::Guard lock_guard(_data.context->_lock);
		_data.context->_ref_cnt--;
		if (_data.context->_ref_cnt == 0)
			_data.context->_destroy_lock.unlock();
	}
}


void Signal::_inc_ref()
{
	if (_data.context) {
		Lock::Guard lock_guard(_data.context->_lock);
		_data.context->_ref_cnt++;
	}
}


Signal::Signal(Signal::Data data) : _data(data)
{
	if (_data.context) {
		_data.context->_ref_cnt = 1;
		_data.context->_destroy_lock.lock();
	}
}


/********************
 ** Signal context **
 ********************/

void Signal_context::submit(unsigned num)
{
	if (!_receiver) {
		PWRN("signal context with no receiver");
		return;
	}

	if (!signal_context_registry()->test_and_lock(this)) {
		PWRN("encountered dead signal context");
		return;
	}

	/* construct and locally submit signal object */
	Signal::Data signal(this, num);
	_receiver->local_submit(signal);

	/* free context lock that was taken by 'test_and_lock' */
	_lock.unlock();
}


/************************
 ** Signal transmitter **
 ************************/

Signal_connection * Signal_transmitter::connection() { return signal_connection(); }


/*********************
 ** Signal receiver **
 *********************/

void Signal_receiver::_unsynchronized_dissolve(Signal_context *context)
{
	/* tell core to stop sending signals referring to the context */
	signal_connection()->free_context(context->_cap);

	/* restore default initialization of signal context */
	context->_receiver = 0;
	context->_cap      = Signal_context_capability();

	/* remove context from context list */
	_contexts.remove(&context->_receiver_le);

	/* unregister context from process-wide registry */
	signal_context_registry()->remove(&context->_registry_le);
}


Signal_receiver::Signal_receiver()
{
	/* make sure that the process-local signal handler thread is running */
	signal_handler_thread();
}


Signal_context_capability Signal_receiver::manage(Signal_context *context)
{
	if (context->_receiver)
		throw Context_already_in_use();

	context->_receiver = this;

	Lock::Guard list_lock_guard(_contexts_lock);

	/* insert context into context list */
	_contexts.insert(&context->_receiver_le);

	/* register context at process-wide registry */
	signal_context_registry()->insert(&context->_registry_le);

	bool try_again = true;
	for (;;) {
		try {

			/* use signal context as imprint */
			context->_cap = signal_connection()->alloc_context((long)context);
			return context->_cap;

		} catch (Signal_session::Out_of_metadata) {

			/* give up if the error occurred a second time */
			if (try_again) { try_again = false; }
			else { break; }

			size_t const quota = 1024*sizeof(long);
			char buf[64];
			snprintf(buf, sizeof(buf), "ram_quota=%zu", quota);

			PINF("upgrading quota donation for SIGNAL session (%zu bytes)", quota);

			env()->parent()->upgrade(signal_connection()->cap(), buf);
		}
	};
	return Signal_context_capability();
}


void Signal_receiver::dissolve(Signal_context *context)
{
	if (context->_receiver != this)
		throw Context_not_associated();

	Lock::Guard list_lock_guard(_contexts_lock);

	_unsynchronized_dissolve(context);

	Lock::Guard context_destroy_lock_guard(context->_destroy_lock);
}


bool Signal_receiver::pending()
{
	Lock::Guard list_lock_guard(_contexts_lock);

	/* look up the contexts for the pending signal */
	for (List_element<Signal_context> *le = _contexts.first(); le; le = le->next()) {

		Signal_context *context = le->object();

		Lock::Guard lock_guard(context->_lock);

		if (context->_pending)
			return true;
	}
	return false;
}


Signal Signal_receiver::pending_signal()
{
	Lock::Guard list_lock_guard(_contexts_lock);

	/* look up the contexts for the pending signal */
	for (List_element<Signal_context> *le = _contexts.first(); le; le = le->next()) {

		Signal_context *context = le->object();

		Lock::Guard lock_guard(context->_lock);

		/* check if context has a pending signal */
		if (!context->_pending)
			continue;

		context->_pending = false;
		Signal::Data result = context->_curr_signal;

		/* invalidate current signal in context */
		context->_curr_signal = Signal::Data(0, 0);

		if (result.num == 0)
			PWRN("returning signal with num == 0");

		Trace::Signal_received trace_event(*context, result.num);

		/* return last received signal */
		return result;
	}

	/*
	 * Normally, we should never arrive at this point because that would
	 * mean, the '_signal_available' semaphore was increased without
	 * registering the signal in any context associated to the receiver.
	 *
	 * However, if a context gets dissolved right after submitting a
	 * signal, we may have increased the semaphore already. In this case
	 * the signal-causing context is absent from the list.
	 */
	throw Signal_not_pending();
}


void Signal_receiver::block_for_signal()
{
	_signal_available.down();
}


Signal Signal_receiver::wait_for_signal()
{
	for (;;) {

		/* block until the receiver has received a signal */
		block_for_signal();

		try {
			return pending_signal();
		} catch (Signal_not_pending) { }
	}
}


void Signal_receiver::local_submit(Signal::Data ns)
{
	Signal_context *context = ns.context;

	/*
	 * Replace current signal of the context by signal with accumulated
	 * counters. In the common case, the current signal is an invalid
	 * signal with a counter value of zero.
	 */
	unsigned num = context->_curr_signal.num + ns.num;
	context->_curr_signal = Signal::Data(context, num);

	/* wake up the receiver if the context becomes pending */
	if (!context->_pending) {
		context->_pending = true;
		_signal_available.up();
	}
}


void Signal_receiver::dispatch_signals(Signal_source *signal_source)
{
	for (;;) {
		Signal_source::Signal source_signal = signal_source->wait_for_signal();

		/* look up context as pointed to by the signal imprint */
		Signal_context *context = (Signal_context *)(source_signal.imprint());

		if (!signal_context_registry()->test_and_lock(context)) {
			PWRN("encountered dead signal context");
			continue;
		}

		/* construct and locally submit signal object */
		Signal::Data signal(context, source_signal.num());
		context->_receiver->local_submit(signal);

		/* free context lock that was taken by 'test_and_lock' */
		context->_lock.unlock();
	}
}


void Signal_receiver::_platform_destructor() { }
