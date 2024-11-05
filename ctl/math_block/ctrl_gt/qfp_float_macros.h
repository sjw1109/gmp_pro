
#ifndef _FILE_QPF_FLOAT_MACROS_H_
#define _FILE_QPF_FLOAT_MACROS_H_

#ifndef saturation_macro
#define saturation_macro(_a, _min, _max) ((_a) <= (_min)) ? (_min) : (((_a) >= (_max)) ? (_max) : (_a))
#endif

// Type conversion function
#define float2ctrl(x) ((float)x)
#define int2ctrl(x)   (qfp_int2float(x))
#define ctrl2int(x)   (qfp_float2int(x))
#define ctrl_mod_1(x) (float((float)A - (int32_t)A))

// Calculation
#define pwm_mul(A, B)        (qfp_fmul(A, B))
#define ctl_div(A, B)        (qfp_fdiv(A, B))
#define ctl_sat(A, Pos, Neg) saturation_macro(A, Pos, Neg)

// Extension Calculation
#define ctl_div2(A) (qfp_fdiv(A, 2.0f))
#define ctl_div4(A) (qfp_fdiv(A, 4.0f))

// Add and sub
#define ctl_add(A, B) (qfp_fadd(A, B))
#define ctl_sub(A, B) (qfp_fsub(A, B))

// Nonlinear support
#define ctl_sin(A)      (qfp_fsin(A))
#define ctl_cos(A)      (qfp_fcos(A))
#define ctl_tan(A)      (qfp_ftan(A))
#define ctl_atan2(Y, X) (qfp_fatan2(Y, X))
#define ctl_exp(A)      (qfp_fexp(A))
#define ctl_ln(A)       (qfp_fln(A))
#define ctl_sqrt(A)     (qfp_fsqrt(A))
#define ctl_isqrt(A)    (qfp_fdiv(1.0f, qfp_fsqrt(A)))

// Specify ctrl_gt is a float number
#define CTRL_GT_IS_FLOAT

#endif // _FILE_QPF_FLOAT_MACROS_H_
