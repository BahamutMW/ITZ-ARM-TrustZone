/*
 * \brief  RAM-session capability type
 * \author Norman Feske
 * \date   2008-08-16
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__RAM_SESSION__CAPABILITY_H_
#define _INCLUDE__RAM_SESSION__CAPABILITY_H_

#include <base/capability.h>

namespace Genode {

	/*
	 * We cannot include 'ram_session/ram_session.h' because this file relies
	 * on the the 'Ram_session_capability' type.
	 */
	class Ram_session;
	typedef Capability<Ram_session> Ram_session_capability;
}

#endif /* _INCLUDE__RAM_SESSION__CAPABILITY_H_ */
