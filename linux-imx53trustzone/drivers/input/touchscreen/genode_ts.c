/*
 * Touchscreen driver for Genode's TrustZone VMM
 *
 * Copyright (C) 2013 Genode Labs GmbH
 * Stefan Kalkowski <stefan.kalkowski@genode-labs.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/slab.h>

enum {
	INVALID,
	PRESS,
	RELEASE,
	MOTION,
};


static unsigned int         poll_sleep = HZ / 10;
static struct timer_list    input_timer;
static int                  in_touch = 0;
struct input_dev           *mpr121;

static int genode_touch_get_event(int *x, int *y, int *code)
{
	int type = INVALID;
	asm(".arch_extension sec\n\t");
	asm volatile ("mov  r0, #1     \n"
				  "mov  r1, #0     \n"
				  "isb             \n"
				  "dsb             \n"
				  "smc  #0         \n"
				  "mov  %[t], r0   \n"
				  "mov  %[x], r1   \n"
				  "mov  %[y], r2   \n"
				  "mov  %[c], r3   \n"
				  : [t] "=&r" (type), [x] "=&r" (*x), [y] "=&r" (*y),
				    [c] "=&r" (*code)
				  :: "memory", "r0", "r1", "r2", "r3");

	return type;
}


static void genode_touch_poll_for_events(unsigned long data)
{
	struct input_dev *input_dev = (struct input_dev *) data;

	int type = INVALID;
	int x = 0, y = 0, keycode = 0;
	while ((type = genode_touch_get_event(&x, &y, &keycode)) != INVALID) {
		if (keycode == BTN_LEFT || type == MOTION) {
			x = (x * 32767) / 1024;
			y = (y * 32767) / 768;

			if (type == RELEASE) {
				input_report_abs(input_dev, ABS_MT_TRACKING_ID, 0);
				input_event(input_dev, EV_ABS, ABS_MT_TOUCH_MAJOR, 0);
				input_mt_sync(input_dev);
			} else {
				input_report_abs(input_dev, ABS_MT_TRACKING_ID, 0);
				input_report_abs(input_dev, ABS_MT_TOUCH_MAJOR, 1);
				input_report_abs(input_dev, ABS_MT_POSITION_X, x);
				input_report_abs(input_dev, ABS_MT_POSITION_Y, y);
				input_mt_sync(input_dev);
			}
			input_sync(input_dev);
		} else if (type == RELEASE || type == PRESS) {
			input_report_key(mpr121, keycode, (type == PRESS) ? 1 : 0);
			input_sync(mpr121);
		}
	}
	mod_timer(&input_timer, jiffies + poll_sleep);
}


static int genode_touch_open(struct input_dev *dev)
{
	input_timer.function = genode_touch_poll_for_events;
	input_timer.expires  = jiffies + poll_sleep;
	input_timer.data     = (unsigned long)dev;
	add_timer(&input_timer);
	return 0;
}


static void genode_touch_close(struct input_dev *dev)
{
	del_timer(&input_timer);
}

static int genode_gpio_open(struct input_dev *dev) {
	return 0; }


static void genode_gpio_close(struct input_dev *dev) { }


static struct input_dev genode_touch_dev = {
	.name  = "genode-touch",
	.open  = genode_touch_open,
	.close = genode_touch_close,
};


static u16 tkey_matrix[4] = {
    KEY_BACK, KEY_HOME, KEY_MENU, KEY_POWER,
};

static int __init genode_touch_init(void)
{
	int ret, i;
	struct input_dev *input_dev = input_allocate_device();
	if (!input_dev)
		return -ENOMEM;

	input_dev->name  = "eGalax Touch Screen";
	input_dev->id.vendor = 0x0EEF;
	input_dev->id.product = 0x0020;
	input_dev->id.version = 0x0001;
	input_dev->open  = genode_touch_open;
	input_dev->close = genode_touch_close;

	__set_bit(EV_ABS, input_dev->evbit);
	__set_bit(EV_KEY, input_dev->evbit);
	__set_bit(BTN_TOUCH, input_dev->keybit);
	__set_bit(ABS_X, input_dev->absbit);
	__set_bit(ABS_Y, input_dev->absbit);
	__set_bit(ABS_PRESSURE, input_dev->absbit);
	input_set_abs_params(input_dev, ABS_X, 0, 32767, 0, 0);
	input_set_abs_params(input_dev, ABS_Y, 0, 32767,  0, 0);

	input_set_abs_params(input_dev, ABS_MT_POSITION_X, 0, 32767, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_POSITION_Y, 0, 32767, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TOUCH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_WIDTH_MAJOR, 0, 255, 0, 0);
	input_set_abs_params(input_dev, ABS_MT_TRACKING_ID, 0, 5, 0, 0);

	ret = input_register_device(input_dev);
	if (ret) {
		input_free_device(input_dev);
		return ret;
	}

	input_dev = input_allocate_device();
	if (!input_dev)
		return -ENOMEM;

	input_dev->name        = "FSL MPR121 Touchkey";
	input_dev->id.bustype  = BUS_I2C;
	input_dev->keycode     = tkey_matrix;
	input_dev->keycodesize = sizeof(tkey_matrix[0]);
	input_dev->keycodemax  = 4;
	for (i = 0; i < input_dev->keycodemax; i++) {
		__set_bit(tkey_matrix[i], input_dev->keybit);
		input_set_capability(input_dev, EV_KEY, tkey_matrix[i]);
	}

	ret = input_register_device(input_dev);
	if (ret) {
		input_free_device(input_dev);
		return ret;
	}
	mpr121 = input_dev;

	init_timer(&input_timer);
	return 0;
}
module_init(genode_touch_init);

static void __exit genode_touch_exit(void)
{
	input_unregister_device(&genode_touch_dev);
}
module_exit(genode_touch_exit);

MODULE_DESCRIPTION("Touchscreen driver for Genode TZ VMM");
MODULE_AUTHOR("Stefan Kalkowski <stefan.kalkowski@genode-labs.com>");
MODULE_LICENSE("GPL");
