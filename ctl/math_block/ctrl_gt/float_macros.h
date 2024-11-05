
#ifndef _FILE_FLOAT_MACROS_H_
#define _FILE_FLOAT_MACROS_H_

#include <math.h>

#ifndef saturation_macro
#define saturation_macro(_a, _min, _max) ((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a))
#endif

// Type conversion function
#define float2ctrl(x) ((float)x)
#define int2ctrl(x)   ((float)x)
#define ctrl2int(x)   ((int)x)
#define ctrl_mod_1(x) (float((float)A - (int32_t)A))

// Calculation
#define pwm_mul(A, B)        ((pwm_gt)((float)A * B))
#define pwm_sat(A, Pos, Neg) saturation_macro(A, Pos, Neg)
#define ctl_mul(A, B)        ((float)((float)A * B))
#define ctl_div(A, B)        ((float)((float)A / B))
#define ctl_sat(A, Pos, Neg) saturation_macro(A, Pos, Neg)

// #define pwm_mpy(CTRL_RESULT, PWM_SAT) ()

// Extension Calculation
#define ctl_div2(A) ((float)((float)A / 2))
#define ctl_div4(A) ((float)((float)A / 4))

// Add and sub
#define ctl_add(A, B) ((float)((float)A + B))
#define ctl_sub(A, B) ((float)((float)A - B))

// Nonlinear support
#define ctl_sin(A)      sinf(A)
#define ctl_cos(A)      cosf(A)
#define ctl_tan(A)      tanf(A)
#define ctl_atan2(Y, X) atan2f(Y, X)
#define ctl_exp(A)      expf(A)
#define ctl_ln(A)       logf(A)
#define ctl_sqrt(A)     sqrtf(A)
#define ctl_isqrt(A)    (1.0f / sqrtf(A))

// Specify ctrl_gt is a float number
#define CTRL_GT_IS_FLOAT

#endif // _FILE_FLOAT_MACROS_H_
