/*
 * \brief  Cpu session interface extension for NOVA
 * \author Alexander Boettcher
 * \date   2012-07-27
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__NOVA_CPU_SESSION__NOVA_CPU_SESSION_H_
#define _INCLUDE__NOVA_CPU_SESSION__NOVA_CPU_SESSION_H_

#include <base/stdint.h>
#include <cpu_session/cpu_session.h>

namespace Genode {

	struct Nova_cpu_session : Cpu_session
	{
		virtual ~Nova_cpu_session() { }

		virtual Native_capability pause_sync(Thread_capability) = 0;
		virtual Native_capability single_step_sync(Thread_capability, bool) = 0;


		/*********************
		 ** RPC declaration **
		 *********************/

		GENODE_RPC(Rpc_pause_sync, Native_capability, pause_sync,
		           Thread_capability);
		GENODE_RPC(Rpc_single_step_sync, Native_capability, single_step_sync,
		           Thread_capability, bool);

		GENODE_RPC_INTERFACE_INHERIT(Cpu_session, Rpc_pause_sync,
		                             Rpc_single_step_sync);
	};
}

#endif /* _INCLUDE__NOVA_CPU_SESSION__NOVA_CPU_SESSION_H_ */
