#include "ppm.h"

#include "utils.h"

#include <stdint.h>
#include <stdlib.h>

#define ppm_mode2str(mode) mode == P3 ? "P3" : "P6"
#define bytes_per_pixel(m) (m > 256 ? 2 : 1) // ppm spec

#define sizeof_image_struct sizeof (PPMImage)
#define n_pixels(w, h) w* h
#define sizeof_pixels(w, h, m) n_pixels (w, h) * bytes_per_pixel (m) * 3
#define sizeof_row(w, m) bytes_per_pixel (m) * w * 3

PPMImage*
ppm_aimage (int width, int height, int max_val, PPMMode mode)
{
	// use the fam allocation technique
	// so we end with 1 allocation instead of 2
	PPMImage* img = (PPMImage*) malloc (
		sizeof_image_struct + sizeof_pixels (width, height, max_val));
	img->header.max_val = max_val;
	img->header.cols	= width;
	img->header.rows	= height;
	img->header.mode	= mode;

	img->data = (Colors**) (img + 1); // set the pointer to the end of
									  // PPMImage struct

	for (int y = 0; y < img->header.rows; ++y)
		{
			img->data[y] = calloc (img->header.cols, sizeof (Colors));
		}
	return img;
}

static void
write_header (PPMHeader* header, FILE* file)
{
	fprintf (file, "%s\n%u %u\n%u\n", ppm_mode2str (header->mode),
			 header->cols, header->rows, header->max_val - 1);
}

static inline void
ppm_write_sample (uint16_t v, FILE* file, uint16_t max_val)
{
	if (max_val < 256)
		{
			uint8_t b = (uint8_t) v;
			fwrite (&b, 1, 1, file);
		}

	else
		{
			uint8_t high = (v >> 8) & 0xFF;
			uint8_t low	 = v & 0xFF;
			fwrite (&high, 1, 1, file);
			fwrite (&low, 1, 1, file);
		}
}

int
ppm_wimage (PPMImage* image, FILE* file)
{
	write_header (&image->header, file);
	for (int y = 0; y < image->header.rows; ++y)
		{
			for (int x = 0; x < image->header.cols; ++x)
				{
					// TODO:
					// generic struct pixels
					// safe write for any endiannes
					ppm_write_sample (image->data[y][x].r, file,
									  image->header.max_val);
					ppm_write_sample (image->data[y][x].g, file,
									  image->header.max_val);
					ppm_write_sample (image->data[y][x].b, file,
									  image->header.max_val);
				}
		}

	return 1;
}

void
ppm_fimage (PPMImage* ppm)
{
	for (int y = 0; y < ppm->header.rows; ++y)
		{
			free (ppm->data[y]);
		}

	free (ppm);
}
