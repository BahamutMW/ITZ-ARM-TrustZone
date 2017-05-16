/*
 * \brief  Mini C printf()
 * \author Norman Feske
 * \date   2008-10-23
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/printf.h>

extern "C" void printf(const char *format, ...)
{
	va_list list;
	va_start(list, format);

	Genode::vprintf(format, list);

	va_end(list);
}
