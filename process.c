#include "process.h"

#include "gen.h"
#include "ppm.h"

extern GeneratorKind gen_kind;
extern const char*	 filename;
extern unsigned		 width;
extern unsigned		 height;

void
process_ppm (FILE* file, PPMMode mode, unsigned max_val)
{
	PPMImage* img = ppm_aimage (width, height, max_val, mode);

	process_debug (filename, width, height);

	switch (gen_kind)
		{
		case GenPerlin: ppm_forpixels (img, perlin_pmacro); break;
		case GenXor: ppm_forpixels (img, xor_pmacro); break;
		}

	ppm_wimage (img, file);
	ppm_fimage (img);
}
