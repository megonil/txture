#include "utils.h"

const char*
_filename ()
{
	if (getflag (FlagUseBMP))
		{
			return default_filename bmp_format;
		}

	else if (getflag (FlagUsePPM))
		{
			return default_filename ppm_format;
		}

	return default_filename;
}
