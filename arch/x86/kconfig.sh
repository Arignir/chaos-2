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
new_kconfig_var KCONFIG_ENABLE_SMP 1

function menu_cpu() {
	menu_checklist "Processor" \
		"Enable SMP" KCONFIG_ENABLE_SMP \
			"This enables support Symmetric Multiprocessing, which let you have multiple processors running in one system." \

}
