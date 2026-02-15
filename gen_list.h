#ifndef txture_genlist
#define txture_genlist

#define GEN_LIST(T)                                                       \
	T (GenPerlin, "perlin")                                               \
	T (GenXor, "xor")                                                     \
	T (GenValue, "value")                                                 \
	T (GenChecker, "checkerboard")                                        \
	T (GenStripes, "stripes")                                             \
	T (GenRadial, "radial")                                               \
	T (GenDiagonal, "diagstripes")                                        \
	T (GenSimplex, "simplex")

#endif // !txture_genlist
