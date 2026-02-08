#include "flags.h"
#include "format.h"
#include "gen.h"
#include "ppm.h"
#include "process.h"
#include "utils.h"

#include <argp.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

flag_t flags = 0;

const char* filename = NULL;

struct Spec spec = {.colors		= {0, 0, 0},
					.colors_set = 0,
					.mono		= 0,
					.fmt_kind	= FmtPPM,
					.gen_kind	= GenPerlin,
					.max_val	= UINT16_MAX,
					.width		= 400,
					.height		= 400};

#define KEY_BMP 512
#define KEY_PPM 513
#define KEY_MONO 514
#define KEY_RANDOM 515

#define save_color(key, color)                                            \
	case key:                                                             \
		spec.colors.color = str2umax (arg, 10);                           \
		spec.colors_set	  = 1;                                            \
		break;

static int
parse_opt (int key, char* arg, struct argp_state* state)
{
	switch (key)
		{
		case KEY_PPM: spec.fmt_kind = FmtPPM; break;
		case KEY_BMP: spec.fmt_kind = FmtBMP; break;

		case 'd': setflag (FlagDebug); break;
		case 's': filename = arg; break;

		case 'h': spec.height = str2umax (arg, 10); break;
		case 'w': spec.width = str2umax (arg, 10); break;
		case KEY_RANDOM: spec.random = 1; break;
		case KEY_MONO:
			spec.mono		= 1;
			spec.colors_set = 0;
			break;

			// colors
			save_color ('r', r);
			save_color ('g', g);
			save_color ('b', b);

		case ARGP_KEY_ARG: // program argument
			{
				spec.gen_kind = gen_fromstr (arg);
			}
		}

	return 0;
}

#undef save_color

#define GROUP_GENS 0
#define GROUP_FORMATS 1
#define GROUP_FILE_OPT 2

static struct argp_option options[]
	= {{.name  = "debug",
		.key   = 'd',
		.arg   = 0,
		.flags = 0,
		"Enable debug mode(verbose)"},
	   {.name  = "ppm",
		.key   = KEY_PPM,
		.arg   = 0,
		.flags = 0,
		.doc   = "Use ppm file format",
		.group = GROUP_FORMATS},
	   {.name  = "bmp",
		.key   = KEY_BMP,
		.arg   = 0,
		.flags = 0,
		.doc   = "Use bmp file format",
		.group = GROUP_FORMATS},
	   {.name  = "save",
		.key   = 's',
		.arg   = "FILE",
		.flags = 0,
		.group = GROUP_FILE_OPT,
		.doc = "Specify to which file save, defaults to out.<fileformat>"},
	   {.name  = "height",
		.key   = 'h',
		.arg   = "NUM",
		.flags = 0,
		.doc   = "Specify the height of the image, defaults to 400",
		.group = GROUP_FILE_OPT},
	   {.name  = "width",
		.key   = 'w',
		.arg   = "NUM",
		.flags = 0,
		.doc   = "Specify the width of the image, defaults to 400",
		.group = GROUP_FILE_OPT},
	   {.name  = "red",
		.key   = 'r',
		.arg   = "NUM",
		.flags = 0,
		.doc   = "Specify base of red color to use"},
	   {.name  = "green",
		.key   = 'g',
		.arg   = "NUM",
		.flags = 0,
		.doc   = "Specify base of green color to use"},
	   {.name  = "blue",
		.key   = 'b',
		.arg   = "NUM",
		.flags = 0,
		.doc   = "Specify base of green color to use"},
	   {.name  = "max",
		.key   = 'x',
		.arg   = "NUM",
		.flags = 0,
		.doc   = "Specify max colors quantity"},
	   {.name  = "mono",
		.key   = KEY_MONO,
		.arg   = 0,
		.flags = 0,
		.doc   = "Do not use any color palette"},
	   {.name  = "random",
		.key   = KEY_RANDOM,
		.arg   = 0,
		.flags = 0,
		.doc   = "Use random for every color(maybe laggy)"}};

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

	switch (spec.fmt_kind)
		{
		case FmtPPM: process_ppm (file, P6, spec.max_val); break;
		case FmtBMP: unimplemented ("BMP file format");
		}

	fclose (file);
	return 0;
}
