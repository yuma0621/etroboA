mkfile_path := $(dir $(lastword $(MAKEFILE_LIST)))

APPL_COBJS +=\
LineTracer.o\
Calibration.o\

APPL_LIBS += -lm

APPL_DIRS += $(mkfile_path)LineTracer
APPL_DIRS += $(mkfile_path)Calibration

INCLUDES += \
 -I$(mkfile_path)LineTracer\
 -I$(mkfile_path)Calibration\
