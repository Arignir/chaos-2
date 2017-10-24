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
	-isystem "$(INCLUDE_DIR)"

export LDFLAGS =

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

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C arch all
	$(Q)$(MAKE) -C kernel all

.PHONY: kernel
kernel: print_building check_up $(KERNEL)

.PHONY: iso
iso: print_building check_up $(ISO)

$(KERNEL): arch/arch.a kernel/kernel.a
	$(Q)printf "  LD\t $(notdir $(KERNEL))\n"
	$(Q)$(LD) $(LDFLAGS) -o "$(KERNEL)" arch/arch.a kernel/kernel.a

$(ISO): $(KERNEL)
	$(Q)./scripts/chaos-iso.sh -b "$(BOOT_FLAGS)"

.PHONY: config
config:
	$(Q)printf "  SHELL\t kconfig\n"
	$(Q)./scripts/kconfig/main.sh
	$(Q)printf "  SHELL\t gen-config.sh\n"
	$(Q)./scripts/gen-config.sh

.PHONY: run
run: iso
	$(Q)./scripts/qemu.sh -m 1G -a "$(ARCH)"

.PHONY: monitor
monitor: iso
	./scripts/qemu.sh -t -m 1G -a "$(ARCH)"

.PHONY: run
kvm: iso
	$(Q)./scripts/qemu.sh -d -k -m 1G -a "$(ARCH)"

.PHONY: clean
clean:
	$(Q)$(MAKE) -C arch clean
	$(Q)$(MAKE) -C kernel clean
	$(Q)$(RM) chaos.iso "$(KERNEL)"

.PHONY: re
re: clean all
