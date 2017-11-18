/*
 * \brief   Platform-specific parts of cores CPU-service
 * \author  Martin Stein
 * \date    2012-04-17
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>

/* Core includes */
#include <cpu_session_component.h>

using namespace Genode;


Ram_dataspace_capability Cpu_session_component::utcb(Thread_capability)
{
	PDBG("not implemented");
	return Ram_dataspace_capability();
}


Cpu_session::Quota Cpu_session_component::quota() { return Quota(); }
