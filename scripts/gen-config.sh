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
	echo
	echo "Please generate a kernel configuration using 'make config'"
	echo
	exit 1
fi

sed 's/\#/\/\//g' "$PROJECT_DIR/kconfig" | sed 's/KCONFIG\[/#define /g' | sed 's/\]=/ /g' > "$PROJECT_DIR/include/kconfig.h"
