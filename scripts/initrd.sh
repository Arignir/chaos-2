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
shopt -s nullglob

declare SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
declare PROJECT_DIR="$SCRIPT_DIR/../"

declare INITRD_PATH="$INITRD" # Define in Makefile
touch "$INITRD_PATH"

if [[ $# -eq 0 ]]; then
	echo "Usage: $0 <files>"
	exit 1
fi

declare INPUT=$@

add_int() {
	local x="$1"
	local out;
	printf -v out '\\x%02x\\x%02x\\x%02x\\x%02x' $((x & 0xFF)) $((x >> 8 & 0xFF)) $((x >> 16 & 0xFF)) $((x >> 24 & 0xFF))
	printf "$out" >> "$INITRD_PATH"
}

add_file() {
	printf "  INITRD $1\n"

	local file_path="$1"
	local file_name="$2"

	local name_size="${#file_name}"
	local file_size=$(stat -Lc%s "$file_path")
	local entry_size=$((file_size + name_size + 1))
	add_int "$entry_size"
	add_int "$file_size"
	printf "$file_name" >> "$INITRD_PATH"
	printf '\x00' >> "${INITRD_PATH}"
	cat "${file_path}" >> ${INITRD_PATH}
}

add_int ${#INPUT}
for file in $INPUT
do
	add_file "$file" $(basename $file)
done
