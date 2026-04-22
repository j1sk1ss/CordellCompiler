CC ?= gcc

CFLAGS = -Wall -Wno-int-conversion -g -Iinclude
LDFLAGS =

ERROR_LOGS ?= 0
WARN_LOGS ?= 0
INFO_LOGS ?= 0
DEBUG_LOGS ?= 0
IO_LOGS ?= 0
MEM_LOGS ?= 0
LOGGING_LOGS ?= 0
SPECIAL_LOGS ?= 0

AVAILABLE_MEMORY ?= 300000
CFLAGS += -DALLOC_BUFFER_SIZE=$(AVAILABLE_MEMORY)

ifeq ($(ERROR_LOGS),1)
	CFLAGS += -DERROR_LOGS
endif

ifeq ($(WARN_LOGS),1)
	CFLAGS += -DWARNING_LOGS
endif

ifeq ($(INFO_LOGS),1)
	CFLAGS += -DINFO_LOGS
endif

ifeq ($(DEBUG_LOGS),1)
	CFLAGS += -DDEBUG_LOGS
endif

ifeq ($(IO_LOGS),1)
	CFLAGS += -DIO_OPERATION_LOGS
endif

ifeq ($(MEM_LOGS),1)
	CFLAGS += -DMEM_OPERATION_LOGS
endif

ifeq ($(LOGGING_LOGS),1)
	CFLAGS += -DLOGGING_LOGS
endif

ifeq ($(SPECIAL_LOGS),1)
	CFLAGS += -DSPECIAL_LOGS
endif

SOURCES := $(sort $(shell find src std -type f -name '*.c'))
OUTPUT = builds/ccompiler

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DPRINT_PARSE $(SOURCES) -o $@ $(LDFLAGS)

clean:
	rm -rf builds

print-sources:
	@printf "%s\n" $(SOURCES)

.PHONY: all clean print-sources