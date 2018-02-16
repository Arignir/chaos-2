#!/usr/bin/env bash

##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Jakob Kellendonk <jakob.kellendonk@epitech.eu>
##
##############################################################################

if (( $# != 2 ))
then
	echo -E "usage: ./${0} IN_DIR OUT_FILE"
	echo -E "[DUMBFS_GEN]	Missing args, exiting."
	exit 1
fi

IN_DIR=${1}
OUT_FILE=${2}

shopt -s nullglob

>${OUT_FILE}

add_int() {
	local x=${1}
	local out;
	printf -v out '\\x%02x\\x%02x\\x%02x\\x%02x' $((x & 0xFF)) $((x >> 8 & 0xFF)) $((x >> 16 & 0xFF)) $((x >> 24 & 0xFF))
	printf ${out} >> ${OUT_FILE}
}

add_file() {
	echo -E "[DUMBFS_GEN]	adding ${1} to dumbfsfile system"

	local file_name=${1}
	local file_path=${IN_DIR}/${file_name}
	local name_size=${#file_name}
	local file_size=$(stat -Lc%s ${file_path})
	local entry_size=$((file_size + name_size + 1))
	add_int ${entry_size}
	add_int ${file_size}
	echo -nE "${file_name}" >> ${OUT_FILE}
	printf '\x00' >> ${OUT_FILE}
	cat ${file_path} >> ${OUT_FILE}
}

files=(${IN_DIR}/*)

echo -E "[DUMBFS_GEN]	found ${#files[@]} files"

add_int ${#files[@]}
for file in ${files[@]}
do
	add_file ${file##*/};
done
