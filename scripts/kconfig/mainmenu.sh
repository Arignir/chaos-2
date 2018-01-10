##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -u

#
# Each menu within the ChaOS configuration
#

# Scheduler
new_kconfig_var KCONFIG_MAX_THREADS		32
new_kconfig_var KCONFIG_THREAD_STACK_SIZE	32

# Memory
new_kconfig_var KCONFIG_ENABLE_ASLR		1

# Debug
new_kconfig_var KCONFIG_DEBUG_MULTIBOOT		0
new_kconfig_var KCONFIG_DEBUG_LOCKS		1
new_kconfig_var KCONFIG_DEBUG_PMM		1
new_kconfig_var KCONFIG_DEBUG_VMM 		1
new_kconfig_var KCONFIG_DEBUG_THREAD		1
new_kconfig_var KCONFIG_DEBUG_KALLOC		1

function menu_debug() {
	menu_checklist "Debug Options" \
		"Enable Multiboot Debug" KCONFIG_DEBUG_MULTIBOOT \
			"This options enables debugging outputs and assertions for the Multiboot's tables parser" \
		"Enable Locks Debug" KCONFIG_DEBUG_LOCKS \
			"This options enables debugging outputs and assertions for kernel's lockers (spinlocks, mutexes, semaphores etc.)" \
		"Enable PMM Debug" KCONFIG_DEBUG_PMM \
			"This options enables debugging outputs and assertions for the Physical Memory Manager" \
		"Enable VMM Debug" KCONFIG_DEBUG_VMM \
			"This options enables debugging outputs and assertions for the Virtual Memory Manager" \
		"Enable Thread Debug" KCONFIG_DEBUG_THREAD \
			"This options enables debugging outputs and assertions for functions related to threads." \
		"Enable Kernel Heap Debug" KCONFIG_DEBUG_KALLOC \
			"This options enables debugging assertions for kalloc(), kfree() and all other kernel-heap related functions." \

}

function menu_max_threads() {
	menu_integer "Maximum number of concurrent threads" KCONFIG_MAX_THREADS 8 16384
}

function menu_thread_stack_size() {
	menu_integer "Stack size (in pages)" KCONFIG_THREAD_STACK_SIZE 1 64
}

function menu_scheduler() {
	menu_link "System" \
		"Maximum Threads" menu_max_threads \
			"Maximum number of threads running at the same time" \
		"Thread stack size" menu_thread_stack_size \
			"Default stack size for a given thread" \

}

function menu_memory() {
	menu_checklist "Memory" \
		"Enable ASLR" KCONFIG_ENABLE_ASLR \
			"Enables support for ASLR (Address Space Layout Randomization). This is a security feature that is higly recommended." \

}

function main_menu() {
	menu_link "Main Menu" \
		"Processor" menu_cpu \
			"Couple of options to help you tune your processor as you wish" \
		"Threads and Scheduling" menu_scheduler \
			"Couple of options to help you tune your system as you wish" \
		"Memory" menu_memory \
			"Couple of options to help you tune memory management as you wish" \
		"Debugging" menu_debug \
			"Couple of options to help you debug the kernel, but they may make it slower." \

}

# Load arch-dependent kconfig
source "$PROJECT_DIR/arch/$ARCH/kconfig.sh"
