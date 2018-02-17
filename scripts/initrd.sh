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

shopt -s nullglob

>${OUTPUT_PATH}

add_int() {
	local x=${1}
	local out;
	printf -v out '\\x%02x\\x%02x\\x%02x\\x%02x' $((x & 0xFF)) $((x >> 8 & 0xFF)) $((x >> 16 & 0xFF)) $((x >> 24 & 0xFF))
	printf ${out} >> ${OUTPUT_PATH}
}

add_file() {
	echo -E "[DUMBFS_GEN]	adding ${1} to dumbfsfile system"

	local file_name=${1}
	local file_path=${DUMBFS_DATA}/${file_name}
	local name_size=${#file_name}
	local file_size=$(stat -Lc%s ${file_path})
	local entry_size=$((file_size + name_size + 1))
	add_int ${entry_size}
	add_int ${file_size}
	echo -nE "${file_name}" >> ${OUTPUT_PATH}
	printf '\x00' >> ${OUTPUT_PATH}
	cat ${file_path} >> ${OUTPUT_PATH}
}

files=(${DUMBFS_DATA}/*)

echo -E "[DUMBFS_GEN]	found ${#files[@]} files"

add_int ${#files[@]}
for file in ${files[@]}
do
	add_file ${file##*/};
done
