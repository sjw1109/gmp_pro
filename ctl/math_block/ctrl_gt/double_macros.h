
#ifndef _FILE_FLOAT_MACROS_H_
#define _FILE_FLOAT_MACROS_H_

#ifndef saturation_macro
#define saturation_macro(_a, _min, _max) ((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a))
#endif

// Type conversion function
#define float2ctrl(x) ((double)x)
#define int2ctrl(x)   ((double)x)
#define ctrl2int(x)   ((int)x)
#define ctrl_mod_1(x) (double((double)A - (int32_t)A))

// Calculation
#define pwm_mul(A, B)        ((double)((double)A * B))
#define ctl_div(A, B)        ((double)((double)A / B))
#define ctl_sat(A, Pos, Neg) saturation_macro(A, Pos, Neg)

// Extension Calculation
#define ctl_div2(A) ((double)((double)A / 2))
#define ctl_div4(A) ((double)((double)A / 4))

// Add and sub
#define ctl_add(A, B) ((double)((double)A + B))
#define ctl_sub(A, B) ((double)((double)A - B))

// Nonlinear support
#define ctl_sin(A)      sin(A)
#define ctl_cos(A)      cos(A)
#define ctl_tan(A)      tan(A)
#define ctl_atan2(Y, X) atan2(Y, X)
#define ctl_exp(A)      exp(A)
#define ctl_ln(A)       log(A)
#define ctl_sqrt(A)     sqrt(A)
#define ctl_isqrt(A)    (1.0f / sqrt(A))

// Specify ctrl_gt is a float number
#define CTRL_GT_IS_FLOAT

#endif // _FILE_FLOAT_MACROS_H_
