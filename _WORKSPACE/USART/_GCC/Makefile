#PROJECT_NAME     := GPIO
TARGETS          := stm32f0xx
OUTPUT_DIRECTORY := _build

SDK_ROOT := ../../..
PROJ_DIR := ..

$(OUTPUT_DIRECTORY)/$(TARGETS).out: \
  LINKER_SCRIPT    := $(TARGETS).ld

# Source files common to all targets
######
# src
SRC_FILES += \
  $(PROJ_DIR)/src/main.c \
  $(PROJ_DIR)/src/stm32f0xx_it.c \
  $(PROJ_DIR)/src/system_stm32f0xx.c \

# STM32F0-Discovery
SRC_FILES += \
  $(SDK_ROOT)/Utilities/STM32F0-Discovery/stm32f0_discovery.c \

# STM32F0-StdPeriph_Driver
SRC_FILES += \
  $(SDK_ROOT)/Libraries/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_rcc.c \
  $(SDK_ROOT)/Libraries/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_misc.c \
  $(SDK_ROOT)/Libraries/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_gpio.c \
  $(SDK_ROOT)/Libraries/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_syscfg.c \
  $(SDK_ROOT)/Libraries/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_exti.c \

#DEBUG_
SRC_FILES += \
  $(SDK_ROOT)/Libraries/debug/error_check.c \
  $(SDK_ROOT)/Libraries/common/critical_state.c \

#SEGGER_
SRC_FILES += \
  $(SDK_ROOT)/Libraries/segger_rtt/rtt_log.c \
  $(SDK_ROOT)/Libraries/segger_rtt/SEGGER_RTT.c \
  $(SDK_ROOT)/Libraries/segger_rtt/SEGGER_RTT_printf.c \

#Startup
SRC_FILES += \
  $(SDK_ROOT)/components/startup/startup_stm32f0xx_gcc.s \

#My_Driver
SRC_FILES += \
  $(SDK_ROOT)/Libraries/STM32F0xx_MyLibrary/src/my_gpio_drv.c \
  $(SDK_ROOT)/Libraries/STM32F0xx_MyLibrary/src/my_usart_drv.c \


# Include folders common to all targets
INC_FOLDERS += \
  $(SDK_ROOT)/components/ST/STM32F0xx/include \
  $(SDK_ROOT)/components/toolchain/cmsis/include \
  $(SDK_ROOT)/Libraries/STM32F0xx_StdPeriph_Driver/inc \
  $(SDK_ROOT)/Libraries/STM32F0xx_MyLibrary/inc \
  $(SDK_ROOT)/Libraries/common \
  $(SDK_ROOT)/Libraries/debug \
  $(SDK_ROOT)/Libraries/segger_rtt \
  $(SDK_ROOT)/Utilities/STM32F0-Discovery \
  $(PROJ_DIR)/inc \

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -DSTM32F0XX
CFLAGS += -DDEBUG_ENABLED -DASSERT_ENABLED -DRTT_LOG_ENABLED
# CORTEX-M0
CFLAGS += -mcpu=cortex-m0
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall
CFLAGS += -mfloat-abi=soft
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums

# C++ flags common to all targets
CXXFLAGS += $(OPT)

# Assembler flags common to all targets
ASMFLAGS += -DUSE_STDPERIPH_DRIVER
ASMFLAGS += -DSTM32F0XX
ASMFLAGS += -DDEBUG_ENABLED -DASSERT_ENABLED -DRTT_LOG_ENABLED
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m0
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=soft

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -mfloat-abi=soft
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

$(TARGETS): CFLAGS += -D__HEAP_SIZE=1024
$(TARGETS): CFLAGS += -D__STACK_SIZE=1024
$(TARGETS): ASMFLAGS += -D__HEAP_SIZE=1024
$(TARGETS): ASMFLAGS += -D__STACK_SIZE=1024

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: $(TARGETS)

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		$(TARGETS)
	@echo		flash      - flashing binary to device
	@echo		erase      - erase binary from device
	@echo		clean_build      - clean build folder

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: flash erase

# Flash the program
flash: default
	@echo Flashing: $(OUTPUT_DIRECTORY)/$(TARGETS).hex
	/Applications/SEGGER/JLink_V640/jLinkExe -if SWD -device STM32F051R8 -speed 4000 -commanderscript flash.jlink
#	st-flash write $(OUTPUT_DIRECTORY)/$(TARGETS).bin 0x08000000

erase:
	/Applications/SEGGER/JLink_V640/jLinkExe -if SWD -device STM32F051R8 -speed 4000 -commanderscript erase.jlink

debug:
	telnet localhost 19021
	
# Clean _build folder
clean_build:
	rm -rf $(BUILD_DIR)_build
