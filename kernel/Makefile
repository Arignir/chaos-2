##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

SRC_C += $(wildcard *.c)
SRC_ASM += $(wildcard *.S)

OBJS += $(SRC_C:.c=.o) $(SRC_ASM:.S=.o)
DEPS = $(OBJS:.o=.d)

KERNEL_OBJ = kernel.o

.PHONY: all
all: $(KERNEL_OBJ)

$(KERNEL_OBJ): $(OBJS)
	$(Q)printf "  LD\t $@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(RM) $(KERNEL_OBJ) $(OBJS) $(DEPS)

-include $(DEPS)
%.o: %.c
	$(Q)printf "  CC\t $^\n"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

-include $(DEPS)
%.o: %.S
	$(Q)printf "  AS\t $^\n"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@
