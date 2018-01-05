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

declare SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
declare PROJECT_DIR="$SCRIPT_DIR/../"

declare OUTPUT=initrd.img
declare OUTPUT_PATH="$PROJECT_DIR/$OUTPUT"

# Add root paths for distros that have a different path for the root user
declare PATH="$PATH:/sbin:/usr/sbin/"

# Check that dependencies are installed
if ! which dd &> /dev/null; then
	printf "You must install dd"
	exit 1
fi

# Create ramdisk image
echo "I love ChaOS" > "$OUTPUT_PATH"
