#ifndef txture_gen
#define txture_gen

#include "gen_list.h"

#define MAX_GEN_VALUE UINT64_MAX

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

double
checkerboard_generator (long x, long y);

double
stripes_generator (long x, long y);

double
diagstripes_generator (long x, long y);

double
radial_generator (long x, long y);

void
normalize (double* val);

void
normalize_wrap (double* val);

void
expand (double* val);

#define mapval(T) mapclr (val, x, y, &T (x, y));

// accepts macro to get the pixel value
// perlin pixel macro
#define perlin_pmacro(x, y, T)                                            \
	double val = (double) perlin_noise (x * 0.1, y * 0.1);                \
	normalize (&val);                                                     \
	mapval (T);

#define checkerboard_pmacro(x, y, T)                                      \
	double val = checkerboard_generator (x, y);                           \
	expand (&val);                                                        \
	mapval (T);

#define xor_pmacro(x, y, T)                                               \
	double val = xor_generator (x, y);                                    \
	expand (&val);                                                        \
	mapval (T);

#define stripes_pmacro(x, y, T)                                           \
	double val = stripes_generator (x, y);                                \
	normalize (&val);                                                     \
	mapval (T);

#define diagstripes_pmacro(x, y, T)                                       \
	double val = diagstripes_generator (x, y);                            \
	normalize (&val);                                                     \
	mapval (T);

#define radial_pmacro(x, y, T)                                            \
	double val = radial_generator (x, y);                                 \
	normalize (&val);                                                     \
	mapval (T);

#define value_pmacro(x, y, T)                                             \
	double val = value_generator (x, y);                                  \
	expand (&val);                                                        \
	mapval (T);

GeneratorKind
gen_fromstr (const char* str);

#endif // !txture_gen
