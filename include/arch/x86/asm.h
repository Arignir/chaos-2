/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#ifndef _ARCH_X86_ASM_H_
# define _ARCH_X86_ASM_H_

# include <chaosdef.h>

/*
** An enum to associate a label for each MSRs.
*/
enum msr_id
{
	MSR_APIC_BASE		= 0x1B,
};

/*
** An enum to associate a label for each ports
*/
enum port
{
	PORT_PIC_MASTER_CMD	= 0x20,
	PORT_PIC_MASTER_DATA	= 0x21,
	PORT_IMCR_ADDRESS	= 0x22,
	PORT_IMCR_DATA		= 0x23,
	PORT_CMOS		= 0x70,
	PORT_CMOS_RETURN	= 0x71,
	PORT_PIC_SLAVE_CMD	= 0xA0,
	PORT_PIC_SLAVE_DATA	= 0xA1,
};

/*
** An structure to associate a label for each eflags bits
*/
union eflags
{
	struct {
		int CF			: 1;
		int __res		: 1;
		int PF			: 1;
		int __res2		: 1;
		int AF			: 1;
		int __res3		: 1;
		int ZF			: 1;
		int SF			: 1;
		int TF			: 1;
		int IF			: 1;
	};
	uint32 value;
};

static_assert(sizeof(union eflags) == sizeof(uint32));

/*
** For a description of the following functions,
** please refer to Intel's Instruction Set Reference.
*/

static inline void
outb(ushort port, uchar data)
{
	asm volatile("out %0, %1" :: "a"(data), "d"(port));
}

static inline uchar
inb(ushort port)
{
	uchar data;

	asm volatile("in %1, %0" : "=a"(data) : "d"(port));
	return (data);
}

static inline void
io_delay(void)
{
	asm volatile("outb %%al, $0x80" : : "a"(0));
}

static inline void
sti(void)
{
	asm volatile("sti");
}

static inline void
cli(void)
{
	asm volatile("cli");
}

static inline void
cpuid(uint32 code, uint32 *eax, uint32 *edx)
{
	asm volatile("cpuid" : "=a"(*eax), "=d"(*edx) : "a"(code) : "ebx", "ecx");
}

static inline void
cpuid_string(uint32 code, uchar str[12])
{
	asm volatile("cpuid" : "=b"(*(uint32 *)str), "=d"(*(uint32 *)(str + 4)), "=c"(*(uint32 *)(str + 8)) : "a"(code));
}

static inline uint64
read_msr(enum msr_id msr)
{
	uint32 low;
	uint32 high;

	asm volatile("rdmsr" : "=a"(low), "=d"(high) : "c"((uint32)msr));
	return (((uint64)high << 32u) | (uint64)low);
}

static inline void
write_msr(enum msr_id msr, uint64 val)
{
	uint32 low;
	uint32 high;

	low = (uint32)val;
	high = (uint32)(val >> 32u);
	asm volatile("wrmsr" : : "a"(low), "d"(high), "c"((uint32)msr));
}

static inline union eflags
get_eflags(void)
{
	union eflags e;

	asm volatile("pushfl; popl %0" : "=rm"(e.value) :: "memory");
	return (e);
}

static inline void
set_eflags(uint32 value)
{
	asm volatile("pushl %0; popfl" :: "g"(value) : "memory", "cc");
}

static inline void
lidt(void *base, ushort size)
{
	struct {
		uint16_t length;
		void*    base;
	} __attribute__((packed)) idtr = { size, base };

	asm volatile("lidt %0" :: "m"(idtr));
}

static inline void
interrupt(uchar v)
{
	asm volatile("int %0" :: "i"(v));
}

static inline uint
xchg(volatile uint *addr, uint newval)
{
	uint res;

	asm volatile("lock xchgl %0, %1" : "+m"(*addr), "=a"(res) : "1"(newval) : "cc");
	return (res);
}

static inline uint64
rdtsc(void)
{
	uint32 a;
	uint32 d;

	asm volatile("rdtsc" : "=a"(a), "=d"(d));
	return ((uint64)d << 32u | a);
}

static inline void
hlt(void)
{
	asm volatile("hlt");
}

#endif /* !_ARCH_X86_ASM_H_ */
