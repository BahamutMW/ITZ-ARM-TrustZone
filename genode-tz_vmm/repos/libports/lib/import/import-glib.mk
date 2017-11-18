GLIB_PORT_DIR := $(call select_from_ports,glib)

INC_DIR += $(GLIB_PORT_DIR)/src/lib/glib $(GLIB_PORT_DIR)/src/lib/glib/glib
