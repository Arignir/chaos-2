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

function print_usage {
	printf "Usage: $0 [OPTIONS]\n"
	printf "\t-a <arch>		architecture (Valid values are 'x86' and 'x86_64') (Default: x86)\n"
	printf "\t-d			debug mode\n"
	printf "\t-t			monitor mode\n"
	printf "\t-k			enables kvm (if available)\n"
	printf "\t-m <MB> 		memory (in MB) (Default: 512MB)\n"
	printf "\t-s <SMP>		number of cores (Default: 1)\n"
	printf "\t-h			print this help menu\n"
	exit 1
}

DEBUG=0
KVM=0
MONITOR=0
MEMORY=512
ARCH="x86"
SMP=1

while getopts dtkhm:a:s: FLAG; do
	case $FLAG in
		d) DEBUG=1;;
		k) KVM=1;;
		t) MONITOR=1;;
		m) MEMORY="$OPTARG";;
		s) SMP="$OPTARG";;
		a) ARCH="$OPTARG";;
		h) print_usage;;
		\?)
			printf "Unknown option\n"
			print_usage
	esac
done

shift $((OPTIND-1))

QEMU=""
case $ARCH in
	"x86")		QEMU="qemu-system-i386";;
	"x86_64")	QEMU="qemu-system-x86_64";;
	*)
		printf "Unknown architecture $ARCH\n"
		print_usage
esac

if [ ! -f "$ISO" ]; then
	RULES="iso"
	printf "  MAKE\t $RULES\n"
	make -C "$PROJECT_DIR" --no-print-directory $RULES
fi

ARGS="-m $MEMORY -cdrom $ISO -smp $SMP"

if [ $DEBUG == 1 ]; then
	ARGS+=" -s -d int,cpu_reset,guest_errors,unimp --no-reboot"
fi

if [ $KVM == 1 ]; then
	ARGS+=" --enable-kvm"
fi

if [ $MONITOR == 1 ]; then
	ARGS+=" -monitor stdio"
else
	ARGS+=" -serial stdio"
fi

if ! which "$QEMU" &> /dev/null; then
	printf "  ERROR\t You must install $QEMU first\n"
	exit 1
fi

printf "  QEMU\t $ISO\n"
$QEMU $ARGS
