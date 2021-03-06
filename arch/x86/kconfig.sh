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

# Processor menu variables
new_kconfig_var PAGE_SIZE 4096u
new_kconfig_var KCONFIG_ENABLE_SMP 1

function menu_cpu_opts
{
	menu_checklist "Processor" \
		"Enable SMP" KCONFIG_ENABLE_SMP \
			"Enables support for Symmetric Multiprocessing, which lets you have multiple processors running in the same system."
}

function menu_cpu_max_cores
{
	menu_integer "Maximum number of cores" KCONFIG_MAX_CPUS 1 64
}

function menu_cpu() {
	menu_link "Processor" \
		"Processor Options" menu_cpu_opts \
			"Couple of options to help you tune your processor as you wish" \
		"if" KCONFIG_ENABLE_SMP \
			"SMP Maximum Cores" menu_cpu_max_cores \
				"Maximum number of processors enabled at the same time"
	if [[ $(get_kconfig_value KCONFIG_ENABLE_SMP) -ne 1 ]]
	then
		set_kconfig_value KCONFIG_MAX_CPU 1
	fi

}
