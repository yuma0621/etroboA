mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=

APPL_CXXOBJS +=\
Tracer.o\
Calibration.o\
Motor.o\

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif

APPL_LIBS += -lm

APPL_DIRS += $(mkfile_path)Tracer
APPL_DIRS += $(mkfile_path)Calibration
APPL_DIRS += $(mkfile_path)Motor

INCLUDES += \
 -I$(mkfile_path)Tracer\
 -I$(mkfile_path)Calibration\
　-l$(mkfile_path)Motor\

#COPTS += -fno-use-cxa-atexit
