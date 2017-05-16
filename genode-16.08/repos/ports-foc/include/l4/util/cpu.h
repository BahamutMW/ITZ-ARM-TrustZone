/*
 * \brief  L4Re functions needed by L4Linux
 * \author Stefan Kalkowski
 * \date   2011-03-17
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _L4__UTIL__CPU_H_
#define _L4__UTIL__CPU_H_

#include <l4/sys/compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned int l4util_cpu_capabilities(void);

#ifdef __cplusplus
}
#endif

#endif /* _L4__UTIL__CPU_H_ */
