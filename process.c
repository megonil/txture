#include "process.h"

#include "gen.h"
#include "ppm.h"
#include "utils.h"

extern const char* filename;
extern struct Spec spec;

void
process_ppm (FILE* file, PPMMode mode, unsigned max_val)
{
	PPMImage* img = ppm_aimage (spec.width, spec.height, max_val, mode);

	process_debug (filename, spec.width, spec.height);

	switch (spec.gen_kind)
		{
		case GenPerlin: ppm_forpixels (img, perlin_pmacro); break;
		case GenXor: ppm_forpixels (img, xor_pmacro); break;
		}

	ppm_wimage (img, file);
	ppm_fimage (img);
}
