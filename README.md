# yabc
YABC (Yet Another Base Converter) is a simple command line base converter for Unix-like systems.

## Build and install instructions
Run `make` or `cc -O3 -o yabc yabc.c` to build yabc.

Install yabc with `make install PREFIX=/usr/local`
to /usr/local/bin. Uninstall with `make uninstall PREFIX=/usr/local`.

## How to use
yabc requires the number that must be converted, the current base of the number and the base that the number will be converted into.

Usage:
`$ ./yabc FROM_BASE TO_BASE TARGET`

where `FROM_BASE` is the input base, `TO_BASE` is the desired output base and `TARGET` is a number that must be converted.

`FROM_BASE` and `TO_BASE` must be one of the following:
 - "hex" for base 16,
 - "dec" for base 10,
 - "oct" for base 8,
 - any number between 2 and 36 inclusive.

`TARGET` must be a valid non-negative integer.

