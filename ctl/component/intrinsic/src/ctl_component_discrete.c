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
#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// Divider

#include <ctl/component/intrinsic/discrete/divider.h>

void ctl_init_divider(ctl_divider_t *obj, uint32_t counter_period)
{
    // Current counter
    obj->counter = 0;

    obj->target = counter_period;
}

//////////////////////////////////////////////////////////////////////////
// Filter IIR2

#include <ctl/component/intrinsic/discrete/discrete_filter.h>
#include <math.h> // support for sinf and cosf

void ctl_init_lp_filter(ctl_low_pass_filter_t *lpf, parameter_gt fs, parameter_gt fc)
{
    lpf->out = 0;
    lpf->a = ctl_helper_lp_filter(fs, fc);
}

void ctl_init_filter_iir2(ctl_filter_IIR2_t *obj, ctl_filter_IIR2_setup_t *setup_obj)
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
}

//////////////////////////////////////////////////////////////////////////
// PLL module

#include <ctl/component/intrinsic/discrete/pll.h>

void ctl_init_pll(
    // PLL Controller Object
    ctl_pll_t *pll,
    // PID parameter
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // PID output limit
    ctrl_gt out_min, ctrl_gt out_max,
    // cutoff frequency
    parameter_gt fc,
    // Sample frequency
    parameter_gt fs)
{
    ctl_init_pid(&pll->pid, kp, Ti, Td, fs);
    ctl_set_pid_limit(&pll->pid, out_max, out_min);
    ctl_init_lp_filter(&pll->filter, fs, fc);
}

//////////////////////////////////////////////////////////////////////////
// Slope Limiter

#include <ctl/component/intrinsic/discrete/slope_lim.h>

void ctl_init_slope_limit(ctl_slope_lim_t *obj, ctrl_gt slope_max, ctrl_gt slope_min)
{
    obj->slope_min = slope_min;
    obj->slope_max = slope_max;

    obj->out = float2ctrl(0);
}

//////////////////////////////////////////////////////////////////////////
// Signal Generator

#include <ctl/component/intrinsic/discrete/stimulate.h>

void ctl_init_sincos_gen(ctl_src_sg_t *sg,
                         parameter_gt init_angle, // pu
                         parameter_gt step_angle) // pu
{
    sg->ph_cos = float2ctrl(cos(init_angle));
    sg->ph_sin = float2ctrl(sin(init_angle));

    sg->ph_sin_delta = float2ctrl(sin(step_angle));
    sg->ph_cos_delta = float2ctrl(cos(step_angle));
}

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
#ifdef _USE_DEBUG_DISCRETE_PID
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
    parameter_gt b1 = ki / 2.0f / fs - 2.0f * kd * fs;
    parameter_gt b0 = kd * fs + ki / 2.0f / fs;

    pid->kp = float2ctrl(kp);

    pid->b2 = float2ctrl(b2);
    pid->b1 = float2ctrl(b1);
    pid->b0 = float2ctrl(b0);

    pid->output_max = float2ctrl(1.0);
    pid->output_min = float2ctrl(-1.0);
}
#else // _USE_DEBUG_DISCRETE_PID
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
    parameter_gt b1 = ki / 2.0f / fs - kp - 2.0f * kd * fs;
    parameter_gt b0 = kp + kd * fs + ki / 2.0f / fs;

    pid->b2 = float2ctrl(b2);
    pid->b1 = float2ctrl(b1);
    pid->b0 = float2ctrl(b0);

    pid->output_max = float2ctrl(1.0);
    pid->output_min = float2ctrl(-1.0);
}

#endif // _USE_DEBUG_DISCRETE_PID

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
    ctl_init_slope_limit(&tp->traj, float2ctrl(slope_max / fs), float2ctrl(slope_min / fs));
    ctl_init_divider(&tp->div, division);

    ctl_init_discrete_pid(&tp->pid, kp, Ti, Td, fs);
    ctl_set_discrete_pid_limit(&tp->pid, sat_max, sat_min);
}

//////////////////////////////////////////////////////////////////////////
// Pole-Zero Compensator

