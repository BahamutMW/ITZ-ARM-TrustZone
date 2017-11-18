GLIB_DIR := $(call select_from_ports,glib)/src/lib/glib

SRC_C = $(notdir $(wildcard $(GLIB_DIR)/*.c))

vpath %.c $(GLIB_DIR)

INC_DIR += $(GLIB_DIR) $(GLIB_DIR)/glib

LIBS += libc

SHARED_LIB = yes
