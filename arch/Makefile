##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

RELATIVE_DIR := $(subst $(PROJECT_DIR)/,,$(shell pwd))

OBJ = arch.o

.PHONY: all
all: check_up

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C $(ARCH) all
	$(Q)$(MAKE) $(OBJ)

$(OBJ): $(ARCH)/$(ARCH).o
	$(Q)printf "  LD\t $(RELATIVE_DIR)/$@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(MAKE) -C $(ARCH) clean
	$(Q)$(RM) $(OBJ)
