/*
 * \brief  Client-side region manager session interface
 * \author Christian Helmuth
 * \author Alexander Boettcher
 * \date   2006-07-11
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__RM_SESSION__CLIENT_H_
#define _INCLUDE__RM_SESSION__CLIENT_H_

#include <rm_session/capability.h>
#include <base/rpc_client.h>

namespace Genode {

	struct Rm_session_client : Rpc_client<Rm_session>
	{
		Dataspace_capability _rm_ds_cap;

		explicit Rm_session_client(Rm_session_capability session)
		: Rpc_client<Rm_session>(session) { }

		Local_addr attach(Dataspace_capability ds, size_t size = 0,
		                  off_t offset = 0, bool use_local_addr = false,
		                  Local_addr local_addr = (void *)0,
		                  bool executable = false)
		{
			return call<Rpc_attach>(ds, size, offset,
			                        use_local_addr, local_addr,
			                        executable);
		}

		void detach(Local_addr local_addr) {
			call<Rpc_detach>(local_addr); }

		Pager_capability add_client(Thread_capability thread) {
			return call<Rpc_add_client>(thread); }

		void remove_client(Pager_capability pager) {
			call<Rpc_remove_client>(pager); }

		void fault_handler(Signal_context_capability handler) {
			call<Rpc_fault_handler>(handler); }

		State state() {
			return call<Rpc_state>(); }

		Dataspace_capability dataspace() {
			if (!_rm_ds_cap.valid())
				_rm_ds_cap = call<Rpc_dataspace>();
			return _rm_ds_cap;
		}
	};
}

#endif /* _INCLUDE__RM_SESSION__CLIENT_H_ */
