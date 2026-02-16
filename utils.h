#ifndef txture_utils
#define txture_utils

#include "format.h"
#include "gen.h"

#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define default_filename "out"
#define bmp_format		 ".bmp"
#define ppm_format		 ".ppm"

#include "flags.h"

typedef enum
{
	ModeUnset,
	ModeMonochrome,
	ModeRandom,
	ModeColors,
	ModeColorsExprs,
	ModeExprs,
} ColorMode;

typedef struct
{
	uint16_t r, g, b;
} Colors;

struct Spec
{
	Colors		  colors;
	const char*	  rexpr;
	const char*	  gexpr;
	const char*	  bexpr;
	const char*	  vexpr;
	unsigned	  width;
	unsigned	  height;
	unsigned	  max_val;
	GeneratorKind gen_kind;
	FormatKind	  fmt_kind;
	ColorMode	  clrmode;
};

#define setflag(fl) flags |= (1 << fl)
#define getflag(fl) (flags & (1 << fl))

ext_flags ();

const char*
_filename ();

// enclose string in delimiters, only for literals
#define _enclose(str)	  "\"" str "\""

#define println(fmt, ...) printf (fmt "\n", ##__VA_ARGS__)
#define println_exit(code, fmt, ...)                                      \
	println (fmt, ##__VA_ARGS__);                                         \
	exit (code)

#define debugln(fmt, ...)                                                 \
	if (getflag (FlagDebug))                                              \
	println (fmt, ##__VA_ARGS__)

#define debug(fmt, ...)                                                   \
	if (getflag (FlagDebug))                                              \
	printf (fmt, ##__VA_ARGS__)

#define error(fmt, ...)	   println_exit (1, "error: " fmt, ##__VA_ARGS__)
#define lnerror(fmt, ...)  println_exit (1, "\nerror: " fmt, ##__VA_ARGS__)
#define unimplemented(str) error (str " is not currently implemented!")

#define warn(fmt, ...)	   println ("warning: " fmt, ##__VA_ARGS__)

#define check_str(a, b)	   (strcmp (a, b) == 0)

#define strconv()                                                         \
	perror ("error: string to number convertion");                        \
	exit (1);

uint
str2umax (char* s, int base);

void
mapclr (double value, unsigned x, unsigned y, Colors* out);

#endif // !txture_utils
