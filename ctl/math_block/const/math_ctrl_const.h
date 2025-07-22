
#ifndef _FILE_FIXED_CONST_PARAM_H_
#define _FILE_FIXED_CONST_PARAM_H_

//////////////////////////////////////////////////////////////////////////
// Math Constant Stack

// Here're some constants for coordinate transform.
#define GMP_CONST_ABC2AB_ALPHA (float2ctrl(0.666666666666667)) // 2/3
#define GMP_CONST_ABC2AB_BETA  (float2ctrl(0.577350269189626)) // 1/sqrt(3)
#define GMP_CONST_ABC2AB_GAMMA (float2ctrl(0.333333333333334)) // 1/3

#define GMP_CONST_AB2ABC_ALPHA (float2ctrl(0.8660254)) // sqrt(3)/2

#define GMP_CONST_AB02AB_ALPHA (float2ctrl(1.154700538379252)) // 2/sqrt(3)

// Here're some constants for SVPWM
#define GMP_CONST_SQRT_3_OVER_2 (float2ctrl(0.8660254038f)) // SQRT(3)/2
#define GMP_CONST_3_OVER_2      (float2ctrl(1.5f))
#define GMP_CONST_SQRT_3        (float2ctrl(1.73205080756888f))
#define GMP_CONST_1             (float2ctrl(1.0f))
#define GMP_CONST_1_OVER_2      (float2ctrl(0.5f))

// Here're some constants for Voltage calculator
#define GMP_CONST_1_OVER_SQRT3 ((float2ctrl(0.5773502691896f))) // 1/sqrt(3)

// Here're some constants for encoder and filter
#define GMP_CONST_PI         (float2ctrl(3.1415926535897932))
#define GMP_CONST_2_PI       (float2ctrl(6.2831853071795865))
#define GMP_CONST_1_OVER_2PI (float2ctrl(1 / 6.2831853071795865))

#endif // _FILE_FIXED_CONST_PARAM_H_
