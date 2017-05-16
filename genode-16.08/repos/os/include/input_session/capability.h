/*
 * \brief  Input-session capability type
 * \author Norman Feske
 * \date   2008-08-16
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__INPUT_SESSION__CAPABILITY_H_
#define _INCLUDE__INPUT_SESSION__CAPABILITY_H_

#include <base/capability.h>
#include <input_session/input_session.h>

namespace Input { typedef Genode::Capability<Session> Session_capability; }

#endif /* _INCLUDE__INPUT_SESSION__CAPABILITY_H_ */
