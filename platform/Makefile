##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

ARCHIVE = platform.a

.PHONY: all
all: check_up | $(ARCHIVE)

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C pc all

$(ARCHIVE): pc/pc.a
	$(Q)printf "  AR\t $(ARCHIVE)\n"
	$(Q)$(AR) -rcT $(ARCHIVE) pc/pc.a

.PHONY: clean
clean:
	$(Q)$(MAKE) -C pc clean
	$(Q)$(RM) $(ARCHIVE)
