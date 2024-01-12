# Base folder definitions
TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := u8g2
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

# List all C and Assembly files
$(LIBNAME)_SRCS_ALL  += $(wildcard u8g2-master/csrc/u8g2*.c)
$(LIBNAME)_SRCS_ALL  += $(wildcard u8g2-master/csrc/u8x8*.c)

$(LIBNAME)_SRCS_ALL  += u8g2-tock.c

# Remove the buffer file, we need to create our own tock-specific version.
$(LIBNAME)_SRCS := $(filter-out u8g2-master/csrc/u8g2_buffer.c,$($(LIBNAME)_SRCS_ALL))

# Need libtock headers
override CPPFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/libtock

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk
