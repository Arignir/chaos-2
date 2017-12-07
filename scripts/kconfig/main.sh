#!/usr/bin/env bash

##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -u

declare SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
declare PROJECT_DIR="$SCRIPT_DIR/../../"
declare KCONFIG_DEFAULT_PATH="$PROJECT_DIR/kconfig"

# Load kconfig saving/loading system
source "$SCRIPT_DIR/kconfig.sh"

# Load menu system
source "$SCRIPT_DIR/menu.sh"

# Load main menu
source "$SCRIPT_DIR/mainmenu.sh"

# Load current config
if [[ -f "$KCONFIG_DEFAULT_PATH" ]]; then
	source "$KCONFIG_DEFAULT_PATH"
fi

if [[ $# -eq 1 && "$1" == "gen-default" ]]; then
	save_config "$KCONFIG_DEFAULT_PATH"
else
	main_menu
fi
