FLEX_DIR := $(call select_from_ports,flex)/src/lib/flex

SRC_C = $(notdir $(wildcard $(FLEX_DIR)/*.c))

vpath %.c $(FLEX_DIR)

INC_DIR += $(FLEX_DIR) $(FLEX_DIR)/src

LIBS += libc

SHARED_LIB = yes
