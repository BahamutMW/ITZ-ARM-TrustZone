/*
 * \brief  Nitpicker-based virtual framebuffer
 * \author Norman Feske
 * \date   2006-09-21
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/rpc_server.h>
#include <base/signal.h>
#include <cap_session/connection.h>
#include <os/config.h>
#include <scout/user_state.h>
#include <scout/nitpicker_graphics_backend.h>

#include "framebuffer_window.h"
#include "services.h"


/**
 * Runtime configuration
 */
namespace Scout { namespace Config
{
	int iconbar_detail    = 1;
	int background_detail = 1;
	int mouse_cursor      = 1;
	int browser_attr      = 0;
} }


void Scout::Launcher::launch() { }


class Background_animator : public Scout::Tick
{
	private:

		Framebuffer_window<Scout::Pixel_rgb565> *_fb_win;

		int _bg_offset;

	public:

		/**
		 * Constructor
		 */
		Background_animator(Framebuffer_window<Scout::Pixel_rgb565> *fb_win):
			_fb_win(fb_win), _bg_offset(0) {
			schedule(20); }

		/**
		 * Tick interface
		 */
		int on_tick()
		{
			_fb_win->bg_offset(_bg_offset);
			_bg_offset += 2;
			_fb_win->refresh();

			/* schedule next tick */
			return 1;
		}
};


/**
 * Animated background
 */
static bool config_animate = true;
static bool config_alpha   = true;

/**
 * Size and position of virtual frame buffer
 */
static long config_fb_width  = 500;
static long config_fb_height = 400;
static long config_fb_x      = 400;
static long config_fb_y      = 260;

/**
 * Window title
 */
static const char *config_title = "Liquid Framebuffer";

/**
 * Resize handle
 */
static bool config_resize_handle = false;

/**
 * Window decoration
 */
static bool config_decoration = true;

/**
 * Parse configuration
 */
static void read_config()
{
	using namespace Genode;

	Xml_node config_node = config()->xml_node();

	try {
		char buf[16];
		config_node.attribute("animate").value(buf, sizeof(buf));

		if      (!strcmp("off", buf)) config_animate = false;
		else if (!strcmp("on",  buf)) config_animate = true;
		else
			Genode::printf("Warning: invalid value for animate declaration,\n"
			               "         valid values are 'on', 'off.\n'");
	} catch (Xml_node::Nonexistent_attribute) { }

	config_alpha = config_animate;

	try { config_node.attribute("xpos").value(&config_fb_x); }
	catch (Xml_node::Nonexistent_attribute) { }

	try { config_node.attribute("ypos").value(&config_fb_y); }
	catch (Xml_node::Nonexistent_attribute) { }

	try { config_node.attribute("width").value(&config_fb_width); }
	catch (Xml_node::Nonexistent_attribute) { }

	try { config_node.attribute("height").value(&config_fb_height); }
	catch (Xml_node::Nonexistent_attribute) { }

	try {
		static char buf[64];
		config_node.attribute("title").value(buf, sizeof(buf));
		config_title = buf;
	} catch (Xml_node::Nonexistent_attribute) { }

	try {
		char buf[16];
		config_node.attribute("decoration").value(buf, sizeof(buf));

		if      (!strcmp("off", buf)) config_decoration = false;
		else if (!strcmp("on",  buf)) config_decoration = true;
		else
			Genode::printf("Warning: invalid value for decoration declaration,\n"
			               "         valid values are 'on', 'off.\n'");
	} catch (Xml_node::Nonexistent_attribute) { }

	try {
		char buf[16];
		config_node.attribute("resize_handle").value(buf, sizeof(buf));

		if      (!strcmp("off", buf)) config_resize_handle = false;
		else if (!strcmp("on",  buf)) config_resize_handle = true;
		else
			Genode::printf("Warning: invalid value for resize_handle declaration,\n"
			               "         valid values are 'on', 'off.\n'");
	} catch (Xml_node::Nonexistent_attribute) { }

}


/*******************
 ** Input handler **
 *******************/

struct Input_handler
{
	GENODE_RPC(Rpc_handle_input, void, handle, Scout::Event&);
	GENODE_RPC_INTERFACE(Rpc_handle_input);
};


