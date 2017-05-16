/*
 * \brief  Decorator configuration handling
 * \author Norman Feske
 * \date   2015-09-17
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Genode includes */
#include <os/session_policy.h>
#include <util/color.h>

/* decorator includes */
#include <decorator/types.h>

namespace Decorator {

	class Config;

	typedef Genode::String<200> Window_title;
}


class Decorator::Config
{
	public:

		/**
		 * Return the base color of the window with the specified title
		 */
		Color base_color(Window_title const &title) const
		{
			Color result(0, 0, 0);

			try {
				Genode::Session_policy policy(title);
				result = policy.attribute_value("color", result);

			} catch (Genode::Session_policy::No_policy_defined) { }

			return result;
		}
};

#endif /* _CONFIG_H_ */
