CC 						?= gcc
AR 						?= ar
PYTHON 					?= python3
RM 						?= rm -f
MKDIR_P 				?= mkdir -p
INSTALL 				?= install

PREFIX 					?= /usr/local
DESTDIR 				?=
BINDIR 					?= $(PREFIX)/bin
LIBDIR 					?= $(PREFIX)/lib
DATADIR 				?= $(PREFIX)/share
CPLLIBDIR 				?= $(DATADIR)/cpl/include
CPLRUNTIMEDIR 			?= $(LIBDIR)/cpl
DOCDIR 					?= $(DATADIR)/doc/cpl
VERSION 				?= 3.6_X

BUILD 					?= debug
AVAILABLE_MEMORY 		?= 67108864
LOGS 					?=
PRINT_PARSE 			?= 1
ENABLE_Z3 				?= auto
INPUT 					?= examples/print.cpl
UTEST 					?= code_utesting
STD_UTEST 				?= std_utesting
VSCODE_DOCKER_IMAGE 	?= cpl-extension
VSCODE_OUTPUT_DIR 		?= $(CURDIR)/vscode/output
DOCS_BACKEND_BUILD_DIR 	?= docs/back/.build
DOCS_BACKEND_PLATFORM 	?= ../$(DOCS_BACKEND_BUILD_DIR)
DOCS_BACKEND_COMPILER 	?= $(DOCS_BACKEND_BUILD_DIR)/cplc
DOCS_BACKEND_OUTPUT 	?= docs/back/cpl_docs_backend

UNAME_S ?= $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	RUN_ARGS ?= 				\
		--arch x86_64 			\
		--sys-type macho64 		\
		--asm-format macho64 	\
		--linker clang
else
	RUN_ARGS ?= 				\
		--arch x86_64 			\
		--sys-type linux64 		\
		--asm-format elf64 		\
		--linker gcc 			\
		--linker-no-pie 
endif

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

SOURCES 		:= $(sort $(shell find src std -type f -name '*.c'))
OUTPUT 			= builds/$(PLATFORM)/cplc
CPLLIB_IMPLS 	:= $(sort $(shell find cpllib -type f -name '*.cpl' ! -name '*_h.cpl'))
CPLLIB_BUILDDIR := builds/$(PLATFORM)/cpllib
CPLLIB_OBJDIR   := $(CPLLIB_BUILDDIR)/obj
CPLLIB_OBJS     := $(patsubst cpllib/%.cpl,$(CPLLIB_OBJDIR)/%.o,$(CPLLIB_IMPLS))
CPLLIB_ARCHIVE  := $(CPLLIB_BUILDDIR)/libcpl.a

CPPFLAGS 		+= -Iinclude -DALLOC_BUFFER_SIZE=$(AVAILABLE_MEMORY) -DCPL_DEFAULT_INCLUDE_DIR=\"$(CPLLIBDIR)\" -DCPL_DEFAULT_RUNTIME_LIB=\"$(CPLRUNTIMEDIR)/libcpl.a\"
CFLAGS   		+= -Wall -Wno-int-conversion
LDFLAGS  		+=
LDLIBS   		+=

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

$(CPLLIB_OBJDIR)/%.o: cpllib/%.cpl $(OUTPUT)
	@$(MKDIR_P) $(dir $@)
	$(OUTPUT) $(RUN_ARGS) -c --output $@ $<

$(CPLLIB_ARCHIVE): $(CPLLIB_OBJS)
	@$(MKDIR_P) $(dir $@)
	$(RM) $@
	$(AR) rcs $@ $^

cpllib: $(CPLLIB_ARCHIVE) ## Build the CPL runtime static library.

docs-backend: ## Build the CPL HTTP backend for the docs Playground.
	$(MAKE) PLATFORM=$(DOCS_BACKEND_PLATFORM) BUILD=$(BUILD) PRINT_PARSE=$(PRINT_PARSE) ENABLE_Z3=$(ENABLE_Z3) all cpllib
	$(DOCS_BACKEND_COMPILER) $(RUN_ARGS) docs/back/main.cpl --output $(DOCS_BACKEND_OUTPUT)

docs-backend-run: docs-backend ## Build and run the CPL docs backend on 127.0.0.1:8000.
	./$(DOCS_BACKEND_OUTPUT)

debug: ## Build a debug compiler.
	$(MAKE) BUILD=debug all

release: ## Build an optimized compiler.
	$(MAKE) BUILD=release PRINT_PARSE=0 all

