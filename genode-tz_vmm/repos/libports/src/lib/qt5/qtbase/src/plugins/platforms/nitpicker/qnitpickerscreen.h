/*
 * \brief  QNitpickerScreen
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#ifndef _QNITPICKERSCREEN_H_
#define _QNITPICKERSCREEN_H_

#include <qpa/qplatformscreen.h>

#include <QDebug>

/* Genode includes */
#include <nitpicker_session/connection.h>

QT_BEGIN_NAMESPACE

class QNitpickerScreen : public QPlatformScreen
{
	private:

		Nitpicker::Connection _nitpicker;
		QRect _geometry;

	public:

		QNitpickerScreen()
		{
			Framebuffer::Mode const scr_mode = _nitpicker.mode();

			if (scr_mode.format() != Framebuffer::Mode::RGB565)
				qCritical() << "Nitpicker screen format is not RGB565";

			_geometry.setRect(0, 0, scr_mode.width(),
			                        scr_mode.height());

			Genode::env()->parent()->close(_nitpicker.cap());
		}

		QRect geometry() const { return _geometry; }
		int depth() const { return 16; }
		QImage::Format format() const { return QImage::Format_RGB16; }
		QDpi logicalDpi() const { return QDpi(80, 80); };
};

QT_END_NAMESPACE

#endif /* _QNITPICKERSCREEN_H_ */
