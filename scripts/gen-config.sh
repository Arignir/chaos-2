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

declare kconfig_h="$PROJECT_DIR/include/kconfig.h"

head -n 7 "$PROJECT_DIR/kconfig" | sed 's/\#/\/\//g' > "$kconfig_h"
echo "#ifndef KCONFIG_H" >> "$kconfig_h"
echo "# define KCONFIG_H" >> "$kconfig_h"

tail -n +7 "$PROJECT_DIR/kconfig" | sed 's/\#/\/\//g' | sed 's/KCONFIG\[/# define /g' | sed 's/\]=/ /g' >> "$PROJECT_DIR/include/kconfig.h"

echo "" >> "$kconfig_h"
echo "#endif /* !KCONFIG_H */" >> "$kconfig_h"
