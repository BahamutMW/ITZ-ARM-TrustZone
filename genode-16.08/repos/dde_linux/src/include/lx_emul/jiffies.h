/*
 * \brief  Linux kernel API
 * \author Norman Feske
 * \author Sebastian Sumpf
 * \author Josef Soentgen
 * \date   2014-08-21
 *
 * Based on the prototypes found in the Linux kernel's 'include/'.
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/*********************
 ** linux/jiffies.h **
 *********************/

#define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)

extern unsigned long jiffies;

enum {
	JIFFIES_TICK_MS = 1000/HZ,
	JIFFIES_TICK_US = 1000*1000/HZ,
};

static inline unsigned long msecs_to_jiffies(const unsigned int m) { return m / JIFFIES_TICK_MS; }
static inline unsigned int jiffies_to_msecs(const unsigned long j) { return j * JIFFIES_TICK_MS; }
static inline unsigned long usecs_to_jiffies(const unsigned int u) { return u / JIFFIES_TICK_US; }

clock_t jiffies_to_clock_t(unsigned long x);
static inline clock_t jiffies_delta_to_clock_t(long delta)
{
	return jiffies_to_clock_t(max(0L, delta));
}

#define time_after(a,b)     ((long)((b) - (a)) < 0)
#define time_after_eq(a,b)  ((long)((a) - (b)) >= 0)
#define time_before(a,b)    time_after(b,a)
#define time_before_eq(a,b) time_after_eq(b,a)

#define time_is_after_jiffies(a) time_before(jiffies, a)
