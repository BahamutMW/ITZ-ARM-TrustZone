LIBC_STDLIB_DIR = $(LIBC_DIR)/lib/libc/stdlib
FILTER_OUT = exit.c atexit.c malloc.c

#SRC_C = $(notdir $(wildcard $(LIBC_STDLIB_DIR)/*.c))
SRC_C = $(filter-out $(FILTER_OUT),$(notdir $(wildcard $(LIBC_STDLIB_DIR)/*.c)))

include $(REP_DIR)/lib/mk/libc-common.inc

vpath %.c $(LIBC_STDLIB_DIR)
