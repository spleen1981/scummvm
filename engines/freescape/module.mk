MODULE := engines/freescape
 
MODULE_OBJS := \
	metaengine.o \
	freescape.o \
	area.o \
	objects/object.o \
	objects/geometricobject.o \
	loaders/8bitBinaryLoader.o \
	language/8bitDetokeniser.o \
	loaders/16bitBinaryLoader.o \
	language/16bitDetokeniser.o \
	language/instruction.o
 
MODULE_DIRS += \
	engines/freescape

 
# This module can be built as a plugin
ifeq ($(ENABLE_FREESCAPE), DYNAMIC_PLUGIN)
PLUGIN := 1
endif
 
# Include common rules 
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
