TOCK_USERLAND_BASE_DIR ?= ..
LIBNAME := lvgl
$(LIBNAME)_DIR := $(TOCK_USERLAND_BASE_DIR)/$(LIBNAME)

$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/core/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/draw/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/layouts/flex/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/layouts/grid/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/themes/basic/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/themes/default/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/themes/mono/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/mono/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/animimg/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/calendar/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/chart/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/colorwheel/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/imgbtn/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/keyboard/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/led/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/list/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/meter/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/msgbox/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/span/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/spinbox/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/spinner/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/tabview/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/tileview/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/extra/widgets/win/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/font/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/gpu/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/hal/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/misc/*.c)
$(LIBNAME)_SRCS  += $(wildcard $($(LIBNAME)_DIR)/lvgl/src/widgets/*.c)

# Avoid failing in CI due to warnings in the library.
override CPPFLAGS_$(LIBNAME) += -Wno-error

include $(TOCK_USERLAND_BASE_DIR)/TockLibrary.mk

