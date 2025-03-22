/**
 * @file ctl_common_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#include <ctl/ctl_core.h>
#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// Divider

#include <ctl/component/intrinsic/discrete/divider.h>

// ec_gt ctl_init_divider(ctl_divider_t *obj)
//{
//     // Current counter
//     obj->counter = 0;
//
//     // No division
//     obj->target = 0;
//
//     return GMP_EC_OK;
// }

void ctl_init_divider(ctl_divider_t *obj, uint32_t counter_period)
{
    // Current counter
    obj->counter = 0;

    obj->target = counter_period;
}

//////////////////////////////////////////////////////////////////////////
// Filter IIR2

#include <ctl/component/intrinsic/discrete/filter.h>
#include <math.h> // support for sinf and cosf

// ec_gt ctl_init_lp_filter(ctl_low_pass_filter_t *lpf)
//{
//     lpf->a = GMP_CONST_1;
//     lpf->out = 0;
//
//     return GMP_EC_OK;
// }

void ctl_init_lp_filter(ctl_low_pass_filter_t *lpf, parameter_gt fs, parameter_gt fc)
{
    lpf->out = 0;
    lpf->a = ctl_helper_lp_filter(fs, fc);
}

//// This function has move to header file
// ctrl_gt ctl_helper_lp_filter(parameter_gt fs, parameter_gt fc)
//{
//     return float2ctrl(fc * 2 * PI / fs);
// }

// ec_gt ctl_init_filter_iir2(ctl_filter_IIR2_t *obj)
//{
//     int i;
//
//     obj->out = 0;
//
//     for (i = 0; i < 2; ++i)
//     {
//         obj->x[i] = 0;
//         obj->y[i] = 0;
//         obj->a[i] = 0;
//         obj->b[i] = 0;
//     }
//
//     obj->b[2] = 0;
//
//         return GMP_EC_OK;
// }

ec_gt ctl_init_filter_iir2(ctl_filter_IIR2_t *obj, ctl_filter_IIR2_setup_t *setup_obj)
{
    // center frequency
    // tex: $$ f_0 = f_c * 2Q$$
    parameter_gt f0 = setup_obj->fc * 2 * setup_obj->q;

    // tex: $$ \theta = 2\pi \frac{f_c}{f_s}$$
    parameter_gt theta = 2.0f * PI * f0 / setup_obj->fs;

    parameter_gt sin_theta = sinf(theta);

    parameter_gt cos_theta = cosf(theta);

    // tex: $$\alpha = \frac{\sin(\theta)}{2Q} $$
    parameter_gt alpha = sin_theta / 2 / setup_obj->q;

    // a_0, a_1, a_2
    parameter_gt a0 = (1.0f + alpha);
    obj->a[0] = float2ctrl(-2.0f * cos_theta / a0);
    obj->a[1] = float2ctrl((1.0f - alpha) / a0);

    switch (setup_obj->filter_type)
    {
    case FILTER_IIR2_TYPE_HIGHPASS:
        obj->b[0] = float2ctrl(setup_obj->gain * (1.0f + cos_theta) / (2 * a0));
        obj->b[1] = float2ctrl(-setup_obj->gain * (1.0f + cos_theta) / a0);
        obj->b[2] = float2ctrl(setup_obj->gain * (1.0f + cos_theta) / (2 * a0));
        break;
    case FILTER_IIR2_TYPE_LOWPASS:
        obj->b[0] = float2ctrl(setup_obj->gain * (1.0f - cos_theta) / (2 * a0));
        obj->b[1] = float2ctrl(setup_obj->gain * (1.0f - cos_theta) / a0);
        obj->b[2] = float2ctrl(setup_obj->gain * (1.0f - cos_theta) / (2 * a0));
        break;
    case FILTER_IIR2_TYPE_BANDPASS:
        obj->b[0] = float2ctrl(setup_obj->gain * sin_theta / (2 * a0));
        obj->b[1] = 0;
        obj->b[2] = float2ctrl(setup_obj->gain * sin_theta / (2 * a0));
        break;
    default:
        // do nothing
        break;
    }

    obj->out = 0;

    return GMP_EC_OK;
}

//////////////////////////////////////////////////////////////////////////
// PLL module

#include <ctl/component/intrinsic/discrete/pll.h>

ec_gt ctl_init_pll(ctl_pll_t *pll)
{
    //ctl_init_pid(&pll->pid);
    //ctl_init_lp_filter(&pll->filter);
    //pll->input.dat[0] = 0;
    //pll->input.dat[1] = 0;
    //pll->output_theta = 0;
    //pll->output_freq = 0;
    //pll->freq_sf = float2ctrl(1.0);

    return GMP_EC_OK;
}

ec_gt ctl_setup_pll(ctl_pll_t *pll, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // PID parameter
                    ctrl_gt out_min, ctrl_gt out_max,                   // PID output limit
                    parameter_gt fs,                                    // Sample frequency
                    parameter_gt fc                                     // cutoff frequency
)
{
    //ctl_setup_pid(&pll->pid, kp, ki, kd, out_min, out_max);

    //ctl_setup_lp_filter(&pll->filter, fs, fc);

    return GMP_EC_OK;
}

//////////////////////////////////////////////////////////////////////////
// Slope Limiter

#include <ctl/component/intrinsic/discrete/slope_lim.h>

// ec_gt ctl_init_slope_limit(ctl_slope_lim_t *obj)
//{
//     obj->slope_min = float2ctrl(-1.0f);
//     obj->slope_max = float2ctrl(1.0f);
//
//     obj->out = float2ctrl(0);
//
//     return GMP_EC_OK;
// }

void ctl_init_slope_limit(ctl_slope_lim_t *obj, ctrl_gt slope_min, ctrl_gt slope_max)
{
    obj->slope_min = slope_min;
    obj->slope_max = slope_max;

    obj->out = float2ctrl(0);
}

//// The following function has move to header file
//void ctl_set_sl_slope(ctl_slope_lim_t *obj, ctrl_gt slope_min, ctrl_gt slope_max)
//{
//    obj->slope_min = slope_min;
//    obj->slope_max = slope_max;
//}

//////////////////////////////////////////////////////////////////////////
// Signal Generator

#include <ctl/component/intrinsic/discrete/stimulate.h>

// ec_gt ctl_init_sincos_gen(ctl_src_sg_t *sg)
//{
//     sg->ph_sin = float2ctrl(0);
//     sg->ph_cos = float2ctrl(1.0f);
//
//     sg->ph_sin_delta = float2ctrl(0);
//     sg->ph_cos_delta = float2ctrl(1.0f);
//
//     return GMP_EC_OK;
// }

void ctl_init_sincos_gen(ctl_src_sg_t *sg,
                         ctrl_gt init_angle, // rad
                         ctrl_gt step_angle) // rad
{
    sg->ph_cos = ctl_cos(init_angle);
    sg->ph_sin = ctl_sin(init_angle);

    sg->ph_sin_delta = ctl_sin(step_angle);
    sg->ph_cos_delta = ctl_cos(step_angle);
}

// ec_gt ctl_init_ramp_gen(ctl_src_rg_t *rg)
//{
//     rg->current = float2ctrl(0);
//     rg->maximum = float2ctrl(1.0f);
//     rg->minimum = float2ctrl(0);
//
//     rg->slope = float2ctrl(0);
//
//     return GMP_EC_OK;
// }

void ctl_init_ramp_gen(ctl_src_rg_t *rg, ctrl_gt slope, parameter_gt amp_pos, parameter_gt amp_neg)
{
    rg->current = float2ctrl(0);

    rg->maximum = float2ctrl(amp_pos);
    rg->minimum = float2ctrl(amp_neg);

    rg->slope = slope;
}

void ctl_init_ramp_gen_via_amp_freq(
    // pointer to ramp generator object
    ctl_src_rg_t *rg,
    // isr frequency, unit Hz
    parameter_gt isr_freq,
    // target frequency, unit Hz
    parameter_gt target_freq,
    // ramp range
    parameter_gt amp_pos, parameter_gt amp_neg)
{
    rg->current = float2ctrl(0);

    rg->maximum = float2ctrl(amp_pos);
    rg->minimum = float2ctrl(amp_neg);

    float a = isr_freq / target_freq;
    float b = amp_pos - amp_neg;

    // rg->slope = float2ctrl((amp_pos - amp_neg) / (isr_freq / target_freq));
    rg->slope = float2ctrl(b / a);
}

//////////////////////////////////////////////////////////////////////////
// Discrete PID controller

#include <ctl/component/intrinsic/discrete/discrete_pid.h>

void ctl_init_discrete_pid(
    // pointer to pid object
    discrete_pid_t *pid,
    // gain of the pid controller
    parameter_gt kp,
    // Time constant for integral and differential part, unit Hz
    parameter_gt Ti, parameter_gt Td,
    // sample frequency, unit Hz
    parameter_gt fs)
{
    pid->input = 0;
    pid->input_1 = 0;
    pid->input_2 = 0;
    pid->output = 0;
    pid->output_1 = 0;

    parameter_gt ki = kp / Ti;
    parameter_gt kd = kp * Td;

    parameter_gt b2 = kd * fs;
    parameter_gt b1 = ki / 2 / fs - kp - 2 * kd * fs;
    parameter_gt b0 = kp + kd * fs + ki / 2 / fs;

    pid->b2 = float2ctrl(b2);
    pid->b1 = float2ctrl(b1);
    pid->b0 = float2ctrl(b0);

    pid->output_max = float2ctrl(1.0);
    pid->output_min = float2ctrl(-1.0);
}

//////////////////////////////////////////////////////////////////////////
// Discrete track pid

#include <ctl/component/intrinsic/discrete/track_discrete_pid.h>

void ctl_init_discrete_track_pid(
    // pointer to track pid object
    track_discrete_pid_t *tp,
    // pid parameters, unit sec
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // saturation limit
    ctrl_gt sat_max, ctrl_gt sat_min,
    // slope limit, unit: p.u./sec
    parameter_gt slope_max, parameter_gt slope_min,
    // division factor:
    uint32_t division,
    // controller frequency, unit Hz
    parameter_gt fs)
{
    ctl_init_discrete_pid(&tp->pid, kp, Ti, Td, fs);
    ctl_set_discrete_pid_limit(&tp->pid, sat_max, sat_min);
    ctl_init_slope_limit(&tp->traj, float2ctrl(slope_min / fs), float2ctrl(slope_max / fs));
    ctl_init_divider(&tp->div, division);
}

//////////////////////////////////////////////////////////////////////////
// Pole-Zero Compensator

#include <ctl/component/intrinsic/discrete/PZCompensator.h>

// unit Hz
void ctl_init_2p2z(
    // pointer to a 2p2z compensator
    ctrl_2p2z_t *ctrl,
    // gain of 2P2Z compensator
    parameter_gt gain,
    // two zero frequency, unit Hz
    parameter_gt f_z0, parameter_gt f_z1,
    // one pole frequency, unit Hz
    parameter_gt f_p1,
    // sample frequency
    parameter_gt fs)
{
    parameter_gt z0 = f_z0 * 2 * PI;
    parameter_gt z1 = f_z1 * 2 * PI;
    parameter_gt p1 = f_p1 * 2 * PI;

    // discrete controller parameter
    parameter_gt gain_discrete = gain * (1 / 2 / fs / (p1 + 2 * fs));
    parameter_gt b0 = (z1 + 2 * fs) * (z0 + 2 * fs);
    parameter_gt b1 = ((z0 + 2 * fs) * (z1 - 2 * fs) + (z1 + 2 * fs) * (z0 - 2 * fs));
    parameter_gt b2 = (z1 - 2 * fs) * (z0 - 2 * fs);
    parameter_gt a1 = -(4 * fs / (p1 + 2 * fs));
    parameter_gt a2 = (2 * fs - p1) / (2 * fs + p1);

    ctrl->a1 = float2ctrl(a1);
    ctrl->a2 = float2ctrl(a2);
    ctrl->b0 = float2ctrl(b0);
    ctrl->b1 = float2ctrl(b1);
    ctrl->b2 = float2ctrl(b2);

    ctrl->out_max = float2ctrl(1.0);
    ctrl->out_min = float2ctrl(-1.0);

    ctrl->output_1 = 0;
    ctrl->output_2 = 0;
    ctrl->input_1 = 0;
    ctrl->input_2 = 0;

    ctrl->output = 0;
    ctrl->input = 0;
}
