/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/random.h>
#include <kernel/init.h>

/*
** Pseudo-random number generation using a sort of RC4 cipher algorithm.
**
** Note that this PRNG isn't secure for cryptographic or security-sensitive
** purposes, but it's enough for this tiny kernel.
*/

# define SEED_SIZE	256
static uchar seeds[SEED_SIZE] = { 0 };
static uint seed_1 = 0;
static uint seed_2 = 0;

/*
** Returns a pseudo-random unsigned char in the range [0, UCHAR_MAX]
*/
uchar
rand_uchar(void)
{
	uchar tmp;

	seed_1 = (seed_1 + 1u) & 0xFF;
	seed_2 = (seed_2 + seeds[seed_1]) & 0xFF;

	/* swap seed[seed_1] and seed[seed_2] */
	tmp = seeds[seed_1];
	seeds[seed_1] = seeds[seed_2];
	seeds[seed_2] = tmp;
	return (seeds[(seeds[seed_1] + seeds[seed_2]) & 0xFF]);
}

/*
** Returns a pseudo-random unsigned char in the range [0, USHORT_MAX]
*/
ushort
rand_ushort(void)
{
	ushort rnd;

	rnd = rand_uchar();
	for (size_t i = 1; i < sizeof(rnd); ++i) {
		rnd = (rnd << 8u) | rand_uchar();
	}
	return (rnd);
}

/*
** Returns a pseudo-random unsigned char in the range [0, UINT_MAX]
*/
uint
rand_uint(void)
{
	uint rnd;

	rnd = rand_uchar();
	for (size_t i = 1; i < sizeof(rnd); ++i) {
		rnd = (rnd << 8u) | rand_uchar();
	}
	return (rnd);
}

/*
** Returns a pseudo-random unsigned char in the range [0, ULONG_MAX]
*/
ulong
rand_ulong(void)
{
	ulong rnd;

	rnd = rand_uchar();
	for (size_t i = 1; i < sizeof(rnd); ++i) {
		rnd = (rnd << 8u) | rand_uchar();
	}
	return (rnd);
}

/*
** Seeds the random algorithm using the current timestamp.
**
** NOTE: The current implementation doesn't use time yet, as it's not
** implemented. Instead it's using the number of cpu clock cicles elapsed
** since boot time. (TODO FIXME)
*/
#include <arch/x86/asm.h> /* TODO FIXME */
void
seed_by_time(void)
{
	size_t i;
	size_t j;
	uint64 tsc;
	unsigned char *key;
	unsigned char tmp;

	tsc = rdtsc();
	key = (unsigned char *)&tsc;
	for (i = 0; i < SEED_SIZE; ++i) {
		seeds[i] = i;
	}
	for (i = j = 0; i < SEED_SIZE; ++i) {
		j = (j + seeds[i] + key[i % sizeof(tsc)]) & 0xFF;
		/* Swap seeds[i] and seeds[j] */
		tmp = seeds[i];
		seeds[i] = seeds[j];
		seeds[j] = tmp;
	}
	seed_1 = 0;
	seed_2 = 0;
}

NEW_INIT_HOOK(seed_random, &seed_by_time, INIT_LEVEL_DRIVER_EARLY);
