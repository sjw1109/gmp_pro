#ifndef _FILE_CTRL_CONFIG_H_
#define _FILE_CTRL_CONFIG_H_


//// global math calculation type
//#ifndef GMP_GLOBAL_MATH_T
//#define GMP_GLOBAL_MATH_T float
//#endif // GMP_GLOBAL_MATH_T
//
//// global control parameter type
//#ifndef GMP_CONTROL_PARAM_T 
//#define GMP_CONTROL_PARAM_T float
//#endif //GMP_CONTROL_PARAM_T
//
//// calculation type definition
//typedef GMP_GLOBAL_MATH_T gmp_math_t;
//typedef GMP_CONTROL_PARAM_T gmp_ctrl_param_t;

#ifndef DEFAULT_ADC_RESOLUTION
#define DEFAULT_ADC_RESOLUTION ((12))
#endif // DEFAULT_ADC_RESOLUTION

#ifndef DEFAULT_ADC_IQN
#define DEFAULT_ADC_IQN ((12))
#endif // DEFAULT_ADC_IQN

// FIX controller configuration
#ifdef USING_FIXED_LIBRARY

#define CTRL_T(x) _IQ(x)
#define ctrl_mpy(A, B) _IQmpy(A, B)
#define ctrl_sat(A, Pos, Neg) _IQsat(A, Pos, Neg) 
#define ctrl2float(x) _IQ24toF(x)

#define pwm_mpy(A, B) _IQmpy(A, B)
#define ctrl_div(A, B) _IQdiv(A, B)
#define pwm_sat(A, Pos, Neg) _IQsat(A, Pos, Neg) 

#define ctrl_div2(A) _IQdiv2(A)
#define ctrl_div4(A) _IQdiv4(A)

#define ctrl_sin(A) _IQsin(A)
#define ctrl_cos(A) _IQcos(A)


#else
// FPU controller configuration 



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
#define GMP_CONST_ABC2AB_ALPHA ((0.666666666666667)) // 2/3
#define GMP_CONST_ABC2AB_BETA  ((0.577350269189626)) // 1/sqrt(3)
#define GMP_CONST_ABC2AB_GAMMA ((0.333333333333334)) // 1/3

#define GMP_CONST_AB02AB_ALPHA ((1.154700538379252)) // 2/sqrt(3)

// Here're some constants for SVPWM
#define GMP_CONST_SQRT_3_OVER_2 ((0.8660254038f))
#define GMP_CONST_3_OVER_2 ((1.5f))
#define GMP_CONST_SQRT_3   ((1.73205080756888f))
#define GMP_CONST_1 ((1.0f))
#define GMP_CONST_1_OVER_2 ((0.5f))

#endif // _FILE_CTRL_CONFIG_H_
