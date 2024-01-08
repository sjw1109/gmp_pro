#ifndef _FILE_CTRL_CONFIG_H_
#define _FILE_CTRL_CONFIG_H_


// global math calculation type
#ifndef GMP_GLOBAL_MATH_T
#define GMP_GLOBAL_MATH_T float
#endif // GMP_GLOBAL_MATH_T

// global control parameter type
#ifndef GMP_CONTROL_PARAM_T 
#define GMP_CONTROL_PARAM_T float
#endif //GMP_CONTROL_PARAM_T

// calculation type definition
typedef GMP_GLOBAL_MATH_T gmp_math_t;
typedef GMP_CONTROL_PARAM_T gmp_ctrl_param_t;

#endif // _FILE_CTRL_CONFIG_H_
