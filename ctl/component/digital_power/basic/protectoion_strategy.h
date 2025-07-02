// This is the standard protection strategy for DC/DC converter

#include <ctl/component/intrinsic/discrete/discrete_filter.h>

#ifndef _FILE_PROTECTION_STRATEGY_H_
#define _FILE_PROTECTION_STRATEGY_H_

// Standard GMP CTL protection has the following three stage
// + Brickwall overcurrent protection
//   When Io > current_max output will close.
//
// + Brickwall overvoltage protection
//   When Vo > voltage_max output will close.
//
// + overpower protection
//   When Po > power_max output will close.
//
typedef struct _tag_std_vip_protection_type
{
    //
    // Controller port
    //

    // output voltage
    adc_ift *adc_uo;

    // output current
    adc_ift *adc_io;

    //
    // output
    //

    // flag of error
    fast_gt flag_error;

    //
    // Parameters
    //

    // voltage limit
    ctrl_gt voltage_max;

    // current limit
    ctrl_gt current_max;

    // power limit
    ctrl_gt power_max;

    //
    // intermediate
    //

    // output voltage
    ctrl_gt uout;

    // output current
    ctrl_gt iout;

    // output power
    ctrl_gt pout;

    //
    // modules
    //

    // power measurement filter
    ctl_low_pass_filter_t power_filter;

    // voltage measurement filter
    ctl_low_pass_filter_t voltage_filter;

    // current measurement filter
    ctl_low_pass_filter_t current_filter;

} std_vip_protection_t;

// Attach ADC interface to
void ctl_attach_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj,
    // output voltage
    adc_ift *uo,
    // output current
    adc_ift *io);

void ctl_init_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj,
    // Power measurement filter cut frequency
    parameter_gt power_f_cut,
    // Voltage measurement filter cut frequency
    parameter_gt voltage_f_cut,
    // Current measurement filter cut frequency
    parameter_gt current_f_cut,
    // voltage maximum, voltage base value
    parameter_gt v_max, parameter_gt v_base,
    // current maximum, current base value
    parameter_gt i_max, parameter_gt i_base,
    // power maximum
    parameter_gt p_max,
    // sample frequency
    parameter_gt fs);

GMP_STATIC_INLINE
void ctl_clear_vip_protection_error(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj)
{
    obj->flag_error = 0;
}

GMP_STATIC_INLINE
fast_gt ctl_step_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj)
{

    // current calculating and filtering
    obj->iout = ctl_step_lowpass_filter(&obj->current_filter, obj->adc_io->value);

    // voltage calculating and filtering
    obj->uout = ctl_step_lowpass_filter(&obj->current_filter, obj->adc_uo->value);

    // power calculating and filtering
    obj->pout = ctl_step_lowpass_filter(&obj->current_filter, ctl_mul(obj->iout, obj->uout));

    // error has occurred
    if (obj->flag_error != 0)
    {
        return obj->flag_error;
    }

    // danger range
    if ((obj->iout > obj->current_max) || (obj->uout > obj->voltage_max) || (obj->pout > obj->power_max))
    {
        obj->flag_error = 1;
    }

    return obj->flag_error;
}

// + Foldback overcurrent protection
typedef struct _tag_std_foldback_protection_type
{
    //
    // Controller port
    //

    // output voltage
    adc_ift *adc_uo;

    // output current
    adc_ift *adc_io;



} std_foldback_protection_t;

#endif // _FILE_PROTECTION_STRATEGY_H_
