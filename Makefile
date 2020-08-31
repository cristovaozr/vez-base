##
# @author Cristóvão Zuppardo Rufino <cristovaozr@gmail.com>
# @version 0.1
#
# @copyright Copyright Cristóvão Zuppardo Rufino (c) 2020
# Please see LICENCE file to information regarding licensing


# If you see an error on the next line it means that you defined no ARCH to build.
# Please refer to documentation to see which ARCH values are available
include arch/$(ARCH)/config.mk

TARGET = vez-base

# Debug build?
DEBUG = 1

# Build path
BUILD_DIR = /tmp/build

# Binaries
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
BIN = $(CP) -O binary -S

# Source files
# C sources
C_SOURCES += \
	core/src/main.c \
	core/src/freertos.c

# FreeRTOS sources
C_SOURCES += \
	freertos/croutine.c \
	freertos/event_groups.c \
	freertos/list.c \
	freertos/queue.c \
	freertos/stream_buffer.c \
	freertos/tasks.c \
	freertos/timers.c \
	arch/$(ARCH)/freertos/portable/MemMang/heap_4.c \
	arch/$(ARCH)/freertos/portable/GCC/ARM_CM3/port.c
# TODO: Change to the improved automatic HEAP user

# Devices
C_SOURCES += \
	core/src/device/device_init.c \
	core/src/device/gpio.c \
	core/src/device/usart.c \
	core/src/device/pwm.c

# ulibc
C_SOURCES += \
	ulibc/log.c \
	ulibc/ustdio.c

# Tasks
C_SOURCES += \
	tasks/shell/shell.c \
	tasks/blinky/blinky.c

# Fetching sources from ARCH related defines
C_SOURCES += $(ARCH_C_SOURCES)
ASM_SOURCES += $(ARCH_ASM_SOURCES)

# MCU
MCU = $(ARCH_MCU)

# AS defines
AS_DEFS = $(ARCH_AS_DEFS)

# AS includes
AS_INCLUDES += \
	-Icore/include
AS_INCLUDES += $(ARCH_AS_INCLUDES)

# AS flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) -Wall -fdata-sections -ffunction-sections

# C defines
C_DEFS += $(ARCH_C_DEFS)

# C includes
C_INCLUDES += \
	-I. \
	-Icore \
	-Icore/include \
	-Ifreertos/include
C_INCLUDES += $(ARCH_C_INCLUDES)

# C flags
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -Os
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Link script
LDSCRIPT = $(ARCH_LDSCRIPT)

# Libraries
LIBS = -lc -lm -lnosys
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) \
	-Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# Default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# Build the application
# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# List of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@
	
$(BUILD_DIR):
	mkdir $@

# Flashes built code to microcontroller
write:
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x08000000

clean:
	-rm -fR $(BUILD_DIR)
  
-include $(wildcard $(BUILD_DIR)/*.d)
