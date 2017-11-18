/*
 * \brief  Libc dummies required by Python
 * \author Sebastian Sumpf
 * \date   2010-02-17
 */

/*
 * Copyright (C) 2010-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <unistd.h>

int dup(int oldfd)
{
	return oldfd;
}

