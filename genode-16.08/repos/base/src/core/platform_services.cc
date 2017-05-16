/*
 * \brief  Dummy implemntation of platform-specific services
 * \author Stefan Kalkowski
 * \date   2012-10-26
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <platform_services.h>


void Genode::platform_add_local_services(Rpc_entrypoint*, Sliced_heap*,
                                         Service_registry*) { }
