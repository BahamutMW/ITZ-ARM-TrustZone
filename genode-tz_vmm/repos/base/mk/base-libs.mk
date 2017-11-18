#
# Genode base libaries
#
# These static libraries are filtered out when linking dynamically linked
# binaries.
#
BASE_LIBS = base-common base startup cxx timed_semaphore alarm config syscall

#
# Name of Genode's dynamic linker
#
DYNAMIC_LINKER = ld
