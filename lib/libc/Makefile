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

LIBC_OBJ = libc.o

.PHONY: all
all: $(LIBC_OBJ)

$(LIBC_OBJ): $(OBJS)
	$(Q)printf "  LD\t $@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(RM) $(LIBC_OBJ) $(OBJS) $(DEPS)

-include $(DEPS)
%.o: %.c
	$(Q)printf "  CC\t $^\n"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@

-include $(DEPS)
%.o: %.S
	$(Q)printf "  AS\t $^\n"
	$(Q)$(CC) $(CFLAGS) -c $^ -o $@
