#ifndef ture_ppm
#define ture_ppm

#include <stdint.h>
#include <stdio.h>
typedef enum
{
	P3,
	P6
} PPMMode;

typedef struct
{
	uint16_t r, g, b; // simple
} PPMPixel;

typedef struct
{
	unsigned int rows; // width
	unsigned int cols; // height
	unsigned int max_val;
	PPMMode		 mode;
} PPMHeader;

typedef struct
{
	PPMHeader  header;
	PPMPixel** data;
} PPMImage;

/// Allocate ppm image
PPMImage*
ppm_aimage (int width, int height, int max_val, PPMMode mode);

/// Write ppm image to file
int
ppm_wimage (PPMImage* ppm, FILE* file);

/// Free image
void
ppm_fimage (PPMImage* ppm);

#define ppm_getpixel(x, y) img->data[y][x]

/// accepts some macros(or function) T(x, y)
#define ppm_forpixels(img, T)                                             \
	for (int y = 0; y < img->header.rows; ++y)                            \
		{                                                                 \
			for (int x = 0; x < img->header.cols; ++x)                    \
				{                                                         \
					T (x, y, ppm_getpixel)                                \
				}                                                         \
		}

#define ppm_default_max_val 255

#endif // !ture_ppm
