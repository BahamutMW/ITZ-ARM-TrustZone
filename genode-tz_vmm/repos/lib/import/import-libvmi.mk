LIBVMI_PORT_DIR := $(call select_from_ports,libvmi)

INC_DIR += $(LIBVMI_PORT_DIR)/src/lib/libvmi/libvmi $(LIBVMI_PORT_DIR)/src/lib/libvmi/libvmi/arch $(LIBVMI_PORT_DIR)/src/lib/libvmi/libvmi/config $(LIBVMI_PORT_DIR)/src/lib/libvmi/libvmi/driver $(LIBVMI_DIR)libvmi/driver/file $(LIBVMI_PORT_DIR)/src/lib/libvmi/libvmi/os
