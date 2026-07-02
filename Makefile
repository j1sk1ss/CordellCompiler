CC ?= gcc
PYTHON ?= python3
RM ?= rm -f
MKDIR_P ?= mkdir -p
INSTALL ?= install

PREFIX ?= /usr/local
DESTDIR ?=
BINDIR ?= $(PREFIX)/bin
DATADIR ?= $(PREFIX)/share
CPLLIBDIR ?= $(DATADIR)/cpl/include
DOCDIR ?= $(DATADIR)/doc/cpl
VERSION ?= 3.6_X

BUILD ?= debug
AVAILABLE_MEMORY ?= 16777216
LOGS ?=
PRINT_PARSE ?= 1
ENABLE_Z3 ?= auto
INPUT ?= examples/print.cpl
RUN_ARGS ?= --arch x86_64 --sys-type linux64 --asm-format elf64 --linker gcc --linker-no-pie
MODULE ?= asm
TEST_CODE ?= dummy_data/simple.cpl
UTEST ?= code_utesting
STD_UTEST ?= std_utesting

Z3_AVAILABLE := $(shell pkg-config --exists z3 2>/dev/null && echo 1 || echo 0)
ifeq ($(ENABLE_Z3),auto)
	Z3_ENABLED := $(Z3_AVAILABLE)
else
	Z3_ENABLED := $(ENABLE_Z3)
endif

ifeq ($(Z3_ENABLED),1)
	Z3_CFLAGS ?= $(shell pkg-config --cflags z3 2>/dev/null)
	Z3_LDLIBS ?= $(shell pkg-config --libs z3 2>/dev/null)
	ifeq ($(strip $(Z3_LDLIBS)),)
		Z3_LDLIBS = -lz3
	endif
else
	Z3_CFLAGS :=
	Z3_LDLIBS :=
endif

PLATFORM ?= $(shell uname -s | tr '[:upper:]' '[:lower:]')-$(shell uname -m | tr '[:upper:]' '[:lower:]')

SOURCES := $(sort $(shell find src std -type f -name '*.c'))
OUTPUT = builds/$(PLATFORM)/cplc

CPPFLAGS += -Iinclude -DALLOC_BUFFER_SIZE=$(AVAILABLE_MEMORY) -DCPL_DEFAULT_INCLUDE_DIR=\"$(CPLLIBDIR)\"
CFLAGS += -Wall -Wno-int-conversion
LDFLAGS +=
LDLIBS +=

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

ifeq ($(Z3_ENABLED),1)
	CPPFLAGS += -DCPL_ENABLE_Z3 $(Z3_CFLAGS)
	LDLIBS += $(Z3_LDLIBS)
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

all: $(OUTPUT) ## Build the compiler with the current configuration.

$(OUTPUT): $(SOURCES)
	@$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $@ $(LDFLAGS) $(LDLIBS)

debug: ## Build a debug compiler.
	$(MAKE) BUILD=debug all

release: ## Build an optimized compiler.
	$(MAKE) BUILD=release PRINT_PARSE=0 all

install: $(OUTPUT) ## Install the compiler and CPL standard library under PREFIX.
	$(INSTALL) -d $(DESTDIR)$(BINDIR) $(DESTDIR)$(CPLLIBDIR) $(DESTDIR)$(DOCDIR)
	$(INSTALL) -m 0755 $(OUTPUT) $(DESTDIR)$(BINDIR)/cplc
	$(INSTALL) -m 0644 cpllib/*.cpl $(DESTDIR)$(CPLLIBDIR)/
	$(INSTALL) -m 0644 LICENSE $(DESTDIR)$(DOCDIR)/

package: ## Build a relocatable binary tarball with the standard library.
	$(MAKE) BUILD=release PRINT_PARSE=0 -B all
	$(RM) -r builds/package/cpl-$(VERSION)
	$(INSTALL) -d builds/package/cpl-$(VERSION)/bin builds/package/cpl-$(VERSION)/share/cpl/include builds/package/cpl-$(VERSION)/share/doc/cpl
	$(INSTALL) -m 0755 $(OUTPUT) builds/package/cpl-$(VERSION)/bin/cplc
	$(INSTALL) -m 0644 cpllib/*.cpl builds/package/cpl-$(VERSION)/share/cpl/include/
	$(INSTALL) -m 0644 LICENSE builds/package/cpl-$(VERSION)/share/doc/cpl/
	tar -C builds/package -czf builds/cpl-$(VERSION)-$(PLATFORM).tar.gz cpl-$(VERSION)

run: $(OUTPUT) ## Compile INPUT with the built compiler.
	$(OUTPUT) $(RUN_ARGS) $(INPUT)

test: ## Run integration tests, e.g. make test MODULE=asm.
	cd tests && $(PYTHON) integrated_testing.py --run --module $(MODULE) --test-code $(TEST_CODE) --compiler $(CC) --output-dir bin --base ../

unit-test: ## Run module tests, e.g. make unit-test UTEST=code_utesting/ast.
	cd tests && $(PYTHON) module_testing.py --path $(UTEST) --compiler $(CC) --output-dir bin --base ../

rewrite-test: ## Rewrite OUTPUT blocks for module tests.
	cd tests && $(PYTHON) module_testing.py --path $(UTEST) --compiler $(CC) --output-dir bin --base ../ --force-rewrite

std-test: ## Run std library tests, e.g. make std-test STD_UTEST=std_utesting/list.
	cd tests && $(PYTHON) std_testing.py --path $(STD_UTEST) --compiler $(CC) --output-dir bin --base ../

cpllib-test: $(OUTPUT) ## Parse all shipped CPL standard library headers.
	$(OUTPUT) --without-compilation tests/cpllib_smoke.cpl

clean: ## Remove compiler build outputs.
	$(RM) -r builds

clean-tests: ## Remove test binaries.
	$(RM) -r tests/bin

distclean: clean clean-tests ## Remove all generated build/test outputs.

print-sources:
	@printf "%s\n" $(SOURCES)

print-config:
	@echo "CC=$(CC)"
	@echo "BUILD=$(BUILD)"
	@echo "PLATFORM=$(PLATFORM)"
	@echo "OUTPUT=$(OUTPUT)"
	@echo "PREFIX=$(PREFIX)"
	@echo "CPLLIBDIR=$(CPLLIBDIR)"
	@echo "CPPFLAGS=$(CPPFLAGS)"
	@echo "CFLAGS=$(CFLAGS)"
	@echo "LDFLAGS=$(LDFLAGS)"
	@echo "LDLIBS=$(LDLIBS)"
	@echo "ENABLE_Z3=$(ENABLE_Z3)"
	@echo "Z3_AVAILABLE=$(Z3_AVAILABLE)"
	@echo "Z3_ENABLED=$(Z3_ENABLED)"
	@echo "Z3_CFLAGS=$(Z3_CFLAGS)"
	@echo "Z3_LDLIBS=$(Z3_LDLIBS)"
	@echo "LOGS=$(LOGS)"
	@echo "INPUT=$(INPUT)"
	@echo "RUN_ARGS=$(RUN_ARGS)"

help:
	@awk 'BEGIN {FS = ":.*## "; printf "Usage: make <target> [VAR=value]\n\nTargets:\n"} /^[a-zA-Z0-9_.-]+:.*## / {printf "  %-14s %s\n", $$1, $$2}' $(MAKEFILE_LIST)

.DELETE_ON_ERROR:
.PHONY: all debug release install package run test unit-test rewrite-test std-test cpllib-test clean clean-tests distclean print-sources print-config help
