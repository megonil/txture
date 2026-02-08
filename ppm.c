#include "ppm.h"

#include <stdint.h>
#include <stdlib.h>

#define ppm_mode2str(mode) mode == P3 ? "P3" : "P6"
#define bytes_per_pixel(m) (m > 256 ? 2 : 1) // ppm spec

#define sizeof_image_struct sizeof (PPMImage)
#define n_pixels(w, h) w* h
#define sizeof_pixels(w, h, m) n_pixels (w, h) * bytes_per_pixel (m) / 8
#define sizeof_row(w, m) bytes_per_pixel (m) * w / 8

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

	img->data = (PPMPixel**) (img + 1); // set the pointer to the end of
										// PPMImage struct

	for (int y = 0; y < img->header.rows; ++y)
		{
			img->data[y] = calloc (img->header.cols, sizeof (PPMPixel));
		}
	return img;
}

static void
write_header (PPMHeader* header, FILE* file)
{
	fprintf (file, "%s\n%u %u\n%u\n", ppm_mode2str (header->mode),
			 header->rows, header->cols, header->max_val);
}

int
ppm_wimage (PPMImage* image, FILE* file)
{
	write_header (&image->header, file);
	uint8_t bytes_per = bytes_per_pixel (image->header.max_val);
	for (int y = 0; y < image->header.rows; ++y)
		{
			for (int x = 0; x < image->header.cols; ++x)
				{
					// TODO:
					// generic struct pixels
					// safe write for any endiannes
					fwrite (&(image->data[y][x]).r, bytes_per, 1, file);
					fwrite (&(image->data[y][x]).g, bytes_per, 1, file);
					fwrite (&(image->data[y][x]).b, bytes_per, 1, file);
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
