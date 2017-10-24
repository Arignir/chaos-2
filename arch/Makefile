##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

ARCHIVE = arch.a

.PHONY: all
all: check_up | $(ARCHIVE)

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C $(ARCH) all

$(ARCHIVE): $(ARCH)/$(ARCH).a
	$(Q)printf "  AR\t arch.a\n"
	$(Q)$(AR) -rcT arch.a $(ARCH)/$(ARCH).a

.PHONY: clean
clean:
	$(Q)$(MAKE) -C $(ARCH) clean
	$(Q)$(RM) arch.a
