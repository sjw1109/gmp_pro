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
#define pwm_sat(A, Pos, Neg) _IQsat(A, Pos, Neg) 

#define ctrl_div2(A) _IQdiv2(A)



#else
// FPU controller configuration 



#endif

#endif // _FILE_CTRL_CONFIG_H_
