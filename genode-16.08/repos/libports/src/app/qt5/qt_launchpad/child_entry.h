/*
 * \brief  Child entry widget interface
 * \author Christian Prochaska
 * \date   2008-04-06
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef CHILD_ENTRY_H
#define CHILD_ENTRY_H

#include <launchpad/launchpad.h>

#include <QWidget>

#include "ui_child_entry.h"

class Child_entry : public QWidget
{
	Q_OBJECT

	private:

		Ui::Child_entryClass ui;

		Launchpad *_launchpad;
		Launchpad_child *_launchpad_child;

	private slots:

		void on_exitButton_clicked();

	public:

		Child_entry(const char *name, int quota_kb, int max_quota_kb,
		            Launchpad *launchpad, Launchpad_child *launchpad_child,
		            QWidget *parent = 0);
};

#endif /* CHILD_ENTRY_H */
