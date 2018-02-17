##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

RELATIVE_DIR := $(subst $(PROJECT_DIR)/,,$(shell pwd))

# Binaries
export DUMBFS_DATA	:= dumbfs_files
export USERSPACE_BINS	:= init

# Flags
CFLAGS		+= -I "$(PROJECT_DIR)/userspace"
LDFLAGS		+= -emain # Set entry point to 'main' (and not '_start')

all:		$(INITRD)

KERNEL_OBJ = kernel.o

.PHONY: all
all: $(INITRD)

$(INITRD): $(wildcard $(DUMBFS_DATA)/*)
	$(Q)printf "  SHELL\t scripts/initrd.sh\n"
	../scripts/initrd.sh

.SECONDEXPANSION:

$(USERSPACE_BINS): $$(addsuffix .o, $$@)
	$(Q)printf "  LD\t $(RELATIVE_DIR)/$@\n"
	$(Q)printf "  $<\n"
	$(LD) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	$(Q)$(RM) $(addsuffix .o, $(USERSPACE_BINS))
	$(Q)$(RM) $(addsuffix .d, $(USERSPACE_BINS))
	$(Q)$(RM) $(USERSPACE_BINS)
	$(Q)$(RM) $(DUMBFS)

-include $(addsuffix .d, $(USERSPACE_BINS))
%.o: %.c
	$(Q)printf "  CC\t $(RELATIVE_DIR)/$<\n"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@
