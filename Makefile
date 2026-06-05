CC ?= gcc

BUILD ?= debug
AVAILABLE_MEMORY ?= 16777216
LOGS ?=
PRINT_PARSE ?= 1

PLATFORM ?= $(shell uname -s | tr '[:upper:]' '[:lower:]')-$(shell uname -m | tr '[:upper:]' '[:lower:]')

SOURCES := $(sort $(shell find src std -type f -name '*.c'))
OUTPUT = builds/$(PLATFORM)/cplc

CPPFLAGS = -Iinclude -DALLOC_BUFFER_SIZE=$(AVAILABLE_MEMORY)
CFLAGS = -Wall -Wno-int-conversion
LDFLAGS =
LDLIBS =

ifeq ($(BUILD),debug)
	CFLAGS += -g -O0
else ifeq ($(BUILD),release)
	CFLAGS += -O2
else
	$(error Unknown BUILD=$(BUILD), use debug or release)
endif

ifeq ($(PRINT_PARSE),1)
	CPPFLAGS += -DPRINT_PARSE
endif

ifneq ($(filter error,$(LOGS)),)
	CPPFLAGS += -DERROR_LOGS
endif

ifneq ($(filter warn,$(LOGS)),)
	CPPFLAGS += -DWARNING_LOGS
endif

ifneq ($(filter info,$(LOGS)),)
	CPPFLAGS += -DINFO_LOGS
endif

ifneq ($(filter debug,$(LOGS)),)
	CPPFLAGS += -DDEBUG_LOGS
endif

ifneq ($(filter io,$(LOGS)),)
	CPPFLAGS += -DIO_OPERATION_LOGS
endif

ifneq ($(filter mem,$(LOGS)),)
	CPPFLAGS += -DMEM_OPERATION_LOGS
endif

ifneq ($(filter logging,$(LOGS)),)
	CPPFLAGS += -DLOGGING_LOGS
endif

ifneq ($(filter special,$(LOGS)),)
	CPPFLAGS += -DSPECIAL_LOGS
endif

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -rf builds

print-sources:
	@printf "%s\n" $(SOURCES)

print-config:
	@echo "CC=$(CC)"
	@echo "BUILD=$(BUILD)"
	@echo "PLATFORM=$(PLATFORM)"
	@echo "OUTPUT=$(OUTPUT)"
	@echo "CPPFLAGS=$(CPPFLAGS)"
	@echo "CFLAGS=$(CFLAGS)"
	@echo "LDFLAGS=$(LDFLAGS)"
	@echo "LDLIBS=$(LDLIBS)"
	@echo "LOGS=$(LOGS)"

.PHONY: all clean print-sources print-config