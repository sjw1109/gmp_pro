/**
 * @file peripheral_if_util_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <gmp_core.h>

#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// ADC channel

#include <ctl/component/intrinsic/interface/adc_channel.h>

ec_gt ctl_init_adc_channel(adc_channel_t *adc_obj)
{
    adc_obj->raw = 0;
    adc_obj->resolution = 0;
    adc_obj->iqn = 0;
    adc_obj->bias = 0;
    adc_obj->gain = float2ctrl(1.0f);
    adc_obj->value = 0;

    return GMP_EC_OK;
}

ec_gt ctl_setup_adc_channel(adc_channel_t *adc_obj, ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn)
{
    adc_obj->bias = bias;
    adc_obj->gain = gain;
    adc_obj->iqn = iqn;
    adc_obj->resolution = resolution;

    return GMP_EC_OK;
}

ec_gt ctl_init_adc_dual_channel(adc_dual_channel_t *adc)
{
    int i = 0;

    for (i = 0; i < 2; ++i)
    {
        adc->raw[i] = 0;
        adc->bias[i] = 0;
        adc->gain[i] = float2ctrl(1.0);
        adc->value[i] = 0;
    }

    adc->resolution = 16;
    adc->iqn = 16;

    return GMP_EC_OK;
}

ec_gt ctl_setup_adc_dual_channel(adc_dual_channel_t *adc, ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn)
{
    int i = 0;

    for (i = 0; i < 2; ++i)
    {
        adc->bias[i] = bias;
        adc->gain[i] = gain;
    }

    adc->resolution = resolution;
    adc->iqn = iqn;

    return GMP_EC_OK;
}

ec_gt ctl_init_adc_tri_channel(adc_tri_channel_t *adc)
{
    int i = 0;

    for (i = 0; i < 3; ++i)
    {
        adc->raw[i] = 0;
        adc->bias[i] = 0;
        adc->gain[i] = float2ctrl(1.0);
        adc->value[i] = 0;
    }

    adc->resolution = 16;
    adc->iqn = 16;

    return GMP_EC_OK;
}

ec_gt ctl_setup_adc_tri_channel(adc_tri_channel_t *adc, ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn)
{
    int i = 0;

    for (i = 0; i < 3; ++i)
    {
        adc->bias[i] = bias;
        adc->gain[i] = gain;
    }

    adc->resolution = resolution;
    adc->iqn = iqn;

    return GMP_EC_OK;
}

// ADC calibrator
ec_gt ctl_init_adc_bias_calibrator(adc_bias_calibrator_t *obj)
{
    obj->bias_output = 0;
    obj->flag_output_valid = 0;
    obj->raw = 0;

    obj->total_period = 0;
    obj->current_period = 0;
    obj->start_period = 0;
    obj->filter_tick = 0;

    ctl_init_filter_iir2(&obj->filter);

    return GMP_EC_OK;
}

ec_gt ctl_setup_adc_bias_calibrator(adc_bias_calibrator_t *obj, ctl_filter_IIR2_setup_t *filter_parameter)
{
    uint32_t total_period = (uint32_t)(10 * filter_parameter->fc);

    // setup the filter
    ctl_setup_filter_iir2(&obj->filter, filter_parameter);

    // at least 1000 period
    if (total_period > 1000)
        obj->total_period = total_period;
    else
        obj->total_period = 1000;

    return GMP_EC_OK;
}

void ctl_restart_adc_bias_calibrator(adc_bias_calibrator_t *obj)
{
    obj->start_period = 0;
    obj->filter_tick = 0;
    obj->flag_output_valid = 0;

    ctl_clear_filter_iir2(&obj->filter);
}

// return value means if the calibration output is valid
fast_gt ctl_step_adc_bias_calibrator(adc_bias_calibrator_t *obj, uint32_t main_isr_tick, ctrl_gt adc_value)
{
    if (obj->start_period == 0)
    {
        // need a brand new start
        obj->start_period = main_isr_tick;
        obj->flag_output_valid = 0;
    }

    // if calibration has not complete
    if (!obj->flag_output_valid)
    {
        // need to call the filter once.
        if (obj->current_period != main_isr_tick)
        {
            ctl_step_filter_iir2(&obj->filter, adc_value);
            obj->filter_tick += 1;
        }
        // break the whole process if current_period == main_isr_tick
        else
        {
            return 0;
        }

        obj->current_period = main_isr_tick;

        // Calculate delta tick
        uint32_t delta_tick;

        if (obj->current_period >= obj->start_period)
        {
            delta_tick = obj->current_period - obj->start_period;
        }
        else
        {
            delta_tick = UINT32_MAX - obj->start_period + obj->current_period;
        }

        // complete the calibrate
        if (delta_tick > obj->total_period)
        {
            obj->bias_output = obj->filter.out;
            obj->flag_output_valid = 1;
            return 1;
        }
        return 0;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////////
// PWM channel

#include <ctl/component/intrinsic/interface/pwm_channel.h>

ec_gt ctl_init_pwm_channel(pwm_channel_t *pwm_obj)
{
    pwm_obj->raw = 0;
    pwm_obj->value = 0;

    return GMP_EC_OK;
}

ec_gt ctl_setup_pwm_channel(pwm_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale)
{
    pwm_obj->full_scale = full_scale;
    pwm_obj->phase = phase;

    return GMP_EC_OK;
}

pwm_gt ctl_calc_pwm_channel(pwm_channel_t *pwm_obj, ctrl_gt raw)
{
    pwm_obj->raw = raw;

    pwm_obj->value = pwm_mul(pwm_obj->raw, pwm_obj->full_scale) + pwm_obj->phase;
    pwm_obj->value = pwm_sat(pwm_obj->value, pwm_obj->full_scale, 0);

    return pwm_obj->value;
}

pwm_gt ctl_calc_pwm_channel_warp(pwm_channel_t *pwm_obj, ctrl_gt raw)
{
    pwm_obj->raw = raw;

    pwm_obj->value = pwm_mul(pwm_obj->raw, pwm_obj->full_scale) + pwm_obj->phase;
    pwm_obj->value = pwm_obj->value % pwm_obj->full_scale;

    return pwm_obj->value;
}

pwm_gt ctl_calc_pwm_channel_inv(pwm_channel_t *pwm_obj, ctrl_gt raw)
{
    pwm_obj->raw = raw;

    pwm_obj->value = pwm_obj->full_scale - pwm_mul(pwm_obj->raw, pwm_obj->full_scale);
    pwm_obj->value = pwm_sat(pwm_obj->value, pwm_obj->full_scale, 0);

    return pwm_obj->value;
}

//////////////////////////////////////////////////////////////////////////
// PWM dual channel

// init funciton for dual channel PWM channel
ec_gt ctl_init_pwm_dual_channel(pwm_dual_channel_t *pwm_obj)
{
    for (int i = 0; i < 2; ++i)
    {
        pwm_obj->raw[i] = 0;
        pwm_obj->value[i] = 0;
    }

    return GMP_EC_OK;
}

// setup pwm object
ec_gt ctl_setup_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale)
{
    pwm_obj->full_scale = full_scale;
    pwm_obj->phase = phase;

    return GMP_EC_OK;
}

// calculate function
void ctl_calc_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw)
{
    for (int i = 0; i < 2; ++i)
    {
        pwm_obj->raw[i] = raw->dat[i];

        pwm_obj->value[i] = pwm_mul(pwm_obj->raw[i], pwm_obj->full_scale) + pwm_obj->phase;
        pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
    }
}

void ctl_calc_pwm_dual_channel_warp(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw)
{
    for (int i = 0; i < 2; ++i)
    {
        pwm_obj->raw[i] = raw->dat[i];

        pwm_obj->value[i] = pwm_mul(pwm_obj->raw[i], pwm_obj->full_scale) + pwm_obj->phase;
        pwm_obj->value[i] = pwm_obj->value[i] % pwm_obj->full_scale;
    }
}

void ctl_calc_pwm_dual_channel_inv(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw)
{
    for (int i = 0; i < 2; ++i)
    {
        pwm_obj->raw[i] = raw->dat[i];

        pwm_obj->value[i] = pwm_obj->full_scale - pwm_mul(pwm_obj->raw[i], pwm_obj->full_scale);
        pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
    }
}

//////////////////////////////////////////////////////////////////////////
// PWM tri channel

// init funciton for tri-channel PWM channel
ec_gt ctl_init_pwm_tri_channel(pwm_tri_channel_t *pwm_obj)
{
    for (int i = 0; i < 3; ++i)
    {
        pwm_obj->raw[i] = 0;
        pwm_obj->value[i] = 0;
    }

    return GMP_EC_OK;
}

// setup PWM object
ec_gt ctl_setup_pwm_tri_channel(pwm_tri_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale)
{
    pwm_obj->full_scale = full_scale;
    pwm_obj->phase = phase;

    return GMP_EC_OK;
}

// calculate function
void ctl_calc_pwm_tri_channel(pwm_tri_channel_t *pwm_obj, ctl_vector3_t *raw)
{
    for (int i = 0; i < 3; ++i)
    {
        pwm_obj->raw[i] = raw->dat[i];

        pwm_obj->value[i] = pwm_mul(pwm_obj->raw[i], pwm_obj->full_scale) + pwm_obj->phase;
        pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
    }
}

void ctl_calc_pwm_tri_channel_warp(pwm_tri_channel_t *pwm_obj, ctl_vector3_t *raw)
{
    for (int i = 0; i < 3; ++i)
    {
        pwm_obj->raw[i] = raw->dat[i];

        pwm_obj->value[i] = pwm_mul(pwm_obj->raw[i], pwm_obj->full_scale) + pwm_obj->phase;
        pwm_obj->value[i] = pwm_obj->value[i] % pwm_obj->full_scale;
    }
}

void ctl_calc_pwm_tri_channel_inv(pwm_tri_channel_t *pwm_obj, ctl_vector3_t *raw)
{
    for (int i = 0; i < 3; ++i)
    {
        pwm_obj->raw[i] = raw->dat[i];

        pwm_obj->value[i] = pwm_obj->full_scale - pwm_mul(pwm_obj->raw[i], pwm_obj->full_scale);
        pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
    }
}
