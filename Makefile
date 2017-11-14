##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

# Version
export MAJOR	= 0
export MINOR	= 1
export PATCH	= 0
export EXTRA	= -indev

export KERNEL = chaos_$(ARCH)-$(MAJOR).$(MINOR).$(PATCH)$(EXTRA).bin
export ISO = chaos_$(ARCH)-$(MAJOR).$(MINOR).$(PATCH)$(EXTRA).iso

# Verbosity
export Q =
export V ?= 0
ifeq ($(V), 0)
	MAKEFLAGS += --silent
	Q = @
endif

# Number of cores (QEMU)
export SMP ?= 1

# Arch
arch ?= x86
export ARCH ?= $(arch)

export PROJECT_DIR = $(shell pwd)
export INCLUDE_DIR = $(PROJECT_DIR)/include

# Flags
export CFLAGS += \
	-MD \
	-nostdlib \
	-nostdinc \
	-W \
	-Wall \
	-Wextra \
	-O2 \
	--std=gnu11 \
	-fno-builtin \
	-fno-stack-protector \
	-fno-omit-frame-pointer \
	-isystem "$(INCLUDE_DIR)" \
	-isystem "$(INCLUDE_DIR)/lib/libc/"

export LDFLAGS = -nostdlib

# Import arch flags
include arch/$(ARCH)/Makefile.cpu

# Do not print "Entering directory ..."
MAKEFLAGS += --no-print-directory

.PHONY: all
all: kernel

.PHONY: print_building
print_building:
	$(Q)echo
	$(Q)echo " *"
	$(Q)echo "*  Building $(notdir $(KERNEL))"
	$(Q)echo " *"
	$(Q)echo
	$(Q)$(MAKE) -C arch all
	$(Q)$(MAKE) -C kernel all
	$(Q)$(MAKE) -C drivers all
	$(Q)$(MAKE) -C lib all
	$(Q)$(MAKE) $(KERNEL)

.PHONY: kernel
kernel: print_building

$(KERNEL): arch/arch.o drivers/drivers.o kernel/kernel.o lib/lib.o
	$(Q)printf "  LD\t $(notdir $@)\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -T $(LINKER_SCRIPT) $^

.PHONY: iso
iso: $(ISO)

$(ISO): kernel
	$(Q)./scripts/chaos-iso.sh -b "$(BOOT_FLAGS)"

.PHONY: config
config:
	$(Q)printf "  SHELL\t kconfig\n"
	$(Q)./scripts/kconfig/main.sh
	$(Q)printf "  SHELL\t gen-config.sh\n"
	$(Q)./scripts/gen-config.sh

.PHONY: run
run: iso
	$(Q)./scripts/qemu.sh -m 1G -a "$(ARCH)" -s "$(SMP)"

.PHONY: monitor
monitor: iso
	./scripts/qemu.sh -t -m 1G -a "$(ARCH)" -s "$(SMP)"

.PHONY: debug
debug: iso
	./scripts/qemu.sh -d -m 1G -a "$(ARCH)" -s "$(SMP)"

.PHONY: run
kvm: iso
	$(Q)./scripts/qemu.sh -d -k -m 1G -a "$(ARCH)" -s "$(SMP)"

.PHONY: clean
clean:
	$(Q)$(MAKE) -C arch clean
	$(Q)$(MAKE) -C kernel clean
	$(Q)$(MAKE) -C drivers clean
	$(Q)$(MAKE) -C lib clean
	$(Q)$(RM) "$(ISO)" "$(KERNEL)"

.PHONY: re
re: clean all
