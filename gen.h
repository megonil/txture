#ifndef txture_gen
#define txture_gen

/// X and Y are the pixel position.
/// returns some value
float
perlin_noise (float x, float y);

// accepts macro to get the pixel value
// perlin pixel macro
#define perlin_pmacro(x, y, T)                                            \
	float n = perlin_noise (x * 0.1, y * 0.1);                            \
                                                                          \
	n += 1.0;                                                             \
	n /= 2.0;                                                             \
                                                                          \
	int c	   = round (n * max_val);                                     \
	T (x, y).r = c;                                                       \
	T (x, y).g = c;                                                       \
	T (x, y).b = c;

#endif // !txture_gen
