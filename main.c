#include "flags.h"
#include "utils.h"

#include <unistd.h>

flag_t flags = 0;

void _Noreturn help ()
{
	println_exit (
		0, "Help:\n\t-p - use ppm file format for image\n\t-b - use bmp "
		   "file format for image\n\t-h - show this message");
}

int
main (int argc, char* argv[])
{
	char c;
	while ((c = getopt (argc, argv, "p::b::h::")) != -1)
		{
			switch (c)
				{
				case 'p': setflag (FlagUsePPM); break;
				case 'b': setflag (FlagUseBMP); break;
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

	if (getflag (FlagUsePPM))
		{
			unimplemented ("PPM file format");
		}

	else if (getflag (FlagUseBMP))
		{
			unimplemented ("BMP file format");
		}
	return 0;
}
