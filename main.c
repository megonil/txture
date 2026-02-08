#include "flags.h"
#include "format.h"
#include "gen.h"
#include "ppm.h"
#include "process.h"
#include "utils.h"

#include <argp.h>
#include <stdio.h>
#include <unistd.h>

flag_t		  flags	   = 0;
GeneratorKind gen_kind = GenPerlin;
FormatKind	  fmt_kind = FmtPPM;
const char*	  filename = NULL;
unsigned	  width	   = 400;
unsigned	  height   = 400;
// arithmetic operation to use
char operator_mode = '%';

static int
parse_opt (int key, char* arg, struct argp_state* state)
{
	switch (key)
		{
		case 'd': setflag (FlagDebug); break;
		case 'p': fmt_kind = FmtPPM; break;
		case 'b': fmt_kind = FmtBMP; break;
		case 's': filename = arg; break;
		case 'h': height = str2umax (arg, 10); break;
		case 'w': width = str2umax (arg, 10); break;
		case ARGP_KEY_ARG: // program argument
			{
				gen_kind = gen_fromstr (arg);
			}
		}

	return 0;
}

#define GROUP_GENS 0
#define GROUP_FORMATS 1
#define GROUP_FILE_OPT 2

static struct argp_option options[] = {
	{.name	= "debug",
	 .key	= 'd',
	 .arg	= 0,
	 .flags = 0,
	 "Enable debug mode(verbose)"},
	{.name	= "ppm",
	 .key	= 'p',
	 .arg	= 0,
	 .flags = 0,
	 .doc	= "Use ppm file format",
	 .group = GROUP_FORMATS},
	{.name	= "bmp",
	 .key	= 'b',
	 .arg	= 0,
	 .flags = 0,
	 .doc	= "Use bmp file format",
	 .group = GROUP_FORMATS},
	{.name	= "save",
	 .key	= 's',
	 .arg	= "FILE",
	 .flags = 0,
	 .group = GROUP_FILE_OPT,
	 .doc	= "Specify to which file save, defaults to out.<fileformat>"},
	{.name	= "height",
	 .key	= 'h',
	 .arg	= "NUM",
	 .flags = 0,
	 .doc	= "Specify the height of the image, defaults to 400",
	 .group = GROUP_FILE_OPT},
	{.name	= "width",
	 .key	= 'w',
	 .arg	= "NUM",
	 .flags = 0,
	 .doc	= "Specify the width of the image, defaults to 400",
	 .group = GROUP_FILE_OPT},
};

static struct argp argp = {options, parse_opt};

int
main (int argc, char* argv[])
{
	argp_parse (&argp, argc, argv, 0, 0, 0);

	if (filename == NULL)
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

	switch (fmt_kind)
		{
		case FmtPPM: process_ppm (file, P6, ppm_default_max_val); break;
		case FmtBMP: unimplemented ("BMP file format");
		}

	fclose (file);
	return 0;
}
