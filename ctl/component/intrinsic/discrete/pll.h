/**
 * @file pll.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// This file is for PLL (Phase lock Loop) Module

// PLL module is based on PID module and filter module
#include <ctl/component/intrinsic/continuous/continuous_pid.h>
#include <ctl/component/intrinsic/discrete/discrete_filter.h>

#ifndef _FILE_PLL_H_
#define _FILE_PLL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// phasor PLL Object struct
typedef struct _tag_pll
{
    // input phasor
    // input[0]: sin theta
    // input[1]: cos theta
    ctl_vector2_t input;

    // output theta, unit rad
    ctrl_gt output_theta;

    // output speed, unit p.u.
    ctrl_gt output_freq;

    // error controller
    ctl_pid_t pid;

    // freq filter
    ctl_low_pass_filter_t filter;

    // parameter frequency scale factor
    // unit: rad / tick -> frequency p.u.
    ctrl_gt freq_sf;

} ctl_pll_t;

ec_gt ctl_init_pll(ctl_pll_t *pll);

ec_gt ctl_setup_pll(ctl_pll_t *pll, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // PID parameter
                    ctrl_gt out_min, ctrl_gt out_max,                   // PID output limit
                    parameter_gt fs,                                    // Sample frequency
                    parameter_gt fc                                     // cutoff frequency
);

GMP_STATIC_INLINE
void ctl_step_pll(ctl_pll_t *pll)
{
    ctl_vector2_t current_phasor;

    ctl_set_phasor_via_angle(pll->output_theta, &current_phasor);

    ctrl_gt error =
        -ctl_mul(pll->input.dat[0], current_phasor.dat[1]) - ctl_mul(pll->input.dat[1], current_phasor.dat[0]);

    ctl_step_pid_ser(&pll->pid, error);

    ctrl_gt spd_filter = ctl_step_lowpass_filter(&pll->filter, pll->pid.out);

    pll->output_theta = pll->output_theta + spd_filter;

    if (pll->output_theta >= GMP_CONST_2_PI)
        pll->output_theta -= GMP_CONST_2_PI;
    else if (pll->output_theta <= -GMP_CONST_2_PI)
        pll->output_theta += GMP_CONST_2_PI;

    // scale for PLL speed
    pll->output_freq = ctl_mul(spd_filter, pll->freq_sf);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PLL_H_
