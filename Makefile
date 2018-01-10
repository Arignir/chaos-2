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
export INITRD = $(PWD)/initrd.img
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

# CPU to emulate (QEMU)
export CPU ?= qemu32

# Arch
arch ?= x86
export ARCH ?= $(arch)

export PROJECT_DIR = $(shell pwd)
export INCLUDE_DIR = $(PROJECT_DIR)/include

# Flags
export CFLAGS += \
	-W \
	-Wall \
	-Wextra \
	-Wstrict-prototypes \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-align \
	-Wwrite-strings \
	-Wmissing-prototypes \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wnested-externs \
	-Winline \
	-Wuninitialized \
	-Wno-missing-braces \
	-MD \
	-O2 \
	--std=gnu11 \
	-fno-builtin \
	-nostdinc \
	-nostdlib \
	-ffreestanding \
	-fms-extensions \
	-fno-stack-protector \
	-fno-omit-frame-pointer \
	-isystem "$(INCLUDE_DIR)" \
	-isystem "$(INCLUDE_DIR)/lib/libc/"

export NASM := nasm
export NASMFLAGS += -I "$(INCLUDE_DIR)/"

export LDFLAGS = -nostdlib
KERNEL_LDFLAGS = $(LDFLAGS) $(shell $(CC) -print-libgcc-file-name $(CFLAGS))

# Import arch flags
include arch/$(ARCH)/Makefile.cpu

# Do not print "Entering directory ..."
MAKEFLAGS += --no-print-directory

.PHONY: all
all: iso

.PHONY: print_building
print_building: kconfig
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
	$(Q)$(LD) -T $(LINKER_SCRIPT) -o $@ $^ $(KERNEL_LDFLAGS)

.PHONY: check_iso
check_iso: kernel
	$(Q)$(MAKE) -C userspace all
	$(Q)$(MAKE) $(ISO)

.PHONY: iso
iso: check_iso

$(ISO): $(INITRD) $(KERNEL)
	$(Q)./scripts/chaos-iso.sh -b "$(BOOT_FLAGS)"

kconfig:
	$(Q)printf "  SHELL\t check_config\n"
	$(Q)./scripts/check_config.sh

.PHONY: config
config:
	$(Q)printf "  SHELL\t kconfig\n"
	$(Q)./scripts/kconfig/main.sh
	$(Q)printf "  SHELL\t gen-config.sh\n"
	$(Q)./scripts/gen-config.sh

.PHONY: run
run: iso
	$(Q)./scripts/qemu.sh -m 1G -a "$(ARCH)" -s "$(SMP)" -c "$(CPU)"

.PHONY: monitor
monitor: iso
	$(Q)./scripts/qemu.sh -t -m 1G -a "$(ARCH)" -s "$(SMP)" -c "$(CPU)"

.PHONY: debug
debug: iso
	$(Q)./scripts/qemu.sh -d -m 1G -a "$(ARCH)" -s "$(SMP)" -c "$(CPU)"

.PHONY: kvm
kvm: iso
	$(Q)./scripts/qemu.sh -t -k -m 1G -a "$(ARCH)" -s "$(SMP)" -c "$(CPU)"

.PHONY: clean
clean:
	$(Q)$(MAKE) -C arch clean
	$(Q)$(MAKE) -C kernel clean
	$(Q)$(MAKE) -C drivers clean
	$(Q)$(MAKE) -C lib clean
	$(Q)$(MAKE) -C userspace clean
	$(Q)$(RM) "$(ISO)" "$(KERNEL)"

.PHONY: re
re: clean all
