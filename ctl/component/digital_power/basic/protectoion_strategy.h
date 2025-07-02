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
    adc_ift *uo;

    // output current
    adc_ift *io;

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

    // output power
    ctrl_gt po;

    //
    // modules
    //

    // power measurement filter
    ctl_low_pass_filter_t power_filter;

} std_vip_protection_t;

void ctl_attach_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj,
    // output voltage
    adc_ift *uo,
    // output current
    adc_ift *io)
{
    obj->io = io;
    obj->uo = uo;
}

void ctl_init_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj,
    // Power measurement filter cut frequency
    parameter_gt f_cut,
    // voltage maximum, voltage base value
    parameter_gt v_max, parameter_gt v_base,
    // current maximum, current base value
    parameter_gt i_max, parameter_gt i_base,
    // power maximum
    parameter_gt p_max,
    // sample frequency
    parameter_gt fs)
{
    obj->voltage_max = float2ctrl(v_max / v_base);
    obj->current_max = float2ctrl(i_max / i_base);
    obj->power_max = float2ctrl(p_max / v_base / i_base);

    ctl_init_lp_filter(&obj->power_filter, fs, f_cut);
}

void ctl_clear_vip_protection_error(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj)
{
    obj->flag_error = 0;
}

fast_gt ctl_step_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj)
{
    // power calculating and filtering
    obj->po = ctl_step_lowpass_filter(ctl_mul(obj->io->value, obj->uo->value));

    // error has occorred
    if (obj->flag_error != 0)
    {
        return obj->flag_error;
    }

    // danger range
    if ((obj->io->value > obj->current_max) || (obj->uo->value > obj->voltage_max) || (obj->po > obj->power_max))
    {
        obj->flag_error = 1;
    }

    return obj->flag_error;
}

// + Foldback overcurrent protection
typedef struct _tag_std_foldback_protection_type
{

} std_foldback_protection_t;

#endif // _FILE_PROTECTION_STRATEGY_H_
