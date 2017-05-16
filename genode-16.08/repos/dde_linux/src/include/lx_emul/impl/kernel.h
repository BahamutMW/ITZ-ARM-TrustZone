/*
 * \brief  Implementation of linux/kernel.h
 * \author Norman Feske
 * \date   2015-09-09
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/snprintf.h>


int sprintf(char *str, const char *format, ...)
{
	enum { BUFFER_LEN = 128 };
	va_list list;

	va_start(list, format);
	Genode::String_console sc(str, BUFFER_LEN);
	sc.vprintf(format, list);
	va_end(list);

	return sc.len();
}


int snprintf(char *buf, size_t size, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	Genode::String_console sc(buf, size);
	sc.vprintf(fmt, args);
	va_end(args);

	return sc.len();
}
