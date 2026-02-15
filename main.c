#include "expr/parser.h"
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

def_parser (r_parser);
def_parser (g_parser);
def_parser (b_parser);
def_parser (v_parser);

struct Spec spec = {.colors	  = {0, 0, 0},
					.fmt_kind = FmtPPM,
					.gen_kind = GenPerlin,
					.max_val  = UINT16_MAX,
					.width	  = 400,
					.height	  = 400,
					.rexpr	  = NULL,
					.gexpr	  = NULL,
					.bexpr	  = NULL,
					.vexpr	  = NULL,
					.clrmode  = ModeUnset};

#define KEY_BMP 512
#define KEY_PPM 513
#define KEY_MONO 514
#define KEY_RANDOM 515
#define KEY_REXPR 516
#define KEY_GEXPR 517
#define KEY_BEXPR 518

#define set_clrmode(mode)                                                 \
	if (spec.clrmode != ModeUnset)                                        \
		{                                                                 \
			warn ("resetting mode!");                                     \
		}                                                                 \
	spec.clrmode = mode

#define set_rgbmode(field)                                                \
	if (spec.clrmode == ModeColors)                                       \
		{                                                                 \
			spec.clrmode = ModeColorsExprs;                               \
		}                                                                 \
	else                                                                  \
		{                                                                 \
			spec.clrmode = ModeExprs;                                     \
		}                                                                 \
	spec.field = arg

#define save_color(key, color)                                            \
	case key:                                                             \
		spec.colors.color = str2umax (arg, 10);                           \
		if (spec.clrmode != ModeColorsExprs)                              \
			{                                                             \
				spec.clrmode = ModeColors;                                \
			}                                                             \
		else if (spec.clrmode == ModeExprs)                               \
			{                                                             \
				spec.clrmode = ModeColorsExprs;                           \
			}                                                             \
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

		case 'x': spec.max_val = str2umax (arg, 10); break;

		case KEY_RANDOM: set_clrmode (ModeRandom); break;
		case KEY_MONO: set_clrmode (ModeMonochrome); break;

		case KEY_REXPR:
			set_rgbmode (rexpr);
			r_parser.inited = 1;
			break;
		case KEY_GEXPR:
			set_rgbmode (gexpr);
			g_parser.inited = 1;
			break;
		case KEY_BEXPR:
			set_rgbmode (bexpr);
			b_parser.inited = 1;
			break;

			// colors
			save_color ('r', r);
			save_color ('g', g);
			save_color ('b', b);

		case ARGP_KEY_ARG: // program argument
			spec.gen_kind = gen_fromstr (arg);
			break;
		}

	return 0;
}

#undef save_color
#undef set_clrmode

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
	 .key	= KEY_PPM,
	 .arg	= 0,
	 .flags = 0,
	 .doc	= "Use ppm file format",
	 .group = GROUP_FORMATS},
	{.name	= "bmp",
	 .key	= KEY_BMP,
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
	{.name	= "red",
	 .key	= 'r',
	 .arg	= "NUM",
	 .flags = 0,
	 .doc	= "Specify base of red color to use"},
	{.name	= "green",
	 .key	= 'g',
	 .arg	= "NUM",
	 .flags = 0,
	 .doc	= "Specify base of green color to use"},
	{.name	= "blue",
	 .key	= 'b',
	 .arg	= "NUM",
	 .flags = 0,
	 .doc	= "Specify base of green color to use"},
	{.name	= "max",
	 .key	= 'x',
	 .arg	= "NUM",
	 .flags = 0,
	 .doc	= "Specify max colors quantity"},
	{.name	= "mono",
	 .key	= KEY_MONO,
	 .arg	= 0,
	 .flags = 0,
	 .doc	= "Do not use any color palette"},
	{.name	= "random",
	 .key	= KEY_RANDOM,
	 .arg	= 0,
	 .flags = 0,
	 .doc	= "Use random for every color"},
	{.name	= "rexpr",
	 .key	= KEY_REXPR,
	 .arg	= "STR",
	 .flags = 0,
	 .doc	= "Provide an expr to calculate the value of the red color"},
	{.name	= "gexpr",
	 .key	= KEY_GEXPR,
	 .arg	= "STR",
	 .flags = 0,
	 .doc	= "Provide an expr to calculate the value of the green color"},
	{.name	= "bexpr",
	 .key	= KEY_BEXPR,
	 .arg	= "STR",
	 .flags = 0,
	 .doc	= "Provide an expr to calculate the value of the blue color"},
	{0}};

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
		default: break;
		}

	fclose (file);
	return 0;
}
