FLEX_PORT_DIR := $(call select_from_ports,flex)

INC_DIR += $(FLEX_PORT_DIR)/src/lib/flex $(FLEX_PORT_DIR)/src/lib/flex/src
