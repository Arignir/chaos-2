##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

LIB_OBJ = lib.o

.PHONY: all
all: check_up

.PHONY: check_up
check_up:
	$(Q)$(MAKE) -C libc all
	$(Q)$(MAKE) $(LIB_OBJ)

$(LIB_OBJ): libc/libc.o
	$(Q)printf "  LD\t $@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(MAKE) -C libc clean
	$(Q)$(RM) $(LIB_OBJ)
