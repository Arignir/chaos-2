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

new_kconfig_var KCONFIG_MAX_THREADS		32

new_kconfig_var KCONFIG_DEBUG_MULTIBOOT		0
new_kconfig_var KCONFIG_DEBUG_LOCKS		0
new_kconfig_var KCONFIG_DEBUG_PMM		0
new_kconfig_var KCONFIG_DEBUG_VMM 		0

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

}

function menu_max_threads() {
	error="Number of threads (Max 16384)"
	while :; do
		menu_textbox "$error" $(get_kconfig_value KCONFIG_MAX_THREADS) 2> "$TEMP"
		nb=$(cat "$TEMP")
		if [[ "$nb" == "" ]]; then
			break
		fi
		re='^[0-9]+$'
		if [[ "$nb" =~ $re && "$nb" -ge 8 && "$nb" -le 16384 ]]; then
			set_kconfig_value KCONFIG_MAX_THREADS "$nb"
			break
		fi
		error="$nb: Error, you must type a number between 8 and 255"
	done
}

function menu_system() {
	menu_link "System" \
		"Maximum Threads" menu_max_threads \
			"Maximum number of threads running at the same time"
}

function main_menu() {
	menu_link "Main Menu" \
		"Processor" menu_cpu \
			"Couple of options to help you tune your processor as you wish" \
		"System" menu_system \
			"Couple of options to help you tune your system as you wish" \
		"Debug" menu_debug \
			"Couple of options to help you debug the kernel. They may make it slow down a bit." \

}

# Load arch-dependent kconfig
source "$PROJECT_DIR/arch/$ARCH/kconfig.sh"
