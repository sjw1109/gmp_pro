
#ifndef gmp_math_sat
#define gmp_math_sat(_a, _min, _max)                \
	((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a))
#endif 
