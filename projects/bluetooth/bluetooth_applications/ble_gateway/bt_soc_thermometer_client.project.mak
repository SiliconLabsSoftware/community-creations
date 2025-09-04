####################################################################
# Automatically-generated file. Do not edit!                       #
# Makefile Version 16                                              #
####################################################################

BASE_SDK_PATH = C:/Users/Admin/SimplicityStudio/SDKs/simplicity_sdk
BASE_PKG_PATH = C:/Users/Admin/.silabs/slt/installs
UNAME:=$(shell $(POSIX_TOOL_PATH)uname -s | $(POSIX_TOOL_PATH)sed -e 's/^\(CYGWIN\).*/\1/' | $(POSIX_TOOL_PATH)sed -e 's/^\(MINGW\).*/\1/')
ifeq ($(UNAME),MINGW)
# Translate "C:/super" into "/C/super" for MinGW make.
SDK_PATH := /$(shell $(POSIX_TOOL_PATH)echo $(BASE_SDK_PATH) | sed s/://)
PKG_PATH := /$(shell $(POSIX_TOOL_PATH)echo $(BASE_PKG_PATH) | sed s/://)
endif
SDK_PATH ?= $(BASE_SDK_PATH)
PKG_PATH ?= $(BASE_PKG_PATH)
COPIED_SDK_PATH ?= simplicity_sdk_2025.6.1

# This uses the explicit build rules below
PROJECT_SOURCE_FILES =

C_SOURCE_FILES   += $(filter %.c, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cpp, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cc, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.s, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.S, $(PROJECT_SOURCE_FILES))
LIB_FILES        += $(filter %.a, $(PROJECT_SOURCE_FILES))

C_DEFS += \

ASM_DEFS += \

INCLUDES += \
 -Iautogen

GROUP_START =-Wl,--start-group
GROUP_END =-Wl,--end-group

PROJECT_LIBS = \
 -lgcc \
 -lc \
 -lm \
 -lnosys

LIBS += $(GROUP_START) $(PROJECT_LIBS) $(GROUP_END)

LIB_FILES += $(filter %.a, $(PROJECT_LIBS))

C_FLAGS += \
 -std=c18 \
 -Wall \
 -Wextra \
 -Os \
 -g \
 -fno-lto

CXX_FLAGS += \
 -std=c++17 \
 -Wall \
 -Wextra \
 -Os \
 -g \
 -fno-lto

ASM_FLAGS += \

LD_FLAGS += \
 -Xlinker -Map=$(OUTPUT_DIR)/$(PROJECTNAME).map \
 -fno-lto


####################################################################
# Pre/Post Build Rules                                             #
####################################################################
pre-build:
	# No pre-build defined

post-build: $(OUTPUT_DIR)/$(PROJECTNAME).out
	# No post-build defined

####################################################################
# SDK Build Rules                                                  #
####################################################################
# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA