LIBVMI_DIR := $(call select_from_ports,libvmi)/src/lib/libvmi

SRC_C = $(notdir $(wildcard $(LIBVMI_DIR)/*.c))

vpath %.c $(LIBVMI_DIR)

INC_DIR += $(LIBVMI_DIR) $(LIBVMI_DIR)/libvmi $(LIBVMI_DIR)/libvmi/arch $(LIBVMI_DIR)libvmi/driver $(LIBVMI_DIR)libvmi/driver/file $(LIBVMI_DIR)/libvmi/os

LIBS += libc glib flex

SHARED_LIB = yes
