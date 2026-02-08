#ifndef txture_utils
#define txture_utils

#include "format.h"
#include "gen.h"

#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#define default_filename "out"
#define bmp_format ".bmp"
#define ppm_format ".ppm"

#include "flags.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	uint16_t r, g, b;
} Colors;

struct Spec
{
	GeneratorKind gen_kind;
	FormatKind	  fmt_kind;
	unsigned	  width;
	unsigned	  height;
	unsigned	  max_val;
	uint8_t		  mono;
	uint8_t		  colors_set;
	uint8_t		  random;
	Colors		  colors;
};

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

typedef enum
{
	MapNormalize,
	MapMod,
	MapChannel,
	MapPalette,
} ColorMapState;

void
mapclr (long value, unsigned x, unsigned y, Colors* out);

#undef strconv
#endif // !txture_utils
