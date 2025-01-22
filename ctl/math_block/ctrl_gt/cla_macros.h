
#ifndef _FILE_CLA_MACROS_H_
#define _FILE_CLA_MACROS_H_

#include <CLAmath.h>

//
// Here list all the functions that CLA math provided.
//
// standard math function 
// 
// + acosf   
// + asinf   
// + atanf   
// + atan2f  
// + cosf    
// + expf    
// + logf    
// + log10f  
// + sinf    
// + sqrtf   
// + tanf 
// 
// + fmaxf
// + fminf
//
// TMU Intrinsics for CLA2
//
// + __divf32
// + __sqrt
// + __sin
// + __cos
// + __atan
// + __atan2
// + __sinpuf32
// + __cospuf32 
//
// FPU Intrinsics
//
// + __einvf32
// + __eisqrtf32
// + __f32toi16r
// + __f32toui16r
// + __fmax
// + __fmin
// + __fracf32
// + __swapf
// + __swapff     

GMP_STATIC_INLINE
float32_t saturation_static_inline(float32_t in, float32_t min_val, float32_t max_val)
{
    float32_t out = in;

    out = __fmax(out, min_val);
    out = __fmin(out, max_val);

    return (out);
}

GMP_STATIC_INLINE
float multiple_static_inline(float A, float B)
{
    return A * B;
}

#ifndef saturation_macro
// #define saturation_macro(_a, _min, _max) (((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a)))
#define saturation_macro(_a, _max, _min) saturation_static_inline(_a, _min, _max)
#endif

// Type conversion function
#define float2ctrl(x) ((float32_t)(x))
#define int2ctrl(x)   ((float32_t)(x))
#define ctrl2int(x)   ((int)(x))
#define ctrl_mod_1(x) ((float32_t)(((float32_t)(x)) - ((int32_t)(x))))

// Calculation
#define pwm_mul(A, B)        ((pwm_gt)((((float)(A)) * ((float)(B)))))
#define pwm_sat(A, Pos, Neg) ((pwm_gt)saturation_macro(((float)(A)), ((float)(Pos)), ((float)(Neg))))
#define ctl_mul(A, B)        ((float)((float)(A) * (B)))
#define ctl_div(A, B)        ((float32_t)(__fracf32((A) , (B))))
#define ctl_sat(A, Pos, Neg) saturation_macro((A), (Pos), (Neg))

// #define pwm_mpy(CTRL_RESULT, PWM_SAT) ()

// Extension Calculation
#define ctl_div2(A) ((float32_t)((float32_t)(A) / 2))
#define ctl_div4(A) ((float32_t)((float32_t)(A) / 4))

// Add and sub
#define ctl_add(A, B) ((float32_t)((float32_t)(A) + (B)))
#define ctl_sub(A, B) ((float32_t)((float32_t)(A) - (B)))

// Nonlinear support
#define ctl_sin(A)      (__sinpuf32((A))   )
#define ctl_cos(A)      (__cospuf32((A))   )
#define ctl_tan(A)      (tanf(2 * PI * (A)))
#define ctl_atan2(Y, X) (__atan2((Y), (X)) )
#define ctl_exp(A)      (expf((A))         )
#define ctl_ln(A)       (logf((A))         )
#define ctl_sqrt(A)     (CLAsqrt((A))      )
#define ctl_isqrt(A)    (__eisqrtf32((A)))

// Specify ctrl_gt is a float number
#define CTRL_GT_IS_FLOAT

#endif // _FILE_ARM_CMSIS_MACROS_H_


