##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

# Binaries
export USERSPACE_BINS	:= game

# Flags
CFLAGS		+= -I "$(PROJECT_DIR)/userspace"
LDFLAGS		+= -emain # Set entry point to 'main' (and not '_start')

all:		$(INITRD)

KERNEL_OBJ = kernel.o

.PHONY: all
all: $(INITRD)

$(INITRD): $(USERSPACE_BINS)
	$(Q)printf "  SHELL\t initrd.sh\n"
	../scripts/initrd.sh

.SECONDEXPANSION:
$(USERSPACE_BINS): $$(addsuffix .o, $$@)
	$(Q)printf "  LD\t $@\n"
	$(LD) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	$(Q)$(RM) $(addsuffix .o, $(USERSPACE_BINS))
	$(Q)$(RM) $(addsuffix .d, $(USERSPACE_BINS))
	$(Q)$(RM) $(USERSPACE_BINS)

-include $(addsuffix .d, $(USERSPACE_BINS))
%.o: %.c
	$(Q)printf "  CC\t $<\n"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@
