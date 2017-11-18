BYACC_DIR := $(call select_from_ports,byacc)/src/lib/byacc

SRC_C = $(notdir $(wildcard $(BYACC_DIR)/*.c))

vpath %.c $(BYACC_DIR)

INC_DIR += $(BYACC_DIR) $(BYACC_DIR)/src

LIBS += libc

SHARED_LIB = yes
