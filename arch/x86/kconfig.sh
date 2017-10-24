#!/usr/bin/sh

##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -u

# Main menu variables
new_kconfig_var KCONFIG_ENABLE_FPU 0

function menu_cpu() {
	menu_checklist "Processor" \
		"Enable FPU" KCONFIG_ENABLE_FPU \
			"This enables support for the floating point unit." \

}