class Input_handler_component : public Genode::Rpc_object<Input_handler,
                                                          Input_handler_component>
{

	private:

		Scout::Platform                          &_pf;
		Scout::User_state                        &_user_state;
		Framebuffer_window<Genode::Pixel_rgb565> &_fb_win;
		Genode::Signal_receiver                  &_sig_rec;
		unsigned long                             _curr_time, _old_time;

	public:

		Input_handler_component(Scout::Platform &pf,
		                        Scout::User_state &user_state,
		                        Framebuffer_window<Genode::Pixel_rgb565> &fb_win,
		                        Genode::Signal_receiver &sig_rec)
		:
			_pf(pf),
			_user_state(user_state),
			_fb_win(fb_win),
			_sig_rec(sig_rec)
		{
			_curr_time = _old_time = _pf.timer_ticks();
		}

		void handle(Scout::Event &ev)
		{
			using Scout::Event;

			if (ev.type != Event::WHEEL)
				ev.mouse_position = ev.mouse_position - _user_state.view_position();

			/* direct all keyboard events to the window content */
			if ((ev.type == Event::PRESS || ev.type == Event::RELEASE)
			 && (ev.code != Event::BTN_LEFT))
				window_content()->handle_event(ev);
			else
				_user_state.handle_event(ev);

			if (ev.type == Event::TIMER) {
				Scout::Tick::handle(_pf.timer_ticks());
				/* check for configuration changes */
				if (_sig_rec.pending()) {
					_sig_rec.wait_for_signal();
					Genode::config()->reload();
					/* keep the current values by default */
					config_fb_x = _fb_win.view_x();
					config_fb_y = _fb_win.view_y();
					config_fb_width = _fb_win.view_w();
					config_fb_height = _fb_win.view_h();
					try { read_config(); } catch (...) { }
					_fb_win.name(config_title);
					_fb_win.config_alpha(config_alpha);
					_fb_win.config_resize_handle(config_resize_handle);
					_fb_win.config_decoration(config_decoration);
					/* must get called after 'config_decoration()' */
					_fb_win.content_geometry(config_fb_x, config_fb_y,
					                         config_fb_width, config_fb_height);
					_user_state.update_view_offset();
				}
			}

			/* perform periodic redraw */
			_curr_time = _pf.timer_ticks();
			if (!_pf.event_pending() && ((_curr_time - _old_time > 20) || (_curr_time < _old_time))) {
				_old_time = _curr_time;
				_fb_win.process_redraw();
			}
		}
};


/**
 * Main program
 */
int main(int argc, char **argv)
{
	using namespace Scout;

	try { read_config(); } catch (...) { }

	/*
	 * Register signal handler for config changes
	 */
	static Genode::Signal_receiver sig_rec;
	static Genode::Signal_context sig_ctx;

	try { Genode::config()->sigh(sig_rec.manage(&sig_ctx)); } catch (...) { }

	/* heuristic for allocating the double-buffer backing store */
	enum { WINBORDER_WIDTH = 10, WINBORDER_HEIGHT = 40 };

	/* init platform */
	static Nitpicker::Connection nitpicker;
	static Platform pf(*nitpicker.input());

	Area  const max_size(config_fb_width  + WINBORDER_WIDTH,
	                     config_fb_height + WINBORDER_HEIGHT);
	Point const initial_position(config_fb_x, config_fb_y);
	Area  const initial_size = max_size;

	static Nitpicker_graphics_backend
		graphics_backend(nitpicker, max_size, initial_position, initial_size);

	/* initialize our window content */
	init_window_content(config_fb_width, config_fb_height, config_alpha);

	/* create instance of browser window */
	static Framebuffer_window<Pixel_rgb565>
		fb_win(graphics_backend, window_content(),
		       initial_position, initial_size, max_size,
		       config_title, config_alpha,
		       config_resize_handle, config_decoration);

	if (config_animate) {
		static Background_animator fb_win_bg_anim(&fb_win);
	}

	/* create user state manager */
	static User_state user_state(&fb_win, &fb_win,
	                             initial_position.x(), initial_position.y());
	fb_win.parent(&user_state);
	fb_win.content_geometry(config_fb_x, config_fb_y,
	                        config_fb_width, config_fb_height);

	/* initialize server entry point */
	enum { STACK_SIZE = 2*1024*sizeof(Genode::addr_t) };
	static Genode::Cap_connection cap;
	static Genode::Rpc_entrypoint ep(&cap, STACK_SIZE, "liquid_fb_ep");

	/* initialize public services */
	init_services(ep);

	/* create local input handler service */
	static Input_handler_component input_handler(pf, user_state, fb_win,
	                                             sig_rec);
	Genode::Capability<Input_handler> input_handler_cap = ep.manage(&input_handler);

	/* enter main loop */
	for (;;) {
		Event ev = pf.get_event();
		input_handler_cap.call<Input_handler::Rpc_handle_input>(ev);

		if (ev.type == Event::QUIT)
			break;
	}

	return 0;
}
