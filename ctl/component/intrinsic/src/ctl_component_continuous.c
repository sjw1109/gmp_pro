/**
 * @file ctl_common_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-03-19
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// PID regular

#include <ctl/component/intrinsic/continuous/continuous_pid.h>

void ctl_init_pid(
    // continuous pid handle
    pid_regular_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs)
{
    hpid->kp = float2ctrl(kp);
    hpid->ki = float2ctrl(kp / (fs * Ti));
    hpid->kd = float2ctrl(kp * fs * Td);

    hpid->out_min = float2ctrl(-1.0f);
    hpid->out_max = float2ctrl(1.0f);

    hpid->integral_min = float2ctrl(-0.8f);
    hpid->integral_max = float2ctrl(0.8f);

    hpid->out = 0;
    hpid->dn = 0;
    hpid->sn = 0;
}

// Init a series PID object
void ctl_init_pid_ser(
    // continuous pid handle
    pid_regular_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs)
{
    hpid->kp = float2ctrl(kp);
    hpid->ki = float2ctrl(1.0f / (fs * Ti));
    hpid->kd = float2ctrl(1.0f * fs * Td);

    hpid->out_min = float2ctrl(-1.0f);
    hpid->out_max = float2ctrl(1.0f);

    hpid->integral_min = float2ctrl(-0.8f);
    hpid->integral_max = float2ctrl(0.8f);

    hpid->out = 0;
    hpid->dn = 0;
    hpid->sn = 0;
}

// init a parallel PID object
void ctl_init_pid_par(
    // continuous pid handle
    pid_regular_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs)
{
    hpid->kp = float2ctrl(kp);
    hpid->ki = float2ctrl(1.0f / (fs * Ti));
    hpid->kd = float2ctrl(1.0f * fs * Td);

    hpid->out_min = float2ctrl(-1.0f);
    hpid->out_max = float2ctrl(1.0f);

    hpid->integral_min = float2ctrl(-0.8f);
    hpid->integral_max = float2ctrl(0.8f);

    hpid->out = 0;
    hpid->dn = 0;
    hpid->sn = 0;
}

// init a Series PID
void ctl_init_pid_aw_ser(
    // continuous pid handle
    pid_aw_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs)
{
    hpid->kp = float2ctrl(kp);
    hpid->ki = float2ctrl(kp / (fs * Ti));
    hpid->kd = float2ctrl(kp * fs * Td);

    // set anti-windup parameter based on kp
    if (kp < 0.7f)
        hpid->kc = float2ctrl(1.3f);
    else if (kp > 2.0f)
        hpid->kc = float2ctrl(0.5f);
    else
        hpid->kc = float2ctrl(1 / kp);

    hpid->out_min = float2ctrl(-1.0f);
    hpid->out_max = float2ctrl(1.0f);

    hpid->out = 0;
    hpid->dn = 0;
    hpid->sn = 0;
}

// init a parallel PID
void ctl_init_pid_aw_par(
    // continuous pid handle
    pid_aw_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs)
{

    hpid->kp = float2ctrl(kp);
    hpid->ki = float2ctrl(kp / (fs * Ti));
    hpid->kd = float2ctrl(kp * fs * Td);

    // set anti-windup parameter based on kp
    if (kp < 0.7f)
        hpid->kc = float2ctrl(1.3f);
    else if (kp > 2.0f)
        hpid->kc = float2ctrl(0.5f);
    else
        hpid->kc = float2ctrl(1 / kp);

    hpid->out_min = float2ctrl(-1.0f);
    hpid->out_max = float2ctrl(1.0f);

    hpid->out = 0;
    hpid->dn = 0;
    hpid->sn = 0;
}

//////////////////////////////////////////////////////////////////////////
// Saturation

#include <ctl/component/intrinsic/continuous/saturation.h>

void ctl_init_saturation(ctl_saturation_t *obj, ctrl_gt out_min, ctrl_gt out_max)
{
    obj->out_min = out_min;
    obj->out_max = out_max;
}

//////////////////////////////////////////////////////////////////////////
// Track_PID.h
//

#include <ctl/component/intrinsic/continuous/track_pid.h>

void ctl_init_track_pid(
    // handle of track pid
    track_pid_t *tp,
    // pid parameters
    ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
    // saturation limit
    ctrl_gt sat_max, ctrl_gt sat_min,
    // slope limit
    ctrl_gt slope_max, ctrl_gt slope_min,
    // division factor
    uint32_t division,
    // controller frequency
    parameter_gt fs)
{
    ctl_init_slope_limit(&tp->traj, float2ctrl(slope_max / fs), float2ctrl(slope_min / fs));
    ctl_init_divider(&tp->div, division);

    ctl_init_pid(&tp->pid, kp, ki, kd, fs);
    ctl_set_pid_limit(&tp->pid, sat_max, sat_min);
}

//////////////////////////////////////////////////////////////////////////
// SOGI controller

#include <ctl/component/intrinsic/continuous/sogi.h>

void ctl_init_sogi_controller(
    // controller handle
    ctl_sogi_t *sogi,
    // gain of this controller
    parameter_gt gain,
    // resonant frequency
    parameter_gt freq_r,
    // cut frequency
    parameter_gt freq_c,
    // controller frequency
    parameter_gt freq_ctrl)
{
    sogi->k_damp = float2ctrl(2 * freq_c / freq_r);
    sogi->k_r = float2ctrl(2 * PI * freq_r / freq_ctrl);
    sogi->gain = float2ctrl(gain);

    sogi->integrate_reference = 0;
    sogi->d_integrate = 0;
    sogi->q_integrate = 0;
}

void ctl_init_sogi_controller_with_damp(
    // controller handle
    ctl_sogi_t *sogi,
    // gain of this controller
    parameter_gt gain,
    // resonant frequency
    parameter_gt freq_r,
    // cut frequency, generally 1.414 is a great choice
    parameter_gt damp,
    // controller frequency
    parameter_gt freq_ctrl)
{
    sogi->k_damp = float2ctrl(damp);
    sogi->k_r = float2ctrl(2 * PI * freq_r / freq_ctrl);
    sogi->gain = float2ctrl(gain);

    sogi->integrate_reference = 0;
    sogi->d_integrate = 0;
    sogi->q_integrate = 0;
}
