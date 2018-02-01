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
declare BOOT_ARGS=""

function print_usage() {
	printf "Usage: $0 [OPTIONS]\n"
	printf "\t-b <boot_flags>              Set some additionnal flags to give to the kernel.\n"
	exit 1
}

while getopts b: FLAG; do
	case $FLAG in
		b)
			BOOT_ARGS="$OPTARG";;
		*)
			printf "Unknown option\n"
			print_usage
	esac
done

if [ ! -f "$PROJECT_DIR/$KERNEL" ]; then
	printf "  MAKE\t $KERNEL\n"
	make -C "$PROJECT_DIR" --no-print-directory kernel
fi

TEMP=$(mktemp -d)

mkdir -p -- "$TEMP/boot/grub"
cp -- "$PROJECT_DIR/$KERNEL" "$TEMP/boot/$KERNEL"
cp -- "$PROJECT_DIR/$(basename "$INITRD")" "$TEMP/boot/$(basename "$INITRD")"

cat -- > "$TEMP/boot/grub/grub.cfg" << EOF
set timeout=0

menuentry "ChaOS" {
	multiboot2 /boot/$KERNEL ${BOOT_ARGS}
	module2 /boot/initrd.img
}
EOF

GRUB_OUTPUT=$(mktemp)

printf "  GRUB\t $ISO\n"

if ! (command -v grub-mkrescue &> /dev/null && grub-mkrescue -o "$PROJECT_DIR/$ISO" "$TEMP" &> "$GRUB_OUTPUT" ||
     command -v grub2-mkrescue &> /dev/null && grub2-mkrescue -o "$PROJECT_DIR/$ISO" "$TEMP" &> "$GRUB_OUTPUT"); then
	cat "$GRUB_OUTPUT"
	exit 1
fi

rm -rf "$GRUB_OUTPUT"
rm -rf "$TEMP"
