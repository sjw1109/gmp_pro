
#ifndef _FILE_GMP_CTL_MATH_H_
#define _FILE_GMP_CTL_MATH_H_



#ifndef gmp_math_sat
#define gmp_math_sat(_a, _min, _max)                \
	((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a))
#endif 


//////////////////////////////////////////////////////////////////////////
// Extension Types

typedef struct _tag_ctl_vector3_t
{
	ctrl_gt dat[3];
}ctl_vector3_t;

typedef struct _tag_ctl_vector2_t
{
	ctrl_gt dat[2];
}ctl_vector2_t;



//////////////////////////////////////////////////////////////////////////
// Math Constant Stack

// Here're some constants for coordinate transform.
#define GMP_CONST_ABC2AB_ALPHA (CTRL_T(0.666666666666667)) // 2/3
#define GMP_CONST_ABC2AB_BETA  (CTRL_T(0.577350269189626)) // 1/sqrt(3)
#define GMP_CONST_ABC2AB_GAMMA (CTRL_T(0.333333333333334)) // 1/3

#define GMP_CONST_AB02AB_ALPHA (CTRL_T(1.154700538379252)) // 2/sqrt(3)

// Here're some constants for SVPWM
#define GMP_CONST_SQRT_3_OVER_2 (CTRL_T(0.8660254038f))
#define GMP_CONST_3_OVER_2		(CTRL_T(1.5f))
#define GMP_CONST_SQRT_3		(CTRL_T(1.73205080756888f))
#define GMP_CONST_1				(CTRL_T(1.0f))
#define GMP_CONST_1_OVER_2		(CTRL_T(0.5f))

// Here're some constants for encoder and filter
#define GMP_CONST_PI			(CTRL_T(3.1415926535897932))
#define GMP_CONST_2_PI			(CTRL_T(6.2831853071795865))


#endif // _FILE_GMP_CTL_MATH_H_
