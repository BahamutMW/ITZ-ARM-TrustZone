PROGRAM_PREFIX = genode-arm-
GCC_TARGET     = arm-none-eabi

# cross-compiling does not work yet
REQUIRES = arm

include $(PRG_DIR)/../gcc/target.inc
