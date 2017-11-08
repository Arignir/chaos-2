##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -u

# Variable containing the current kernel configuration
declare -A KCONFIG

# new_kconfig_var <name> <default_val>
function new_kconfig_var() {
	KCONFIG["$1"]="$2"
}

# set_kconfig_bool <name> <value>
function set_kconfig_bool() {
	if [[ "$2" -eq 0 ]]; then
		KCONFIG["$1"]=0
	else
		KCONFIG["$1"]=1
	fi
}

# set_kconfig_value <name> <value>
function set_kconfig_value() {
	KCONFIG["$1"]="$2"
}

# get_kconfig_value <name>
function get_kconfig_value() {
	echo "${KCONFIG["$1"]}"
}

# get_kconfig_status <name>
function get_kconfig_status() {
	if [[ "${KCONFIG["$1"]}" -eq 0 ]]; then
		echo OFF
	else
		echo ON
	fi
}

# save_config <path_to_config_file>
function save_config() {
	declare path

	path="$PROJECT_DIR/$1"
	if [[ $1 == /* ]]; then
		path="$1"
	fi
	path=$(readlink -m -- "$path")

	if [[ -e "$path" && ! -f "$path" ]]; then
		echo "The given kconfig path ($path) exists and is not a regular file! Aborting."
		exit 1
	fi
	rm -rf -- "$path"

	cat -- > "$path" << EOF
#
# ChaOS/$ARCH $MAJOR.$MINOR.$PATCH$EXTRA
#
# This file has been automaticaly generated using 'make menuconfig'.
# You shoudln't modify it directly unless you know what you're doing.
#

EOF
	for key in "${!KCONFIG[@]}"; do
		echo "KCONFIG[$key]=${KCONFIG[$key]}" >> "$path"
	done
	echo "Configuration written to $path"
}
