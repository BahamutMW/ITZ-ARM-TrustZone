/*
 * \brief  I/O-memory session interface
 * \author Christian Helmuth
 * \date   2006-08-01
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__IO_MEM_SESSION__IO_MEM_SESSION_H_
#define _INCLUDE__IO_MEM_SESSION__IO_MEM_SESSION_H_

#include <dataspace/capability.h>
#include <session/session.h>

namespace Genode {

	struct Io_mem_dataspace;
	struct Io_mem_session;

	typedef Capability<Io_mem_dataspace> Io_mem_dataspace_capability;
}


struct Genode::Io_mem_dataspace : Dataspace { };


struct Genode::Io_mem_session : Session
{
	static const char *service_name() { return "IO_MEM"; }

	virtual ~Io_mem_session() { }

	/**
	 * Request dataspace containing the IO_MEM session data
	 *
	 * \return  capability to IO_MEM dataspace
	 *          (may be invalid)
	 */
	virtual Io_mem_dataspace_capability dataspace() = 0;


	/*********************
	 ** RPC declaration **
	 *********************/

	GENODE_RPC(Rpc_dataspace, Io_mem_dataspace_capability, dataspace);
	GENODE_RPC_INTERFACE(Rpc_dataspace);
};

#endif /* _INCLUDE__IO_MEM_SESSION__IO_MEM_SESSION_H_ */
