
#ifndef _FILE_DOUBLE_MACROS_H_
#define _FILE_DOUBLE_MACROS_H_

#include <math.h>

GMP_STATIC_INLINE
double saturation_static_inline(double A, double min_val, double max_val)
{
    if (A < min_val)
        return min_val;
    else if (A > max_val)
        return max_val;
    else
        return A;
}

GMP_STATIC_INLINE
double multiple_static_inline(double A, double B)
{
    return A * B;
}

#ifndef saturation_macro
// #define saturation_macro(_a, _min, _max) ((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a))
#define saturation_macro(_a, _max, _min) saturation_static_inline(_a, _min, _max)
#endif

// Type conversion function
#define float2ctrl(x) ((double)(x))
#define int2ctrl(x)   ((double)(x))
#define ctrl2int(x)   ((int)(x))
#define ctrl_mod_1(x) ((double)(((double)(x)) - ((int32_t)(x))))

// Calculation
#define pwm_mul(A, B)        ((pwm_gt)((double)(A) * (double)(B)))
#define pwm_sat(A, Pos, Neg) ((pwm_gt)saturation_macro(((double)(A)), ((double)(Pos)), ((double)(Neg))))

#define ctl_mul(A, B)        ((double)((double)(A) * (B)))
#define ctl_div(A, B)        ((double)((double)(A) / (B)))
#define ctl_sat(A, Pos, Neg) saturation_macro((A), (Pos), (Neg))

// Extension Calculation
#define ctl_div2(A) ((double)((double)(A) / 2))
#define ctl_div4(A) ((double)((double)(A) / 4))

#define ctl_mul2(A) ((double)((double)(A) * 2))
#define ctl_mul4(A) ((double)((double)(A) * 4))

// Add and sub
#define ctl_add(A, B) ((double)((double)(A) + (B)))
#define ctl_sub(A, B) ((double)((double)(A) - (B)))

// Nonlinear support
#define ctl_sin(A)      sin(2 * PI * (A))
#define ctl_cos(A)      cos(2 * PI * (A))
#define ctl_tan(A)      tan(2 * PI * (A))
#define ctl_atan2(Y, X) atan2(Y, X)
#define ctl_exp(A)      exp((A))
#define ctl_ln(A)       log((A))
#define ctl_sqrt(A)     sqrt((A))
#define ctl_isqrt(A)    (1.0 / sqrt(A))

// Specify ctrl_gt is a float number
#define CTRL_GT_IS_FLOAT

#endif // _FILE_FLOAT_MACROS_H_
