/*
 * \brief  Parent_execve interface
 * \author Christian Prochaska
 * \date   2014-01-13
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NOUX__PARENT_EXECVE__H_
#define _NOUX__PARENT_EXECVE__H_

/* Noux includes */
#include <noux_session/sysio.h>

namespace Noux {

	struct Family_member;

	struct Parent_execve
	{
		virtual void execve_child(Family_member &child,
		                          const char *filename,
		                          Args const &args,
		                          Sysio::Env const &env,
		                          bool verbose) = 0;
	};

};

#endif /* _NOUX__PARENT_EXECVE__H_ */

