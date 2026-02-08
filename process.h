#ifndef txture_process
#define txture_process

#include "ppm.h"
#include "utils.h"

#include <stdio.h>

#define process_func(name) void name (FILE* file, int width, )

#define process_debug(filename, width, height)                            \
	debugln ("Creating image with filename %s, with height = %d and "     \
			 "width = %d",                                                \
			 filename, height, width)

void
process_ppm (FILE* file, PPMMode mode, unsigned max_val);

#undef process_func
#endif // !txture_process
