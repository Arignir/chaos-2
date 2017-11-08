##############################################################################
##
##  This file is part of the Chaos Kernel, and is made available under
##  the terms of the GNU General Public License version 2.
##
##  Copyright (C) 2017 - Benjamin Grange <benjamin.grange@epitech.eu>
##
##############################################################################

set -u

declare DIALOG=${DIALOG=dialog}

declare TITLE="ChaOS/$ARCH Kernel Configuration"
declare MENUHELP="Use arrow keys to naviguate the menu."
declare -i WIDTH=80
declare -i HEIGHT=25
declare -a COMMON_FLAGS=("--keep-tite" "--clear" "--backtitle" "$TITLE")
declare TEMP=$(mktemp)

if ! which $DIALOG > /dev/null; then
	echo "Dependency \"$DIALOG\" is missing"
	exit 1
fi

#
# Set of functions to build menus
#

# menu_msg title content
function menu_msg() {
	$DIALOG $COMMON_FLAGS --title "$1" --msgbox "$2" $HEIGHT $WIDTH  2> "$TEMP"
}

# menu_help <name> <content> [<kvar>]
function menu_help() {
	declare content

	content="Name: $1"
	if [[ $# -eq 3 ]]; then
		content="$content\nVariable name: $3"
	fi
	content="$content\n\n$2"
	menu_msg "$1" "$content"
}

# menu_textbox text default
function menu_textbox() {
	$DIALOG "--keep-tite" "--clear" --inputbox "$1" 10 60 "$2"
}

# menu_save
function menu_save() {
	menu_textbox "Enter a file name to which this configuration should be saved." "./kconfig" 2> "$TEMP"
	if [[ $? -eq 0 ]]; then
		declare kconfig

		kconfig=$(cat -- "$TEMP")
		if [[ "$kconfig" != "" ]]; then
			save_config "$kconfig"
		fi
	fi
}

# menu_link <title> [if VAR] [<name> <func> <help>]...
function menu_link() {
	declare default=""
	declare -a args=( "${@:2}" )

	while :; do
		declare -A links=()
		declare -A help=()
		declare -a dialog_flags=()
		declare -i retval
		declare choice

		for ((i = 0, j = 0; i < ${#args[*]}; i+=1, ++j)); do
			declare name

			if [[ "${args[$i]}" == "if" ]]; then
				declare var

				i=$(($i + 1))
				var="${args[$i]}"
				if [[ $(get_kconfig_value "$var") -eq 0 ]]; then
					i=$(($i + 3))
				fi
				continue
			fi
			name="${args[$i]} --->"
			links["$name"]="${args[$(($i + 1))]}"
			help["$name"]="${args[$(($i + 2))]}"
			dialog_flags[$j]="$name"
			i=$(($i + 2))
		done

		$DIALOG "${COMMON_FLAGS[@]}" \
			--title "ChaOS/$ARCH $1" \
			--no-items \
			--ok-label "Select" \
			--help-button \
			--default-item "$default" \
			--extra-button --extra-label "Save" \
			--menu "$MENUHELP" $HEIGHT $WIDTH 999 "${dialog_flags[@]}" \
			2> "$TEMP"

		retval=$?
		choice=$(cat -- "$TEMP")
		case $retval in
			0)
				default="$choice"
				${links["$choice"]};;
			1)
				break;;
			2)
				declare name
				declare pretty_choice

				name=$(echo "$choice" | sed -e "s/^HELP //g");
				pretty_choice=$(echo "$name" | sed -e "s/ --->$//g");
				default="$name"
				menu_help "$pretty_choice" "${help["$name"]}";;
			3)
				default="$choice"
				menu_save;;
			255)
				break;;
			*)
				cat -- "$TEMP"
				exit 1;;
		esac
	done
}

# menu_checklist <title> [<name> <var> <help>]...
function menu_checklist() {
	declare -a args=( "${@:2}" )
	declare -A kvar=()
	declare -A help=()
	declare -a dialog_flags=()
	declare default=""

	while :; do
		declare -i retval
		declare choice

		for ((i = 0; i < ${#args[*]}; i+=3)); do
			declare name

			name="${args[$i]}"
			kvar["$name"]="${args[$(($i + 1))]}"
			help["$name"]="${args[$(($i + 2))]}"
			dialog_flags[$i]="$name"
			dialog_flags[$(($i + 1))]="$name"
			dialog_flags[$(($i + 2))]=$(get_kconfig_status ${kvar["$name"]})
		done

		$DIALOG "${COMMON_FLAGS[@]}" \
			--title "ChaOS/$ARCH $1" \
			--no-tags \
			--ok-label "Select" \
			--help-status \
			--help-button \
			--default-item "$default" \
			--extra-button --extra-label "Save" \
			--checklist "$MENUHELP" $HEIGHT $WIDTH 999 "${dialog_flags[@]}" \
			2> "$TEMP"

		retval=$?
		choice=$(cat -- "$TEMP")
		case $retval in
			0|2|3)
				declare -a 'opts=('"$choice"')'
				declare -a varopts

				# Set all kconfig variables of this menu to 0
				for opt in ${kvar[@]}; do
					set_kconfig_bool "$opt" 0
				done

				# Skip first if HELP
				varopts=("${opts[@]}")
				if [[ $retval -eq 2 ]]; then
					varopts=("${opts[@]:1}")
				fi

				# Set all selected kconfig variables to 1
				for opt in "${varopts[@]}"; do
					set_kconfig_bool "${kvar[$opt]}" 1
				done

				# Do some button-specific stuff
				if [[ $retval -eq 2 ]]; then
					declare name

					name=$(echo "${opts[1]}" | sed -e "s/^HELP //g");
					default="$name"
					menu_help "$name" "${help["$name"]}" "${kvar["$name"]}"
				elif [[ $retval -eq 3 ]]; then
					menu_save
				else
					break
				fi
				;;
			1)
				break;;
			255)
				break;;
			*)
				echo "An error occured"
				exit 1;;
		esac
	done
}
