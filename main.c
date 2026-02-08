#include "flags.h"
#include "gen.h"
#include "ppm.h"
#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

flag_t flags = 0;

void _Noreturn help ()
{
	println_exit (
		0, "Help:\n\t-p - use ppm file format for image\n\t-b - use bmp "
		   "file format for image\n\t-d - debug mode(verbose)\n\t-h - "
		   "show this "
		   "message");
}

int
main (int argc, char* argv[])
{
	char c;
	while ((c = getopt (argc, argv, "p::b::d::h::")) != -1)
		{
			switch (c)
				{
				case 'p': setflag (FlagUsePPM); break;
				case 'b': setflag (FlagUseBMP); break;
				case 'd': setflag (FlagDebug); break;
				case 'h': help ();
				}
		}

	const char* filename;
	if (optind != argc)
		{
			filename = argv[optind];
		}
	else
		{
			filename = _filename ();
		}

	// open the actual file
	// w+ means open for write and reading and if file does not exist -
	// create it
	// b - means binary
	FILE* file = fopen (filename, "w+b");
	if (file == NULL)
		{
			perror ("error");
			return 1;
		}

	if (getflag (FlagUsePPM))
		{
			// just defaults
			int		width	= 400;
			int		height	= 400;
			int		max_val = 255;
			PPMMode mode	= P6;

			PPMImage* img = ppm_aimage (width, height, max_val, mode);
			debugln ("Created image.");
			// for now use the perlin noise generator
			ppm_forpixels (img, perlin_pmacro);

			ppm_wimage (img, file);
			ppm_fimage (img);
		}

	else if (getflag (FlagUseBMP))
		{
			unimplemented ("BMP file format");
		}

	fclose (file);
	return 0;
}
