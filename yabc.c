/*
 zlib License

 Copyright (c) 2021 SSYSS000

 This software is provided 'as-is', without any express or implied
 warranty. In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.

 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.

 3. This notice may not be removed or altered from any source distribution.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <inttypes.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>

#if 0
#include <libintl.h>
#include <locale.h>
#endif

#define YABC_VERSION "1.3"
#define MIN_RADIX 2
#define MAX_RADIX 36

#define _(s) (s)

struct arguments {
	int from_base;
	int to_base;
	bool uppercase_output;
	uintmax_t conv_number;
};

/*
 * Parse a string _tip_ into an int that represents a base.
 * _tip_ can be numeric or an abbreviation of a numeral system.
 * If _tip_ is numeric, it is converted into an int and returned.
 * If _tip_ is an abbreviation of one of the recognized numeral systems,
 * the number of digits in the system is returned.
 * Recognized numeral systems with their abbreviations:
 * hexadecimal - hex,
 * decimal - dec,
 * octal - oct,
 * binary - bin
 * If the base cannot be determined or is out of range, -1 is returned.
 */
int parse_base(const char *tip)
{
	char *endptr;
	long parsed = strtol(tip, &endptr, 0);

	if (*endptr == '\0') {
		if (2 <= parsed && parsed <= INT_MAX)
			return parsed;
	}
	else {
		/*
		 * Tip wasn't numeric, so assume it's
		 * the name of a numeral system.
		 */
		if (!strcasecmp(tip, "hex"))
			return 16;

		if (!strcasecmp(tip, "dec"))
			return 10;

		if (!strcasecmp(tip, "oct"))
			return 8;

		if (!strcasecmp(tip, "bin"))
			return 2;
	}

	return -1;
}

/*
 * Print usage instructions.
 * _prog_name is the program name.
 */
void print_usage(const char *prog_name)
{
	fprintf(stderr,
		_("Usage: %s [options] from_base to_base target\n"
		"         where from_base is the base of target,\n"
		"         to_base is the base of the result,\n"
		"         target is an integer to be converted\n"
		"Options:\n"
		" -h\tdisplay this information\n"
		" -l\tlowercase result\n"
		" -u\tuppercase result\n"
		" -v\tdisplay version\n"), prog_name);
}

bool is_supported_base(int base)
{
	return MIN_RADIX <= base && base <= MAX_RADIX;
}

/*
 * Scan process arguments. If the arguments contain
 * illegal values, an error message shall be printed
 * and the program shall terminate using exit().
 * argc is the amount of arguments in argv and
 * argv is an array containing the arguments
 * (with the first argument being the program name).
 */
struct arguments scan_args(int argc, char **argv)
{
	int opt;
	struct arguments args = {0};

	while ((opt = getopt(argc, argv, "hluv")) != -1) {
		switch (opt) {
		case 'h':
			print_usage(argv[0]);
			exit(EXIT_SUCCESS);
			break;
		case 'l': args.uppercase_output = false; break;
		case 'u': args.uppercase_output = true; break;
		case 'v':
			fprintf(stderr, "YABC v%s\n", YABC_VERSION);
			exit(EXIT_SUCCESS);
			break;
		default:
			fputs(_("Unrecognized option.\n"), stderr);
			exit(EXIT_FAILURE);
		}
	}

	if ((argc - (optind - 1)) < 4) {
		print_usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	const char *arg_from_base = argv[optind];
	const char *arg_to_base = argv[optind + 1];
	const char *arg_number = argv[optind + 2];

	if ((args.from_base = parse_base(arg_from_base)) == -1) {
		fprintf(stderr, _("Unrecognized base: %s\n"), arg_from_base);
		exit(EXIT_FAILURE);
	}

	if (!is_supported_base(args.from_base)) {
		fprintf(stderr, _("Unsupported base: %s\n"), arg_from_base);
		exit(EXIT_FAILURE);
	}

	if ((args.to_base = parse_base(arg_to_base)) == -1) {
		fprintf(stderr, _("Unrecognized base: %s\n"), arg_to_base);
		exit(EXIT_FAILURE);
	}

	if (!is_supported_base(args.to_base)) {
		fprintf(stderr, _("Unsupported base: %s\n"), arg_to_base);
		exit(EXIT_FAILURE);
	}

	char *endptr;
	errno = 0;
	args.conv_number = strtoumax(arg_number, &endptr, args.from_base);

	if (*endptr != '\0') {
		fprintf(stderr, _("Target not in base %s.\n"), arg_from_base);
		exit(EXIT_FAILURE);
	}

	if (args.conv_number == UINTMAX_MAX && errno == ERANGE) {
		fprintf(stderr, _("Target out of range.\n"));
		exit(EXIT_FAILURE);
	}

	return args;
}

/*
 * Print a number in any base between
 * MIN_RADIX and MAX_RADIX inclusive.
 * Output shall be in uppercase if 'uppercase' is true.
 * Otherwise, output will be in lowercase.
 * Thread-Safe.
 */
void print_in_base(int base, uintmax_t num, bool uppercase)
{
	assert(is_supported_base(base));

	// 'buffer' must hold at least as many characters as there
	// are bits needed to represent the value of 'num'.
	char buffer[CHAR_BIT * sizeof(num)];
	char *seq_start;
	char *seq_end;
	char letter_a_as_10 = uppercase ? 'A' : 'a';

	/*
	 * 'seq_start' points to the first char of the
	 * character sequence that represents the converted
	 * value in 'buffer'.
	 */
	seq_end = seq_start = buffer + sizeof(buffer) - 1;

	do {
		--seq_start;

		int remainder = num % base;
		num /= base;

		if (remainder < 10) {
			*seq_start = '0' + remainder;
		}
		else {
			*seq_start = letter_a_as_10 + remainder - 10;
		}
	} while (num > 0);

	fwrite(seq_start, seq_end - seq_start, 1, stdout);
}

int main(int argc, char **argv)
{
#if 0
	setlocale(LC_ALL, "");
	bindtextdomain("yabc", LOCALE_DIR);
	textdomain("yabc");
#endif
	struct arguments args = scan_args(argc, argv);
	print_in_base(args.to_base, args.conv_number, args.uppercase_output);
	putchar('\n');
	return EXIT_SUCCESS;
}
