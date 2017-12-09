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

new_kconfig_var KCONFIG_DEBUG_MULTIBOOT 0
new_kconfig_var KCONFIG_DEBUG_PMM 0
new_kconfig_var KCONFIG_DEBUG_VMM 0

function menu_debug() {
	menu_checklist "Debug Options" \
		"Enable Multiboot Debug" KCONFIG_DEBUG_MULTIBOOT \
			"This options enables debugging outputs and assertions for the Multiboot's tables parser" \
		"Enable PMM Debug" KCONFIG_DEBUG_PMM \
			"This options enables debugging outputs and assertions for the Physical Memory Manager" \
		"Enable VMM Debug" KCONFIG_DEBUG_VMM \
			"This options enables debugging outputs and assertions for the Virtual Memory Manager" \

}

function main_menu() {
	menu_link "Main Menu" \
		"Processor" menu_cpu \
			"Couple of options to help you tune your processor as you wish" \
		"Debug" menu_debug \
			"Couple of options to help you debug the kernel. They may make it slow down a bit." \

}

# Load arch-dependent kconfig
source "$PROJECT_DIR/arch/$ARCH/kconfig.sh"
