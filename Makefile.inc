mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=
Tracer.o\
Color.o\
Odometry.o\

APPL_CXXOBJS +=\

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_LIBS += -lm

APPL_DIRS += $(mkfile_path)Tracer
APPL_DIRS += $(mkfile_path)Color
APPL_DIRS += $(mkfile_path)Odometry

INCLUDES += \
 -I$(mkfile_path)Tracer\
 -I$(mkfile_path)Color\
 -I$(mkfile_path)Odometry\

#COPTS += -fno-use-cxa-atexit
