#include "process.h"

#include "expr/parser.h"
#include "gen.h"
#include "ppm.h"
#include "utils.h"

extern const char* filename;
extern struct Spec spec;

extern Parser r_parser;
extern Parser g_parser;
extern Parser b_parser;

#define process_end()                                                     \
	if (spec.clrmode == ModeColorsExprs || spec.clrmode == ModeExprs)     \
		{                                                                 \
			parser_destroy (&r_parser);                                   \
			parser_destroy (&g_parser);                                   \
			parser_destroy (&b_parser);                                   \
		}

void
process_ppm (FILE* file, PPMMode mode, unsigned max_val)
{
	PPMImage* img = ppm_aimage (spec.width, spec.height, max_val, mode);

	process_debug (filename, spec.width, spec.height);

	switch (spec.gen_kind)
		{
		case GenPerlin: ppm_forpixels (img, perlin_pmacro); break;
		case GenXor: ppm_forpixels (img, xor_pmacro); break;
		case GenValue: ppm_forpixels (img, value_pmacro); break;
		default: __builtin_unreachable ();
		}

	ppm_wimage (img, file);
	ppm_fimage (img);
	process_end ();
}
