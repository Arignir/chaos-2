##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -u

#
# Each menu within the ChaOS configuration
#

function main_menu() {
	menu_link "Main Menu" \
		"Processor" menu_cpu \
			"Couple of options to help you tune your processor as you wish" \

}

# Load arch-dependent kconfig
source "$PROJECT_DIR/arch/$ARCH/kconfig.sh"
