#ifndef txture_utils
#define txture_utils

#define default_filename "out"
#define bmp_format ".bmp"
#define ppm_format ".ppm"

#include "flags.h"

#include <stdio.h>
#include <stdlib.h>

#define setflag(fl) flags |= (1 << fl)
#define getflag(fl) (flags & (1 << fl))

ext_flags ();

inline const char*
_filename ()
{
	if (getflag (FlagUseBMP))
		{
			return default_filename bmp_format;
		}

	else if (getflag (FlagUsePPM))
		{
			return default_filename ppm_format;
		}

	return default_filename;
}

// enclose string in delimiters, only for literals
#define _enclose(str) "\"" str "\""

#define println(fmt, ...) printf (fmt "\n", ##__VA_ARGS__)
#define println_exit(code, fmt, ...)                                      \
	println (fmt, ##__VA_ARGS__);                                         \
	exit (code)

#define error(fmt, ...) println_exit (1, "error: " fmt, ##__VA_ARGS__)
#define unimplemented(str) error (str " is not currently implemented!")

#endif // !txture_utils
