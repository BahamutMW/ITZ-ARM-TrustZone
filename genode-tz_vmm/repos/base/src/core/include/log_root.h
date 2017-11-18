/*
 * \brief  Log root interface
 * \author Norman Feske
 * \date   2006-09-15
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__LOG_ROOT_H_
#define _CORE__INCLUDE__LOG_ROOT_H_

#include <root/component.h>
#include <util/arg_string.h>

#include "log_session_component.h"

namespace Genode {

	class Log_root : public Root_component<Log_session_component>
	{
		protected:

			/**
			 * Root component interface
			 */
			Log_session_component *_create_session(const char *args)
			{
				char label_buf[Log_session_component::LABEL_LEN];

				Arg label_arg = Arg_string::find_arg(args, "label");
				label_arg.string(label_buf, sizeof(label_buf), "");

				return new (md_alloc()) Log_session_component(label_buf);
			}

		public:

			/**
			 * Constructor
			 *
			 * \param session_ep  entry point for managing cpu session objects
			 * \param md_alloc    meta-data allocator to be used by root component
			 */
			Log_root(Rpc_entrypoint *session_ep, Allocator *md_alloc):
				Root_component<Log_session_component>(session_ep, md_alloc) { }
	};
}

#endif /* _CORE__INCLUDE__LOG_ROOT_H_ */
