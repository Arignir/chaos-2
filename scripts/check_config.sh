#!/usr/bin/env bash

##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -e -u

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/../"

if [ ! -f "$PROJECT_DIR/kconfig" ]; then
	printf "You have no configuration with your kernel.\n"
	printf "A default one will be used. You can edit it using 'make config'.\n"
	"$SCRIPT_DIR/kconfig/main.sh" "gen-default"
	"$SCRIPT_DIR/gen-config.sh"
fi
