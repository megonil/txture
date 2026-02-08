#include "utils.h"

#include "format.h"

extern FormatKind fmt_kind;

const char*
_filename ()
{
	switch (fmt_kind)
		{
		case FmtPPM: return default_filename ".ppm";
		case FmtBMP: return default_filename ".bmp";
		}

	return default_filename;
}
