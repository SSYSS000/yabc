# yabc
yabc (Yet Another Base Converter) is a base converter developed for GNU/Linux.

## Build instructions
Enter the project root directory and run `make`.

## How to use
yabc requires the number that must be converted, the current base of the number and the base that the number will be converted into.

Usage:
`$ yabc FROM_BASE TO_BASE TARGET`

where `FROM_BASE` is the input base, `TO_BASE` is the desired output base and `TARGET` is a number that must be converted.

`FROM_BASE` and `TO_BASE` must be one of the following:
 - "hex" for base 16,
 - "dec" for base 10,
 - "oct" for base 8,
 - any number between 2 and 36 inclusive.

`TARGET` must be a valid non-negative integer.

