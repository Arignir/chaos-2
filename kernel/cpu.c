/* ------------------------------------------------------------------------ *\
**
**  This file is part of the Chaos Kernel, and is made available under
**  the terms of the GNU General Public License version 2.
**
**  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
**
\* ------------------------------------------------------------------------ */

#include <kernel/cpu.h>

/* Number of CPUs on the current system. */
uint ncpu = 0;

/* An array for all possible cpus. */
struct cpu cpus[KCONFIG_MAX_CPUS] = { 0 };
