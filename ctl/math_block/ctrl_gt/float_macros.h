
#ifndef _FILE_FLOAT_MACROS_H_
#define _FILE_FLOAT_MACROS_H_

#include <math.h>

GMP_STATIC_INLINE
float saturation_static_inline(float A, float min_val, float max_val)
{
    if (A < min_val)
        return min_val;
    else if (A > max_val)
        return max_val;
    else
        return A;
}

GMP_STATIC_INLINE
float multiple_static_inline(float A, float B)
{
    return A * B;
}

GMP_STATIC_INLINE
float abs_static_inline(float A)
{
    if (A < 0)
        return (-A);
    else
        return A;
}

#ifndef saturation_macro
// #define saturation_macro(_a, _min, _max) (((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a)))
#define saturation_macro(_a, _max, _min) saturation_static_inline(_a, _min, _max)
#endif

// Type conversion function
#define float2ctrl(x) ((float)(x))
#define ctrl2float(x) ((float)(x))
#define int2ctrl(x)   ((float)(x))
#define ctrl2int(x)   ((int)(x))
#define ctrl_mod_1(x) ((float)(((float)(x)) - ((int32_t)(x))))

// Calculation
#define pwm_mul(A, B)        ((pwm_gt)((((float)(A)) * ((float)(B)))))
#define pwm_sat(A, Pos, Neg) ((pwm_gt)saturation_macro(((float)(A)), ((float)(Pos)), ((float)(Neg))))
#define ctl_mul(A, B)        ((float)((float)(A) * (B)))
#define ctl_div(A, B)        ((float)((float)(A) / (B)))
#define ctl_sat(A, Pos, Neg) saturation_macro((A), (Pos), (Neg))
#define ctl_abs(A)           abs_static_inline(A)

// #define pwm_mpy(CTRL_RESULT, PWM_SAT) ()

// Extension Calculation
#define ctl_div2(A) ((float)((float)(A) / 2))
#define ctl_div4(A) ((float)((float)(A) / 4))

#define ctl_mul2(A) ((float)((float)(A) * 2))
#define ctl_mul4(A) ((float)((float)(A) * 4))

// Add and sub
#define ctl_add(A, B) ((float)((float)(A) + (B)))
#define ctl_sub(A, B) ((float)((float)(A) - (B)))

// Nonlinear support
#define ctl_sin(A)      sinf(2 * PI * (A))
#define ctl_cos(A)      cosf(2 * PI * (A))
#define ctl_tan(A)      tanf(2 * PI * (A))
#define ctl_atan2(Y, X) atan2f((Y), (X))
#define ctl_exp(A)      expf((A))
#define ctl_ln(A)       logf((A))
#define ctl_sqrt(A)     sqrtf((A))
#define ctl_isqrt(A)    (1.0f / sqrtf((A)))

// Specify ctrl_gt is a float number
#define CTRL_GT_IS_FLOAT

#endif // _FILE_FLOAT_MACROS_H_
