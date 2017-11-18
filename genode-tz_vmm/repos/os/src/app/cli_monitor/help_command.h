/*
 * \brief  Help command
 * \author Norman Feske
 * \date   2013-03-18
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _HELP_COMMAND_H_
#define _HELP_COMMAND_H_

struct Help_command : Command
{
	Help_command() : Command("help", "brief help information") { }

	void execute(Command_line &, Terminal::Session &terminal)
	{
		tprintf(terminal, "  Press [tab] for a list of commands.\n");
		tprintf(terminal, "  When given a command, press [tab] for a list of arguments.\n");
	}
};

#endif /* _HELP_COMMAND_H_ */