#include <ctl/component/intrinsic/discrete/pole_zero.h>

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
    parameter_gt gain_discrete = gain * (1.0f / 2.0f / fs / (p1 + 2.0f * fs));
    parameter_gt b0 = (z1 + 2.0f * fs) * (z0 + 2.0f * fs);
    parameter_gt b1 = ((z0 + 2.0f * fs) * (z1 - 2.0f * fs) + (z1 + 2.0f * fs) * (z0 - 2.0f * fs));
    parameter_gt b2 = (z1 - 2.0f * fs) * (z0 - 2.0f * fs);
    parameter_gt a1 = -(4.0f * fs / (p1 + 2.0f * fs));
    parameter_gt a2 = (2.0f * fs - p1) / (2.0f * fs + p1);

    ctrl->a1 = float2ctrl(a1);
    ctrl->a2 = float2ctrl(a2);
    ctrl->b0 = float2ctrl(b0);
    ctrl->b1 = float2ctrl(b1);
    ctrl->b2 = float2ctrl(b2);
    ctrl->gain = float2ctrl(gain_discrete);

    ctrl->out_max = float2ctrl(1.0);
    ctrl->out_min = float2ctrl(-1.0);

    ctrl->output_1 = 0;
    ctrl->output_2 = 0;
    ctrl->input_1 = 0;
    ctrl->input_2 = 0;

    ctrl->output = 0;
    ctrl->input = 0;
}

//////////////////////////////////////////////////////////////////////////
// PR / QPR controller

#include <ctl/component/intrinsic/discrete/proportional_resonant.h>

void ctl_init_pr_controller(
    // handle of PR controller
    pr_ctrl_t *pr,
    // Kp
    parameter_gt kp,
    // gain of resonant frequency
    parameter_gt kr,
    // resonant frequency, unit Hz
    parameter_gt freq_resonant,
    // controller frequency
    parameter_gt fs)
{
    // resonant frequency, unit rad/s
    parameter_gt omega_r = 2 * PI * freq_resonant;

    pr->kpg = float2ctrl(kp);
    pr->krg = float2ctrl(kr * (4 * fs) / (4 * fs * fs + omega_r * omega_r));
    pr->kr = float2ctrl(2 * (4 * fs * fs - omega_r * omega_r) / (4 * fs * fs + omega_r * omega_r));

    // clear temp variables
    ctl_clear_pr_controller(pr);
}

void ctl_init_qpr_controller(
    // handle of QPR controller
    qpr_ctrl_t *qpr,
    // Kp
    parameter_gt kp,
    // gain of resonant frequency
    parameter_gt kr,
    // resonant frequency, unit Hz
    parameter_gt freq_resonant,
    // cut frequency, unit Hz
    parameter_gt freq_cut,
    // controller frequency
    parameter_gt fs)
{
    ctl_clear_qpr_controller(qpr);

    parameter_gt omega_r_sqr = 4.0f * PI * PI * freq_resonant * freq_resonant;
    parameter_gt omega_c_fs = 4.0f * 2.0f * PI * freq_cut * fs;

    parameter_gt kr_suffix = 4.0f * freq_cut * fs / (4.0f * fs * fs + omega_c_fs + omega_r_sqr);

    parameter_gt b1 = 2.0f * (4.0f * fs * fs - omega_r_sqr) / (4.0f * fs * fs + omega_c_fs + omega_r_sqr);
    parameter_gt b2 = (4.0f * fs * fs - omega_c_fs + omega_r_sqr) / (4.0f * fs * fs + omega_c_fs + omega_r_sqr);

    // Discrete parameters
    qpr->a0 = float2ctrl(kr_suffix);
    qpr->b1 = float2ctrl(b1);
    qpr->b2 = float2ctrl(b2);

    qpr->kp = float2ctrl(kp);
    qpr->kr = float2ctrl(kr);
}

//////////////////////////////////////////////////////////////////////////
//

#include <ctl/component/intrinsic/discrete/discrete_sogi.h>

void ctl_init_discrete_sogi(
    // Handle of discrete SOGI object
    discrete_sogi_t *sogi,
    // damp coefficient, generally is 0.5
    parameter_gt k_damp,
    // center frequency, Hz
    parameter_gt fn,
    // isr frequency, Hz
    parameter_gt fs)
{
    ctl_clear_discrete_sogi(sogi);

    parameter_gt osgx, osgy, temp, wn, delta_t;
    delta_t = 1.0f / fs;
    wn = fn * 2.0f * 3.14159265f;
    // wn = fn * GMP_CONST_2_PI;

    osgx = (2.0f * k_damp * wn * delta_t);
    osgy = (float32_t)(wn * delta_t * wn * delta_t);
    temp = (float32_t)1.0 / (osgx + osgy + 4.0f);

    sogi->b0 = float2ctrl(osgx * temp);
    sogi->b2 = -sogi->b0;
    sogi->a1 = float2ctrl((2.0f * (4.0f - osgy)) * temp);
    sogi->a2 = float2ctrl((osgx - osgy - 4.0f) * temp);
    sogi->qb0 = float2ctrl((k_damp * osgy) * temp);
    sogi->qb1 = float2ctrl(sogi->qb0 * (2.0f));
    sogi->qb2 = sogi->qb0;
}
