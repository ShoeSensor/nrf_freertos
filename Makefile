PROJECT_NAME := $(shell basename $(CURDIR))

export OUTPUT_FILENAME
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME))
SDK_ROOT := $(shell echo $$SDK_ROOT)
PROJ_HOME := $(SDK_ROOT)/apps/$(PROJECT_NAME)

TEMPLATE_PATH = $(SDK_ROOT)/components/toolchain/gcc
ifeq ($(OS),Windows_NT)
include $(TEMPLATE_PATH)/Makefile.windows
else
include $(TEMPLATE_PATH)/Makefile.posix
endif

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

# Toolchain commands
CC              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-gcc'
AS              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-as'
AR              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ar' -r
LD              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-ld'
NM              := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-nm'
OBJDUMP         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objdump'
OBJCOPY         := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-objcopy'
SIZE            := '$(GNU_INSTALL_ROOT)/bin/$(GNU_PREFIX)-size'

#function for removing duplicates in a list
remduplicates = $(strip $(if $1,$(firstword $1) $(call remduplicates,$(filter-out $(firstword $1),$1))))

#Test support

ifeq ("$(TEST)","1")
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/tests/threadtest.c)
else
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/main.c)
endif

#Project specific files
INC_PATHS  += -I$(abspath $(PROJ_HOME)/config)
INC_PATHS  += -I$(abspath $(PROJ_HOME)/include)
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/src/os_mutex.c)
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/src/os_thread.c)
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/src/os_semaphore.c)
C_SOURCE_FILES += $(abspath $(PROJ_HOME)/src/os_timer.c)


#source common to all targets
C_SOURCE_FILES += \
$(abspath $(SDK_ROOT)/components/libraries/util/app_error.c) \
$(abspath $(SDK_ROOT)/components/libraries/util/app_util_platform.c) \
$(abspath $(SDK_ROOT)/components/libraries/util/nrf_assert.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/croutine.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/event_groups.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/portable/MemMang/heap_1.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/list.c) \
$(abspath $(SDK_ROOT)/external/freertos/portable/GCC/nrf52/port.c) \
$(abspath $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52/port_cmsis.c) \
$(abspath $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52/port_cmsis_systick.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/queue.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/tasks.c) \
$(abspath $(SDK_ROOT)/external/freertos/source/timers.c) \
$(abspath $(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c) \
$(abspath $(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c) \
$(abspath $(SDK_ROOT)/components/toolchain/system_nrf52.c) \

#assembly files common to all targets
ASM_SOURCE_FILES  = $(abspath $(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf52.s)

#includes common to all targets
INC_PATHS += -I$(abspath $(SDK_ROOT)/examples/bsp)
INC_PATHS += -I$(abspath $(SDK_ROOT)/external/freertos/config)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/device)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/hal)
INC_PATHS += -I$(abspath $(SDK_ROOT)/external/freertos/portable/GCC/nrf52)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/libraries/util)
INC_PATHS += -I$(abspath $(SDK_ROOT)/external/freertos/portable/CMSIS/nrf52)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/common)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/toolchain)
INC_PATHS += -I$(abspath $(SDK_ROOT)/external/freertos/source/include)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/config)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/toolchain/gcc)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/drivers_nrf/clock)
INC_PATHS += -I$(abspath $(SDK_ROOT)/components/toolchain/CMSIS/Include)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

ifeq ("$(DEBUG)","1")
    CFLAGS = -DDEBUG -g3 -O0
else
    CFLAGS = -DNDEBUG -Werror -Os
endif

