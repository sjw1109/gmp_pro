// Gain Model Calculator

#ifndef _FILE_GAIN_MODEL_H_
#define _FILE_GAIN_MODEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// if ADC V reference and voltage divider ratio is known,
// target V base is designed.
// 
// such as voltage sensor is a voltage divider, 200kOhm and 3.3kOhm are connected in series.
// The voltage of the ADC is obtained from the two ends of a 3.3k resistor.
// for now Vgain = (3.3 / 203.3) = 0.01623217
GMP_STATIC_INLINE
parameter_gt ctl_gain_calc_via_gain(
    // ADC reference Voltage, unit V
    parameter_gt Vref, 
    // Gain from Voltage sensor, unit 1
    parameter_gt Vgain, 
    // target Voltage base, unit V
    parameter_gt Vbase)
{
    return Vref / Vgain / Vbase;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BIAS_MODEL_H_
