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
new_kconfig_var KCONFIG_ENABLE_SMP 1
new_kconfig_var KCONFIG_MAX_CPUS 8

function menu_cpu_opts
{
	menu_checklist "Processor" \
		"Enable SMP" KCONFIG_ENABLE_SMP \
			"This enables support Symmetric Multiprocessing, which let you have multiple processors running in one system."
}

function menu_cpu_max_cores
{
	error="Number of cores (Max 255)"
	while :; do
		menu_textbox "$error" $(get_kconfig_value KCONFIG_MAX_CPUS) 2> "$TEMP"
		nb=$(cat "$TEMP")
		if [[ "$nb" == "" ]]; then
			break
		fi
		re='^[0-9]+$'
		if [[ "$nb" =~ $re && "$nb" -gt 0 && "$nb" -le 255 ]]; then
			set_kconfig_value KCONFIG_MAX_CPUS "$nb"
			break
		fi
		error="$nb: Error, you must type a number between 1 and 255"
	done
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