#flags common to all targets
CFLAGS  = -DNRF52_PAN_12
CFLAGS += -DNRF52_PAN_15
CFLAGS += -DNRF52_PAN_58
CFLAGS += -DFREERTOS
CFLAGS += -DNRF52_PAN_20
CFLAGS += -DNRF52_PAN_54
CFLAGS += -DNRF52_PAN_31
CFLAGS += -DNRF52_PAN_30
CFLAGS += -DNRF52_PAN_51
CFLAGS += -DNRF52_PAN_36
CFLAGS += -DNRF52_PAN_53
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF52_PAN_64
CFLAGS += -DNRF52_PAN_55
CFLAGS += -DNRF52_PAN_62
CFLAGS += -DNRF52_PAN_63
CFLAGS += -DBOARD_PCA10040
CFLAGS += -DNRF52
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs --std=gnu99
CFLAGS += -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in separate section. This will allow linker to dump unused functions
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums
# keep every function in separate section. This will allow linker to dump unused functions
LDFLAGS += -Xlinker -Map=$(LISTING_DIRECTORY)/$(OUTPUT_FILENAME).map
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

# Assembler flags
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DNRF52_PAN_12
ASMFLAGS += -DNRF52_PAN_15
ASMFLAGS += -DNRF52_PAN_58
ASMFLAGS += -DFREERTOS
ASMFLAGS += -DNRF52_PAN_20
ASMFLAGS += -DNRF52_PAN_54
ASMFLAGS += -DNRF52_PAN_31
ASMFLAGS += -DNRF52_PAN_30
ASMFLAGS += -DNRF52_PAN_51
ASMFLAGS += -DNRF52_PAN_36
ASMFLAGS += -DNRF52_PAN_53
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF52_PAN_64
ASMFLAGS += -DNRF52_PAN_55
ASMFLAGS += -DNRF52_PAN_62
ASMFLAGS += -DNRF52_PAN_63
ASMFLAGS += -DBOARD_PCA10040
ASMFLAGS += -DNRF52
ASMFLAGS += -DBSP_DEFINES_ONLY
#default target - first one defined
default: clean nrf52422_xxac

#building all targets

all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e nrf52422_xxac

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	nrf52422_xxac


C_SOURCE_FILE_NAMES = $(notdir $(C_SOURCE_FILES))
C_PATHS = $(call remduplicates, $(dir $(C_SOURCE_FILES) ) )
C_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(C_SOURCE_FILE_NAMES:.c=.o) )

ASM_SOURCE_FILE_NAMES = $(notdir $(ASM_SOURCE_FILES))
ASM_PATHS = $(call remduplicates, $(dir $(ASM_SOURCE_FILES) ))
ASM_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(ASM_SOURCE_FILE_NAMES:.s=.o) )

vpath %.c $(C_PATHS)
vpath %.s $(ASM_PATHS)

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

nrf52422_xxac: OUTPUT_FILENAME := nrf52422_xxac
nrf52422_xxac: LINKER_SCRIPT=config/blinky_FreeRTOS_gcc_nrf52.ld
nrf52422_xxac: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e finalize

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from C SRC files
$(OBJECT_DIRECTORY)/%.o: %.c
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(CFLAGS) $(INC_PATHS) -c -o $@ $<

# Assemble files
$(OBJECT_DIRECTORY)/%.o: %.s
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CC) $(ASMFLAGS) $(INC_PATHS) -c -o $@ $<


# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).out
	$(NO_ECHO)$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out


## Create binary .bin file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex: $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

finalize: genbin genhex echosize

genbin:
	@echo Preparing: $(OUTPUT_FILENAME).bin
	$(NO_ECHO)$(OBJCOPY) -O binary $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).bin

## Create binary .hex file from the .out file
genhex:
	@echo Preparing: $(OUTPUT_FILENAME).hex
	$(NO_ECHO)$(OBJCOPY) -O ihex $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).hex

echosize:
	-@echo ''
	$(NO_ECHO)$(SIZE) $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).out
	-@echo ''

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o

flash: $(MAKECMDGOALS)
	@echo Flashing: $(OUTPUT_BINARY_DIRECTORY)/$<.hex
	nrfjprog --program $(OUTPUT_BINARY_DIRECTORY)/$<.hex -f nrf52  --chiperase
	nrfjprog --reset

## Flash softdevice
flash_softdevice:
	@echo Flashing: nrf52_8.0.0_softdevice.hex
	nrfjprog --program $(SDK_ROOT)/components/softdevice/s110/hex/nrf52_8.0.0_softdevice.hex -f nrf52 --chiperase
	nrfjprog --reset
