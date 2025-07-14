// Bias Model Calculator

#ifndef _FILE_BIAS_MODEL_H_
#define _FILE_BIAS_MODEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// if ADC V reference and Bias voltage is known
GMP_STATIC_INLINE
parameter_gt ctl_bias_calc_via_Vref_Vbias(parameter_gt Vref, parameter_gt Vbias)
{
    return Vbias / Vref;
}

#ifndef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BIAS_MODEL_H_
