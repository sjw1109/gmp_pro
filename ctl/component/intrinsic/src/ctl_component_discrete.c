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
#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// Divider

#include <ctl/component/intrinsic/discrete/divider.h>

ec_gt ctl_init_divider(ctl_divider_t *obj)
{
    // Current counter
    obj->counter = 0;

    // No division
    obj->target = 0;

    return GMP_EC_OK;
}

ec_gt ctl_setup_divider(ctl_divider_t *obj, uint32_t counter_period)
{
    obj->target = counter_period;

    return GMP_EC_OK;
}

//////////////////////////////////////////////////////////////////////////
// Filter IIR2

#include <ctl/component/intrinsic/discrete/filter.h>
#include <math.h> // support for sinf and cosf

ec_gt ctl_init_lp_filter(ctl_low_pass_filter_t *lpf)
{
    lpf->a = GMP_CONST_1;
    lpf->out = 0;

    return GMP_EC_OK;
}

ec_gt ctl_setup_lp_filter(ctl_low_pass_filter_t *lpf, parameter_gt fs, parameter_gt fc)
{
    lpf->a = ctl_helper_lp_filter(fs, fc);

    return GMP_EC_OK;
}

ctrl_gt ctl_helper_lp_filter(parameter_gt fs, parameter_gt fc)
{
    return float2ctrl(fc * 2 * PI / fs);
}

ec_gt ctl_init_filter_iir2(ctl_filter_IIR2_t *obj)
{
    int i;

    obj->out = 0;

    for (i = 0; i < 2; ++i)
    {
        obj->x[i] = 0;
        obj->y[i] = 0;
        obj->a[i] = 0;
        obj->b[i] = 0;
    }

    obj->b[2] = 0;
        
        return GMP_EC_OK;
}

ec_gt ctl_setup_filter_iir2(ctl_filter_IIR2_t *obj, ctl_filter_IIR2_setup_t *setup_obj)
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

            return GMP_EC_OK;

}

//////////////////////////////////////////////////////////////////////////
// HCC regular

#include <ctl/component/intrinsic/discrete/hcc.h>

ec_gt ctl_init_hcc(ctl_hcc_t *hcc)
{
    hcc->target = 0;
    hcc->half_width = float2ctrl(0.5);
    hcc->current = 0;
    hcc->switch_out = 0;
    hcc->flag_polarity = 1;

    return GMP_EC_OK;
}

ec_gt ctl_setup_hcc(ctl_hcc_t *hcc, fast_gt flag_polarity, ctrl_gt half_width)
{
    hcc->flag_polarity = flag_polarity;
    hcc->half_width = half_width;

    return GMP_EC_OK;
}

//////////////////////////////////////////////////////////////////////////
// PID regular

#include <ctl/component/intrinsic/discrete/pid.h>

ec_gt ctl_init_pid(pid_regular_t *hpid)
{
    hpid->out = 0;

    hpid->kp = float2ctrl(1.0f);
    hpid->ki = 0;
    hpid->kd = 0;

    hpid->out_min = float2ctrl(-1.0f);
    hpid->out_max = float2ctrl(1.0f);

    hpid->dn = 0;
    hpid->sn = 0;

    return GMP_EC_OK;
}

ec_gt ctl_setup_pid(pid_regular_t *hpid, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, ctrl_gt out_min, ctrl_gt out_max)
{
    hpid->kp = kp;
    hpid->ki = ki;
    hpid->kd = kd;

    hpid->out_min = out_min;
    hpid->out_max = out_max;

    return GMP_EC_OK;
}

void ctl_set_pid_parameter(pid_regular_t *hpid, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd)
{
    hpid->kp = kp;
    hpid->ki = ki;
    hpid->kd = kd;
}

void ctl_set_pid_limit(pid_regular_t *hpid, ctrl_gt out_min, ctrl_gt out_max)
{
    hpid->out_min = out_min;
    hpid->out_max = out_max;
}

//////////////////////////////////////////////////////////////////////////
// PLL module

#include <ctl/component/intrinsic/discrete/pll.h>

ec_gt ctl_init_pll(ctl_pll_t *pll)
{
    ctl_init_pid(&pll->pid);
    ctl_init_lp_filter(&pll->filter);
    pll->input.dat[0] = 0;
    pll->input.dat[1] = 0;
    pll->output_theta = 0;
    pll->output_freq = 0;
    pll->freq_sf = float2ctrl(1.0);

    return GMP_EC_OK;
}

