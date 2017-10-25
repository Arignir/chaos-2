##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

ARCHIVE = lib.a

.PHONY: all
all: check_up | $(ARCHIVE)

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C libc all

$(ARCHIVE): libc/libc.a
	$(Q)printf "  AR\t $(ARCHIVE)\n"
	$(Q)$(AR) -rcT $(ARCHIVE) libc/libc.a

.PHONY: clean
clean:
	$(Q)$(MAKE) -C libc clean
	$(Q)$(RM) $(ARCHIVE)