install: $(OUTPUT) $(CPLLIB_ARCHIVE) ## Install the compiler and CPL standard library under PREFIX.
	$(INSTALL) -d $(DESTDIR)$(BINDIR) $(DESTDIR)$(CPLLIBDIR) $(DESTDIR)$(CPLRUNTIMEDIR) $(DESTDIR)$(DOCDIR)
	$(INSTALL) -m 0755 $(OUTPUT) $(DESTDIR)$(BINDIR)/cplc
	$(INSTALL) -m 0644 cpllib/*.cpl $(DESTDIR)$(CPLLIBDIR)/
	$(INSTALL) -m 0644 $(CPLLIB_ARCHIVE) $(DESTDIR)$(CPLRUNTIMEDIR)/libcpl.a
	$(INSTALL) -m 0644 LICENSE $(DESTDIR)$(DOCDIR)/

package: ## Build a relocatable binary tarball with the standard library.
	$(MAKE) BUILD=release PRINT_PARSE=0 -B all cpllib
	$(RM) -r builds/package/cpl-$(VERSION)
	$(INSTALL) -d builds/package/cpl-$(VERSION)/bin builds/package/cpl-$(VERSION)/lib/cpl builds/package/cpl-$(VERSION)/share/cpl/include builds/package/cpl-$(VERSION)/share/doc/cpl
	$(INSTALL) -m 0755 $(OUTPUT) builds/package/cpl-$(VERSION)/bin/cplc
	$(INSTALL) -m 0644 cpllib/*.cpl builds/package/cpl-$(VERSION)/share/cpl/include/
	$(INSTALL) -m 0644 $(CPLLIB_ARCHIVE) builds/package/cpl-$(VERSION)/lib/cpl/libcpl.a
	$(INSTALL) -m 0644 LICENSE builds/package/cpl-$(VERSION)/share/doc/cpl/
	tar -C builds/package -czf builds/cpl-$(VERSION)-$(PLATFORM).tar.gz cpl-$(VERSION)

run: $(OUTPUT) ## Compile INPUT with the built compiler.
	$(OUTPUT) $(RUN_ARGS) $(INPUT)

test: unit-test ## Run module tests.

unit-test: ## Run module tests, e.g. make unit-test UTEST=code_utesting/ast.
	cd tests && $(PYTHON) module_testing.py --path $(UTEST) --compiler $(CC) --output-dir bin --base ../

rewrite-test: ## Rewrite OUTPUT blocks for module tests.
	cd tests && $(PYTHON) module_testing.py --path $(UTEST) --compiler $(CC) --output-dir bin --base ../ --force-rewrite

std-test: ## Run std library tests, e.g. make std-test or make std-test STD_UTEST=std_utesting/list.
	@if [ "$(STD_UTEST)" = "std_utesting" ]; then \
		for dir in tests/std_utesting/*; do \
			if [ -d "$$dir" ]; then \
				name=$$(basename "$$dir"); \
				cd tests && $(PYTHON) std_testing.py --path "std_utesting/$$name" --compiler $(CC) --output-dir bin --base ../ || exit 1; \
				cd ..; \
			fi; \
		done; \
	else \
		cd tests && $(PYTHON) std_testing.py --path $(STD_UTEST) --compiler $(CC) --output-dir bin --base ../; \
	fi

vscode-docker-build: ## Build the VS Code extension Docker image.
	docker build -t $(VSCODE_DOCKER_IMAGE) vscode

vscode-docker-package: vscode-docker-build ## Build and package the VS Code extension in Docker.
	docker run --rm -v $(CURDIR)/vscode:/app -v $(VSCODE_OUTPUT_DIR):/output $(VSCODE_DOCKER_IMAGE)

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
	@echo "LIBDIR=$(LIBDIR)"
	@echo "CPLLIBDIR=$(CPLLIBDIR)"
	@echo "CPLRUNTIMEDIR=$(CPLRUNTIMEDIR)"
	@echo "CPLLIB_IMPLS=$(CPLLIB_IMPLS)"
	@echo "CPLLIB_ARCHIVE=$(CPLLIB_ARCHIVE)"
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
	@echo "VSCODE_DOCKER_IMAGE=$(VSCODE_DOCKER_IMAGE)"
	@echo "VSCODE_OUTPUT_DIR=$(VSCODE_OUTPUT_DIR)"

help:
	@awk 'BEGIN {FS = ":.*## "; printf "Usage: make <target> [VAR=value]\n\nTargets:\n"} /^[a-zA-Z0-9_.-]+:.*## / {printf "  %-14s %s\n", $$1, $$2}' $(MAKEFILE_LIST)

.DELETE_ON_ERROR:
.PHONY: all cpllib debug release install package run test unit-test rewrite-test std-test vscode-docker-build vscode-docker-package clean clean-tests distclean print-sources print-config help
