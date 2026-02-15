#ifndef txture_gen
#define txture_gen

#include "expr/parser.h"
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

double
value_generator (long x, long y);

void
normalize (double* val);

// accepts macro to get the pixel value
// perlin pixel macro
#define perlin_pmacro(x, y, T)                                            \
	double n = (double) perlin_noise (x * 0.1, y * 0.1);                  \
	normalize (&n);                                                       \
	mapclr (n, x, y, &T (x, y));

#define xor_pmacro(x, y, T)                                               \
	double val = xor_generator (x, y);                                    \
	normalize (&val);                                                     \
	mapclr (val, x, y, &T (x, y));

#define value_pmacro(x, y, T)                                             \
	double val = value_generator (x, y);                                  \
	normalize (&val);                                                     \
	mapclr (val, x, y, &T (x, y));

GeneratorKind
gen_fromstr (const char* str);

#endif // !txture_gen
