/*
 * \brief  A Qt Widget that shows a nitpicker view
 * \author Christian Prochaska
 * \date   2010-08-26
 */

#include <base/ipc.h>

#include <string.h>

#include <QtGui>

#include <qnitpickerplatformwindow.h>
#include <qnitpickerviewwidget/qnitpickerviewwidget.h>


/*************************
 ** QEmbeddedViewWidget **
 *************************/

QEmbeddedViewWidget::QEmbeddedViewWidget(QWidget *parent) { }


QEmbeddedViewWidget::~QEmbeddedViewWidget() { }


QEmbeddedViewWidget::View_geometry QEmbeddedViewWidget::_calc_view_geometry()
{
	/* mark all sliders as unchecked */
	QHashIterator<QScrollBar*, bool> i(_scrollbars);
	while(i.hasNext()) {
		i.next();
		_scrollbars.insert(i.key(), false);
	}

	QWidget *parent = parentWidget();

	int w = 0;
	int h = 0;

	int diff_x = 0;
	int diff_y = 0;

	int x0 = mapToGlobal(QPoint(0, 0)).x();
	int x1 = mapToGlobal(QPoint(_orig_w - 1, 0)).x();
	int y0 = mapToGlobal(QPoint(0, 0)).y();
	int y1 = mapToGlobal(QPoint(0, _orig_h - 1)).y();

	while(parent) {

		if (parent->inherits("QAbstractScrollArea")) {
			QAbstractScrollArea *scrollarea = qobject_cast<QAbstractScrollArea*>(parent);
			QScrollBar *scrollbar;

			scrollbar = scrollarea->horizontalScrollBar();
			if (!_scrollbars.contains(scrollbar)) {
				connect(scrollbar, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
				connect(scrollbar, SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject*)));
			}
			/* update/insert value */
			_scrollbars.insert(scrollbar, true);

			scrollbar = scrollarea->verticalScrollBar();
			if (!_scrollbars.contains(scrollbar)) {
				connect(scrollbar, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
				connect(scrollbar, SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject*)));
			}
			/* update/insert value */
			_scrollbars.insert(scrollbar, true);
		}

		x0 = qMax(x0, parent->mapToGlobal(parent->contentsRect().topLeft()).x());
		x1 = qMin(x1, parent->mapToGlobal(parent->contentsRect().bottomRight()).x());
		y0 = qMax(y0, parent->mapToGlobal(parent->contentsRect().topLeft()).y());
		y1 = qMin(y1, parent->mapToGlobal(parent->contentsRect().bottomRight()).y());

		w = x1 - x0 + 1;
		h = y1 - y0 + 1;

		if (parent->childrenRect().x() < 0) {
			diff_x += parent->childrenRect().x();
		}

		if (parent->childrenRect().y() < 0) {
			diff_y += parent->childrenRect().y();
		}

		parent = parent->parentWidget();
	}

	/* disconnect and remove any scrollbar that is not in the hierarchy anymore */
	i.toBack();
	while(i.hasNext()) {
		i.next();
		if (_scrollbars.value(i.key()) == false) {
			disconnect(i.key(), SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
			disconnect(i.key(), SIGNAL(destroyed(QObject*)), this, SLOT(destroyed(QObject*)));
			_scrollbars.remove(i.key());
		}
	}

	return QEmbeddedViewWidget::View_geometry { x0, y0, w, h,
	                                            _orig_buf_x + diff_x,
	                                            _orig_buf_y + diff_y };
}


void QEmbeddedViewWidget::valueChanged()
{
	if (isVisible()) {
		QPaintEvent e(rect());
		paintEvent(&e);
	}
}


void QEmbeddedViewWidget::destroyed(QObject *obj)
{
	_scrollbars.remove(qobject_cast<QScrollBar*>(obj));
}


/**************************
 ** QNitpickerViewWidget **
 **************************/

QNitpickerViewWidget::QNitpickerViewWidget(QWidget *parent)
:
	QEmbeddedViewWidget(parent), nitpicker(0)
{ }


void QNitpickerViewWidget::setNitpickerView(Nitpicker::Session_client *new_nitpicker,
                                            Nitpicker::Session::View_handle new_view_handle,
                                            int buf_x, int buf_y, int w, int h)
{
	QEmbeddedViewWidget::_orig_geometry(w, h, buf_x, buf_y);

	nitpicker = new_nitpicker;
	view_handle = new_view_handle;
	setFixedSize(w, h);
}


QNitpickerViewWidget::~QNitpickerViewWidget() { }


void QNitpickerViewWidget::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
}


void QNitpickerViewWidget::hideEvent(QHideEvent *event)
{
	QWidget::hideEvent(event);

	if (nitpicker) {

		QEmbeddedViewWidget::View_geometry const view_geometry =
			QEmbeddedViewWidget::_calc_view_geometry();

		typedef Nitpicker::Session::Command Command;

		Nitpicker::Rect const geometry(Nitpicker::Point(mapToGlobal(pos()).x(),
		                                                mapToGlobal(pos()).y()),
		                               Nitpicker::Area(0, 0));
		nitpicker->enqueue<Command::Geometry>(view_handle, geometry);

		Nitpicker::Point const offset(view_geometry.buf_x, view_geometry.buf_y);
		nitpicker->enqueue<Command::Offset>(view_handle, offset);

		nitpicker->execute();
	}
}


void QNitpickerViewWidget::paintEvent(QPaintEvent *event)
{
	QWidget::paintEvent(event);

	if (!nitpicker)
		return;

	QEmbeddedViewWidget::View_geometry const view_geometry =
		QEmbeddedViewWidget::_calc_view_geometry();

	typedef Nitpicker::Session::Command Command;

	/* argument to mapToGlobal() is relative to the Widget's origin
	 * the plugin view starts at (0, 0)
	 */
	if (mask().isEmpty()) {

		Nitpicker::Rect const geometry(Nitpicker::Point(view_geometry.x,
		                                                view_geometry.y),
		                               Nitpicker::Area(view_geometry.w,
		                                               view_geometry.h));
		nitpicker->enqueue<Command::Geometry>(view_handle, geometry);
		Nitpicker::Point const offset(view_geometry.buf_x,
		                              view_geometry.buf_y);
		nitpicker->enqueue<Command::Offset>(view_handle, offset);
	} else {

		Nitpicker::Rect const
			geometry(Nitpicker::Point(mapToGlobal(mask().boundingRect().topLeft()).x(),
			                          mapToGlobal(mask().boundingRect().topLeft()).y()),
			         Nitpicker::Area(mask().boundingRect().width(),
			                         mask().boundingRect().height()));
		nitpicker->enqueue<Command::Geometry>(view_handle, geometry);

		Nitpicker::Point const offset(view_geometry.buf_x, view_geometry.buf_y);
		nitpicker->enqueue<Command::Offset>(view_handle, offset);
	}

	/* bring the plugin view to the front of the Qt window */
	QNitpickerPlatformWindow *platform_window =
		dynamic_cast<QNitpickerPlatformWindow*>(window()->windowHandle()->handle());

	Nitpicker::Session::View_handle const neighbor_handle =
		nitpicker->view_handle(platform_window->view_cap());

	nitpicker->enqueue<Command::To_front>(view_handle, neighbor_handle);
	nitpicker->execute();

	nitpicker->release_view_handle(neighbor_handle);
}


void QNitpickerViewWidget::focusInEvent(QFocusEvent *)
{
	QNitpickerPlatformWindow *platform_window =
		dynamic_cast<QNitpickerPlatformWindow*>(window()->windowHandle()->handle());

	platform_window->nitpicker().focus(*nitpicker);
}
