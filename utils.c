#include "utils.h"

#include "format.h"
#include "gen.h"

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

void
mapclr (long value, unsigned x, unsigned y, Colors* out)
{
	if (spec.mono)
		{
			uint16_t n_val = (value * spec.max_val) / MAX_GEN_VALUE;

			out->r = out->g = out->b = (uint16_t) n_val;
		}

	else if (spec.colors_set)
		{
			out->r = lmod (value, spec.colors.r);
			out->g = lmod (value, spec.colors.g);
			out->b = lmod (value, spec.colors.b);
		}

	else if (spec.random)
		{
			out->r = lmod (value, random () % spec.max_val);
			out->g = lmod (value, random () % spec.max_val);
			out->b = lmod (value, random () % spec.max_val);
		}

	else
		{
			out->r = value * x * 255 / UINT16_MAX;
			out->g = value * y * 255 / UINT16_MAX;
			out->b = value * (x + y) * 255 / UINT16_MAX;
		}
}
