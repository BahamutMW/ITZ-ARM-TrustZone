/*
 * \brief  Server-side audio-session interface
 * \author Sebastian Sumpf
 * \date   2012-12-10
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__AUDIO_OUT_SESSION__RPC_OBJECT_H_
#define _INCLUDE__AUDIO_OUT_SESSION__RPC_OBJECT_H_

#include <base/env.h>
#include <base/rpc_server.h>
#include <audio_out_session/audio_out_session.h>


namespace Audio_out { class Session_rpc_object; }


class Audio_out::Session_rpc_object : public Genode::Rpc_object<Audio_out::Session,
                                                                Session_rpc_object>
{
	protected:

		bool                             _stopped;   /* state */
		Genode::Ram_dataspace_capability _ds;        /* contains Audio_out_stream */
		Genode::Signal_transmitter       _progress;
		Genode::Signal_transmitter       _alloc;

		Genode::Signal_context_capability _data_cap;

		bool _progress_sigh; /* progress signal on/off */
		bool _alloc_sigh;    /* alloc signal on/off */

	public:

		Session_rpc_object(Genode::Signal_context_capability data_cap)
		:
			_stopped(true), _data_cap(data_cap),
			_progress_sigh(false), _alloc_sigh(false)
		{
			_ds = Genode::env()->ram_session()->alloc(sizeof(Stream));
			_stream = static_cast<Stream *>(Genode::env()->rm_session()->attach(_ds));
		}

		virtual ~Session_rpc_object()
		{
			if (_ds.valid()) {
				Genode::env()->rm_session()->detach(_stream);
				Genode::env()->ram_session()->free(_ds);
			}
		}


		/**************
		 ** Signals  **
		 **************/

		void progress_sigh(Genode::Signal_context_capability sigh)
		{
			_progress.context(sigh);
			_progress_sigh = true;
		}

		Genode::Signal_context_capability data_avail_sigh() {
			return _data_cap; }

		void alloc_sigh(Genode::Signal_context_capability sigh)
		{
			_alloc.context(sigh);
			_alloc_sigh = true;
		}


		/***********************
		 ** Session interface **
		 ***********************/

		void start() { _stopped = false; }
		void stop()  { _stopped = true; }

		Genode::Dataspace_capability dataspace() { return _ds; }


		/**********************************
		 ** Session interface extensions **
		 **********************************/

		/**
		 * Send 'progress' signal
		 */
		void progress_submit()
		{
			if (_progress_sigh)
				_progress.submit();
		}

		/**
		 * Send 'alloc' signal
		 */
		void alloc_submit()
		{
			if (_alloc_sigh)
				_alloc.submit();
		}

		/**
		 * Return true if client state is stopped
		 */
		bool stopped() const { return _stopped; }

		/**
		 * Return true if client session is active
		 */
		bool active()  const { return !_stopped; }
};

#endif /* _INCLUDE__AUDIO_OUT_SESSION__RPC_OBJECT_H_ */
