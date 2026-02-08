#ifndef txture_gen
#define txture_gen

#include "gen_list.h"

#define MAX_GEN_VALUE UINT64_MAX

#include <math.h>

typedef enum
{
#define T(variant, str) variant,
	GEN_LIST (T)
#undef T
} GeneratorKind;
/// X and Y are the pixel position.
/// returns some value
float
perlin_noise (float x, float y);

long
xor_generator (long x, long y);

// accepts macro to get the pixel value
// perlin pixel macro
#define perlin_pmacro(x, y, T)                                            \
	float n = perlin_noise (x * 0.1, y * 0.1);                            \
                                                                          \
	n += 1.0;                                                             \
	n /= 2.0;                                                             \
                                                                          \
	long c = roundl (n * max_val);                                        \
	mapclr (c, x, y, &T (x, y));

#define xor_pmacro(x, y, T)                                               \
	long val = xor_generator (x, y);                                      \
	mapclr (val, x, y, &T (x, y));

// xor pixel colorized macro
#define xor_pcmaro()

GeneratorKind
gen_fromstr (const char* str);

#endif // !txture_gen
