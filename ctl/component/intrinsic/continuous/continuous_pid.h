/**
 * @file pid.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-03-19
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// continuous PID controller

#include <ctl/math_block/gmp_math.h>

#ifndef _FILE_CONTINUOUS_PID_H_
#define _FILE_CONTINUOUS_PID_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// PI regular intermediate variables
typedef struct _tag_pid_regular_inter_t
{
    // output
    ctrl_gt out;

    // parameters
    ctrl_gt kp;
    ctrl_gt ki;
    ctrl_gt kd;

    ctrl_gt out_min;
    ctrl_gt out_max;

    // intermediate variables

    // sum-up of N
    ctrl_gt sn;

    // difference of N
    ctrl_gt dn;
} pid_regular_t, ctl_pid_t;

// paralleling PID model
GMP_STATIC_INLINE
ctrl_gt ctl_step_pid_par(pid_regular_t *hpid, ctrl_gt input)
{
    // I sum up
    // Bug fix: hpid->sn may overflow
    hpid->sn = ctl_sat(hpid->sn + ctl_mul(input, (hpid->ki)), hpid->out_max, hpid->out_min);

    // output = P item + I item  + D item
    hpid->out = ctl_mul(input, hpid->kp) + hpid->sn + ctl_mul((input - hpid->dn), hpid->kd);

    // output saturation
    hpid->out = ctl_sat(hpid->out, hpid->out_max, hpid->out_min);

    // record input param
    hpid->dn = input;

    return hpid->out;
}

// Series PID model
GMP_STATIC_INLINE
ctrl_gt ctl_step_pid_ser(pid_regular_t *hpid, ctrl_gt input)
{
    // Kp gain firstly, out = P item
    hpid->out = ctl_mul(input, hpid->kp);

    // I sum up
    // Bug fix: hpid->sn may overflow
    hpid->sn = ctl_sat(hpid->sn + ctl_mul(hpid->out, hpid->ki), hpid->out_max, hpid->out_min);

    // output += I item + D item
    hpid->out += hpid->sn + ctl_mul((input - hpid->dn), hpid->kd);

    // output saturation
    hpid->out = ctl_sat(hpid->out, hpid->out_max, hpid->out_min);

    // record input param
    hpid->dn = input;

    return hpid->out;
}

void ctl_init_pid(
    // continuous pid handle
    pid_regular_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs);

// init a Series PID
void ctl_init_pid_ser(
    // continuous pid handle
    pid_regular_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs);

// init a paralleling PID
void ctl_init_pid_par(
    // continuous pid handle
    pid_regular_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs);

GMP_STATIC_INLINE
void ctl_set_pid_parameter(pid_regular_t *hpid, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd)
{
    hpid->kp = kp;
    hpid->ki = ki;
    hpid->kd = kd;
}

GMP_STATIC_INLINE
void ctl_set_pid_limit(pid_regular_t *hpid, ctrl_gt out_max, ctrl_gt out_min)
{
    hpid->out_min = out_min;
    hpid->out_max = out_max;
}

GMP_STATIC_INLINE
void ctl_clear_pid(pid_regular_t *hpid)
{
    hpid->dn = 0;
    hpid->sn = 0;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_pid_output(pid_regular_t *hpid)
{
    return hpid->out;
}

// PID controller with anti-integral windup
typedef struct _tag_pid_anti_windup
{
    // output
    ctrl_gt out;

    // parameters
    ctrl_gt kp;
    ctrl_gt ki;
    ctrl_gt kd;

    ctrl_gt out_min;
    ctrl_gt out_max;

    // Back-Calculation parameters
    ctrl_gt kc;

    // intermediate variables
    // sum-up of N
    ctrl_gt sn;

    // difference of N
    ctrl_gt dn;

    // output variable without saturation
    ctrl_gt out_without_sat;
} pid_aw_t;

// init a Series PID
void ctl_init_pid_aw_ser(
    // continuous pid handle
    pid_aw_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs);

// init a parallel PID
void ctl_init_pid_aw_par(
    // continuous pid handle
    pid_aw_t *hpid,
    // PID parameters
    parameter_gt kp, parameter_gt Ti, parameter_gt Td,
    // controller frequency
    parameter_gt fs);

// paralleling PID model
GMP_STATIC_INLINE
ctrl_gt ctl_step_pid_aw_par(pid_aw_t *hpid, ctrl_gt input)
{
    // I sum up
    // Bug fix: hpid->sn may overflow
    hpid->sn = ctl_sat(hpid->sn + ctl_mul(input, (hpid->ki)), hpid->out_max, hpid->out_min);

    // output = P item + I item  + D item
    hpid->out_without_sat = ctl_mul(input, hpid->kp) + hpid->sn + ctl_mul((input - hpid->dn), hpid->kd);

    // output saturation
    hpid->out = ctl_sat(hpid->out_without_sat, hpid->out_max, hpid->out_min);

    // back calculation for integrate
    if (hpid->out_without_sat > hpid->out_max)
        hpid->sn = hpid->sn - ctl_mul(hpid->out_without_sat - hpid->out, hpid->kc);

    // record input param
    hpid->dn = input;

    return hpid->out;
}

// Series PID model
GMP_STATIC_INLINE
ctrl_gt ctl_step_pid_aw_ser(pid_aw_t *hpid, ctrl_gt input)
{
    // Kp gain firstly, out = P item
    hpid->out_without_sat = ctl_mul(input, hpid->kp);

    // I sum up
    // Bug fix: hpid->sn may overflow
    hpid->sn = ctl_sat(hpid->sn + ctl_mul(hpid->out_without_sat, hpid->ki), hpid->out_max, hpid->out_min);

    // output += I item + D item
    hpid->out_without_sat += hpid->sn + ctl_mul((input - hpid->dn), hpid->kd);

    // output saturation
    hpid->out = ctl_sat(hpid->out, hpid->out_max, hpid->out_min);

    // back calculation for integrate
    if (hpid->out_without_sat > hpid->out_max)
        hpid->sn = hpid->sn - ctl_mul(hpid->out_without_sat - hpid->out, hpid->kc);

    // record input param
    hpid->dn = input;

    return hpid->out;
}

GMP_STATIC_INLINE
void ctl_clear_pid_aw(pid_aw_t *hpid)
{
    hpid->dn = 0;
    hpid->sn = 0;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_pid_aw_output(pid_aw_t *hpid)
{
    return hpid->out;
}

GMP_STATIC_INLINE
void ctl_set_pid_aw_back_gain(pid_aw_t *hpid, ctrl_gt back_gain)
{
    hpid->kc = back_gain;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_CONTINUOUS_PID_H_