ec_gt ctl_setup_pll(ctl_pll_t *pll, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // PID parameter
                    ctrl_gt out_min, ctrl_gt out_max,                   // PID output limit
                    parameter_gt fs,                                    // Sample frequency
                    parameter_gt fc                                     // cutoff frequency
)
{
    ctl_setup_pid(&pll->pid, kp, ki, kd, out_min, out_max);

    ctl_setup_lp_filter(&pll->filter, fs, fc);

    return GMP_EC_OK;
}

//////////////////////////////////////////////////////////////////////////
// Saturation

#include <ctl/component/intrinsic/discrete/saturation.h>

ec_gt ctl_init_saturation(saturation_t *obj)
{
    obj->out_min = float2ctrl(-1.0f);
    obj->out_max = float2ctrl(1.0f);

    return GMP_EC_OK;
}

ec_gt ctl_setup_saturation(saturation_t *obj, ctrl_gt out_min, ctrl_gt out_max)
{
    obj->out_min = out_min;
    obj->out_max = out_max;

    return GMP_EC_OK;
}

void ctl_set_saturation(saturation_t *obj, ctrl_gt out_min, ctrl_gt out_max)
{
    obj->out_min = out_min;
    obj->out_max = out_max;
}

//////////////////////////////////////////////////////////////////////////
// Slope Limiter

#include <ctl/component/intrinsic/discrete/slope_lim.h>

ec_gt ctl_init_slope_limit(ctl_slope_lim_t *obj)
{
    obj->slope_min = float2ctrl(-1.0f);
    obj->slope_max = float2ctrl(1.0f);

    obj->out = float2ctrl(0);

    return GMP_EC_OK;
}

ec_gt ctl_setup_slope_limit(ctl_slope_lim_t *obj, ctrl_gt slope_min, ctrl_gt slope_max)
{
    obj->slope_min = slope_min;
    obj->slope_max = slope_max;

    return GMP_EC_OK;
}

void ctl_set_sl_slope(ctl_slope_lim_t *obj, ctrl_gt slope_min, ctrl_gt slope_max)
{
    obj->slope_min = slope_min;
    obj->slope_max = slope_max;
}

//////////////////////////////////////////////////////////////////////////
// Signal Generator

#include <ctl/component/intrinsic/discrete/stimulate.h>

ec_gt ctl_init_sincos_gen(ctl_src_sg_t *sg)
{
    sg->ph_sin = float2ctrl(0);
    sg->ph_cos = float2ctrl(1.0f);

    sg->ph_sin_delta = float2ctrl(0);
    sg->ph_cos_delta = float2ctrl(1.0f);

    return GMP_EC_OK;
}

ec_gt ctl_setup_sincos_gen(ctl_src_sg_t *sg,
                           ctrl_gt init_angle, // rad
                           ctrl_gt step_angle) // rad
{
    sg->ph_cos = ctl_cos(init_angle);
    sg->ph_sin = ctl_sin(init_angle);

    sg->ph_sin_delta = ctl_sin(step_angle);
    sg->ph_cos_delta = ctl_cos(step_angle);

    return GMP_EC_OK;
}

ec_gt ctl_init_ramp_gen(ctl_src_rg_t *rg)
{
    rg->current = float2ctrl(0);
    rg->maximum = float2ctrl(1.0f);
    rg->minimum = float2ctrl(0);

    rg->slope = float2ctrl(0);

    return GMP_EC_OK;
}

ec_gt ctl_setup_ramp_gen(ctl_src_rg_t *rg, ctrl_gt slope, parameter_gt amp_pos, parameter_gt amp_neg)
{
    rg->maximum = float2ctrl(amp_pos);
    rg->minimum = float2ctrl(amp_neg);

    rg->slope = slope;

    return GMP_EC_OK;
}

ec_gt ctl_setup_ramp_gen_via_amp_freq(ctl_src_rg_t *rg, parameter_gt isr_freq, parameter_gt target_freq,
                                      parameter_gt amp_pos, parameter_gt amp_neg)
{
    rg->maximum = float2ctrl(amp_pos);
    rg->minimum = float2ctrl(amp_neg);
	
	float a = isr_freq / target_freq;
	float b = amp_pos - amp_neg;

    //rg->slope = float2ctrl((amp_pos - amp_neg) / (isr_freq / target_freq));
	rg->slope = float2ctrl( b / a);

    return GMP_EC_OK;
}
