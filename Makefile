CC ?= gcc

CFLAGS = -Wall -Wno-int-conversion -g -Iinclude

ERROR_LOGS ?= 1
WARN_LOGS ?= 1
INFO_LOGS ?= 1
DEBUG_LOGS ?= 1
IO_LOGS ?= 1
MEM_LOGS ?= 0
LOGGING_LOGS ?= 1
SPECIAL_LOGS ?= 1

AVALIABLE_MEMORY ?= 300000
CFLAGS += -DALLOC_BUFFER_SIZE=$(AVALIABLE_MEMORY)

ifeq ($(ERROR_LOGS), 1)
	CFLAGS += -DERROR_LOGS
endif

ifeq ($(WARN_LOGS), 1)
	CFLAGS += -DWARNING_LOGS
endif

ifeq ($(INFO_LOGS), 1)
	CFLAGS += -DINFO_LOGS
endif

ifeq ($(DEBUG_LOGS), 1)
	CFLAGS += -DDEBUG_LOGS
endif

ifeq ($(IO_LOGS), 1)
	CFLAGS += -DIO_OPERATION_LOGS
endif

ifeq ($(MEM_LOGS), 1)
	CFLAGS += -DMEM_OPERATION_LOGS
endif

ifeq ($(LOGGING_LOGS), 1)
	CFLAGS += -DLOGGING_LOGS
endif

ifeq ($(SPECIAL_LOGS), 1)
	CFLAGS += -DSPECIAL_LOGS
endif

SOURCES := $(shell find src std -type f -name '*.c')

OUTPUT = builds/ccompiler_all.o

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -DPRINT_PARSE -r $(SOURCES) -o $@

clean:
	rm -rf builds

print-sources:
	@printf "%s\n" $(SOURCES)

.PHONY: all clean print-sources