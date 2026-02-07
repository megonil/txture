#ifndef txture_flags
#define txture_flags

typedef enum
{
	FlagUsePPM,
	FlagUseBMP,
} Flags;

typedef int flag_t;

#define ext_flags() extern flag_t flags;

#endif // !txture_flags
