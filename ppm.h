#ifndef ture_ppm
#define ture_ppm

#include <stdint.h>
typedef enum
{
	P3,
	P6
} PPMMode;

/// Write ppm mode
void
ppm_wmode (PPMMode m, const char* filename);

typedef struct
{
	int		width;
	int		height;
	PPMMode mode;
} PPMHeader;

/// Write ppm header to file
void
ppm_wheader (PPMHeader* h, const char* filename);

typedef uint8_t* PPMData;

typedef struct
{
	PPMHeader header;
	PPMData	  data;
} PPM;

/// Write ppm image to file
void
ppm_wimage (PPM* ppm, const char* filename);

#endif // !ture_ppm
