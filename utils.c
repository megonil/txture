#include "utils.h"

#include "expr/parser.h"
#include "format.h"
#include "gen.h"

#include <errno.h>
#include <stdint.h>

extern struct Spec spec;

static inline long
lmod (long a, long m)
{
	if (m <= 0)
		{
			return a;
		}

	long r = a % m;
	if (r < 0)
		{
			r += m;
		}

	return r;
};

inline const char*
_filename ()
{
	switch (spec.fmt_kind)
		{
		case FmtPPM: return default_filename ".ppm";
		case FmtBMP: return default_filename ".bmp";
		}

	return default_filename;
}

extern Parser r_parser;
extern Parser g_parser;
extern Parser b_parser;

void
mapclr (double value, unsigned x, unsigned y, Colors* out)
{
	uint16_t n_val = (uint16_t) value;
	switch (spec.clrmode)
		{
		case ModeUnset:
		case ModeMonochrome:
			{
				out->r = out->g = out->b = n_val;
				break;
			}
		case ModeRandom:
			{
				out->r = lmod (n_val, random () * spec.max_val);
				out->g = lmod (n_val, random () * spec.max_val);
				out->b = lmod (n_val, random () * spec.max_val);
				break;
			}
		case ModeColors:
			{
				out->r = lmod (n_val, spec.colors.r);
				out->g = lmod (n_val, spec.colors.g);
				out->b = lmod (n_val, spec.colors.b);
				break;
			}
		case ModeColorsExprs:
			{
				if (!r_parser.inited || !g_parser.inited
					|| !b_parser.inited)
					{
						error ("expected to have exprs for all colors!");
					}

				out->r = lmod (expr (&r_parser, x, y, value, spec.max_val,
									 spec.rexpr),
							   spec.colors.r);
				out->g = lmod (expr (&g_parser, x, y, value, spec.max_val,
									 spec.gexpr),
							   spec.colors.g);
				out->b = lmod (expr (&b_parser, x, y, value, spec.max_val,
									 spec.bexpr),
							   spec.colors.b);
				break;
			}
		case ModeExprs:
			{
				if (!r_parser.inited || !g_parser.inited
					|| !b_parser.inited)
					{
						error ("expected to have exprs for all colors!");
					}

				out->r = expr (&r_parser, x, y, value, spec.max_val,
							   spec.rexpr);
				out->g = expr (&g_parser, x, y, value, spec.max_val,
							   spec.gexpr);
				out->b = expr (&b_parser, x, y, value, spec.max_val,
							   spec.bexpr);
				break;
			}
		default: __builtin_unreachable ();
		}
}

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
