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
SRC_ASM += $(wildcard *.asm)

OBJS += $(SRC_C:.c=.o) $(SRC_ASM:.asm=.o)
DEPS = $(OBJS:.o=.d)

ARCH_OBJ = $(ARCH).o

.PHONY: all
all: $(ARCH_OBJ)

$(ARCH_OBJ): $(OBJS)
	$(Q)printf "  LD\t $(RELATIVE_DIR)/$@\n"
	$(Q)$(LD) $(LDFLAGS) -o $@ -r $^

.PHONY: clean
clean:
	$(Q)$(RM) $(ARCH_OBJ) $(OBJS) $(DEPS)

-include $(DEPS)
%.o: %.c
	$(Q)printf "  CC\t $(RELATIVE_DIR)/$<\n"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)
%.o: %.asm
	$(Q)printf "  NASM\t $(RELATIVE_DIR)/$<\n"
	$(Q)$(NASM) $(NASMFLAGS) -M $< -o $@ > $(patsubst %.o,%.d, $@)
	$(Q)$(NASM) $(NASMFLAGS) $< -o $@
