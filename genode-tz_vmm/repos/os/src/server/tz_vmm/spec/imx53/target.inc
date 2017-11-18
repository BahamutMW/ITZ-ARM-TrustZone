TARGET    = tz_vmm
REQUIRES += trustzone imx53
LIBS     += base 
LIBS     += libc
LIBS     += libcrypto
LIBS     += vfs_jitterentropy
LIBS     += jitterentropy
LIBS     += config
LIBS	 += glib
SRC_CC   += vm.cc
SRC_CC   += main.cc
SRC_CC   += serial.cc
SRC_CC   += block.cc
SRC_CC	 += itz.cc
INC_DIR  += $(PRG_DIR)
INC_DIR  += $(PRG_DIR)/../../include
INC_DIR  += $(PRG_DIR)/../../include/newlibvmi
INC_DIR  += $(PRG_DIR)/../imx53

vpath main.cc $(PRG_DIR)/../imx53
vpath %       $(PRG_DIR)/../..

INC_DIR	+= $(call select_from_ports,openssl)/include
INC_DIR += $(REP_DIR)/src/lib/vfs/jitterentropy
INC_DIR += $(REP_DIR)/src/lib/libvmi/libvmi

