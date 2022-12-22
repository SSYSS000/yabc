#!/bin/env bash

YABC_EXECUTABLE=${1:-./yabc}

COLOR_FAIL="\033[0;31m"
COLOR_SUCCESS="\033[0;32m"
CLEAR_COLOR="\033[m"

if [ ! -f "$YABC_EXECUTABLE" ]; then
	echo "No executable."
	exit 1
fi

TEST_COUNTER=0
function runtest {
	local YABC_ARGS=
	local EXPECTED_OUTPUT=
	local EXPECTED_SUCCESS=true
	local OPTIND
	
	while getopts "a:fso:" opt; do
		case $opt in
		a) YABC_ARGS="$OPTARG" ;;
		o) EXPECTED_OUTPUT="$OPTARG" ;;
		f) EXPECTED_SUCCESS=false ;;
		s) EXPECTED_SUCCESS=true ;;
		esac
	done

	shift $((OPTIND - 1))
	
	local TEST_NO=$TEST_COUNTER
	((TEST_COUNTER++))
	
	local CMD="$YABC_EXECUTABLE $YABC_ARGS"
	
	echo "Test #$TEST_NO: $CMD"
	local CMD_OUTPUT
	local EXIT_CODE
	
	CMD_OUTPUT=$($CMD 2>&1)
	EXIT_CODE=$?
	
	echo -n "Exit status: "
	
	if [[ $EXIT_CODE -eq 0 && "$EXPECTED_SUCCESS" = true ||
		  $EXIT_CODE -ne 0 && "$EXPECTED_SUCCESS" = false ]]; then
		echo -ne "${COLOR_SUCCESS}"
	else
		echo -ne "${COLOR_FAIL}"
	fi
	
	if [ $EXIT_CODE -eq 0 ]; then
		echo -n "success"
	else
		echo -n "fail"
	fi
	
	echo -e "${CLEAR_COLOR}"
	
	if [ -n "$EXPECTED_OUTPUT" ]; then
		echo -n "Output: "
        if [ "$CMD_OUTPUT" = "$EXPECTED_OUTPUT" ]; then
            echo -e "${COLOR_SUCCESS}${CMD_OUTPUT}${CLEAR_COLOR}"
        else
            echo -ne "${COLOR_FAIL}${CMD_OUTPUT}${CLEAR_COLOR} (expected $EXPECTED_OUTPUT)"
        fi
	fi
	
	echo ""
}


runtest -s -a "bin hex 1101111" -o 6f
runtest -f -a "bin bin 1101211"
runtest -f -a "bgin hex 1101211"
runtest -s -a "2 2 11011011001" -o 11011011001
runtest -s -a "3 3 21101120110021" -o 21101120110021
runtest -s -a "4 4 123000" -o 123000
runtest -s -a "32 32 avavava" -o avavava
