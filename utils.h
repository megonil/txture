#ifndef txture_utils
#define txture_utils

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#define default_filename "out"
#define bmp_format ".bmp"
#define ppm_format ".ppm"

#include "flags.h"

#include <stdio.h>
#include <stdlib.h>

#define setflag(fl) flags |= (1 << fl)
#define getflag(fl) (flags & (1 << fl))

ext_flags ();

const char*
_filename ();

// enclose string in delimiters, only for literals
#define _enclose(str) "\"" str "\""

#define println(fmt, ...) printf (fmt "\n", ##__VA_ARGS__)
#define println_exit(code, fmt, ...)                                      \
	println (fmt, ##__VA_ARGS__);                                         \
	exit (code)

#define debugln(fmt, ...)                                                 \
	if (getflag (FlagDebug))                                              \
	println (fmt, ##__VA_ARGS__)

#define error(fmt, ...) println_exit (1, "error: " fmt, ##__VA_ARGS__)
#define unimplemented(str) error (str " is not currently implemented!")

#define strconv()                                                         \
	perror ("error: string to number convertion");                        \
	exit (1);

inline uint
str2umax (char* s, int base)
{
	char* end;
	errno  = 0;
	long n = strtol (s, &end, base);
	if (end == s)
		{
			strconv ();
		}
	if ((errno == ERANGE && n == LONG_MIN))
		{
			strconv ();
		}
	if ((errno == ERANGE && n == LONG_MAX))
		{
			strconv ();
		}
	if (n < 0)
		{
			error ("expected positive number, got \"%s\"", s);
		}

	if (n > UINT_MAX)
		{
			error ("too big number \"%s\"!", s);
		}

	return n;
}

#undef strconv
#endif // !txture_utils
