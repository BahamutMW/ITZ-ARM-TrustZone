/*
 * \brief  Utility for implementing blocking syscalls
 * \author Norman Feske
 * \date   2011-11-05
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NOUX__WAKE_UP_NOTIFIER__H_
#define _NOUX__WAKE_UP_NOTIFIER__H_

/* Genode includes */
#include <util/list.h>
#include <base/lock.h>

namespace Noux {

	struct Wake_up_notifier : List<Wake_up_notifier>::Element
	{
		Lock *lock;

		Wake_up_notifier(Lock *lock = 0)
		: lock(lock) { }

		void wake_up()
		{
			if (lock)
				lock->unlock();
		}
	};
};

#endif /* _NOUX__WAKE_UP_NOTIFIER__H_ */
