##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

RELATIVE_DIR := $(subst $(PROJECT_DIR)/,,$(shell pwd))

LIB_OBJ = lib.o

.PHONY: all
all: check_up

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C libc all
	$(Q)$(MAKE) -C bdev all
	$(Q)$(MAKE) -C fs all
	$(Q)$(MAKE) $(LIB_OBJ)

$(LIB_OBJ): libc/libc.o bdev/bdev.o fs/fs.o
	$(Q)printf "  LD\t $(RELATIVE_DIR)/$@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(MAKE) -C libc clean
	$(Q)$(MAKE) -C bdev clean
	$(Q)$(MAKE) -C fs clean
	$(Q)$(RM) $(LIB_OBJ)
