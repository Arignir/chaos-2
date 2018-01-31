##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

RELATIVE_DIR := $(subst $(PROJECT_DIR)/,,$(shell pwd))

SRC_C += $(wildcard *.c)

OBJS += $(SRC_C:.c=.o)
DEPS = $(OBJS:.o=.d)

KERNEL_OBJ = kernel.o

.PHONY: all
all: $(KERNEL_OBJ)

$(KERNEL_OBJ): $(OBJS)
	$(Q)printf "  LD\t $(RELATIVE_DIR)/$@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(RM) $(KERNEL_OBJ) $(OBJS) $(DEPS)

-include $(DEPS)
%.o: %.c
	$(Q)printf "  CC\t $(RELATIVE_DIR)/$<\n"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@
