/**
 * @file pid.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-03-20
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/math_block/gmp_math.h>

#ifndef _FILE_PI_DISCRETE_H_
#define _FILE_PI_DISCRETE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// PID transfer function
// tex:
// $$ G(s) = k_p + \frac{k_i}{s} + k_d s $$
// where, $$ T_i = \frac{k_p}{k_i} $$ ,
// $$T_d = \frac{k_d}{k_p}$$

// using Tustin/Trapezoidal Transformation
// tex:
// $$ s = 2f_s\frac{1-z^{-1}}{1+z^{-1}}$$

// for integral item use backward Euler Transformation
// tex:
// $$ s = f_s (1-z^{-1})$$

typedef struct _tag_discrete_pid_tuning_t
{

    parameter_gt kp;
    parameter_gt Ti;
    parameter_gt Td;
    parameter_gt fs;
} discrete_pid_tunning_t;

typedef struct _tag_discrete_pid_t
{
    // SISO standard interface
    ctrl_gt input;
    ctrl_gt output;

    ctrl_gt b2, b1, b0;
#ifdef _USE_DEBUG_DISCRETE_PID
    ctrl_gt kp;
#endif // _USE_DEBUG_DISCRETE_PID

    ctrl_gt input_1;
    ctrl_gt input_2;

    ctrl_gt output_1;

    ctrl_gt output_max;
    ctrl_gt output_min;
} discrete_pid_t;

void ctl_init_discrete_pid(discrete_pid_t *pid, parameter_gt kp, parameter_gt Ti, parameter_gt Td, parameter_gt fs);

GMP_STATIC_INLINE
void ctl_clear_discrete_pid(discrete_pid_t *pid)
{
    pid->input_1 = 0;
    pid->input_2 = 0;
    pid->output_1 = 0;
}

GMP_STATIC_INLINE
void ctl_set_discrete_pid_limit(discrete_pid_t *pid, ctrl_gt limit_max, ctrl_gt limit_min)
{
    pid->output_max = limit_max;
    pid->output_min = limit_min;
}

// SISO standard interface
#ifdef _USE_DEBUG_DISCRETE_PID
GMP_STATIC_INLINE
ctrl_gt ctl_step_discrete_pid(discrete_pid_t *ctrl, ctrl_gt input)
{
    ctrl->input = input;

    // transfer function
    ctrl->output = ctl_mul(ctrl->kp, ctrl->input);
    ctrl->output += ctl_mul(ctrl->b0, ctrl->input);
    ctrl->output += ctl_mul(ctrl->b1, ctrl->input_1);
    ctrl->output += ctl_mul(ctrl->b2, ctrl->input_2);

    ctrl->output += ctl_mul(float2ctrl(0.95), ctrl->output_1);

    // saturation
    ctrl->output = ctl_sat(ctrl->output, ctrl->output_max, ctrl->output_min);

    // round numbers
    ctrl->input_2 = ctrl->input_1;
    ctrl->input_1 = ctrl->input;
    ctrl->output_1 = ctrl->output;

    return ctrl->output;
}
#else // _USE_DEBUG_DISCRETE_PID

GMP_STATIC_INLINE
ctrl_gt ctl_step_discrete_pid(discrete_pid_t *ctrl, ctrl_gt input)
{
    ctrl->input = input;

    // transfer function
    ctrl->output = ctl_mul(ctrl->b0, ctrl->input);
    ctrl->output += ctl_mul(ctrl->b1, ctrl->input_1);
    ctrl->output += ctl_mul(ctrl->b2, ctrl->input_2);

    ctrl->output += ctrl->output_1;

    // saturation
    ctrl->output = ctl_sat(ctrl->output, ctrl->output_max, ctrl->output_min);

    // round numbers
    ctrl->input_2 = ctrl->input_1;
    ctrl->input_1 = ctrl->input;
    ctrl->output_1 = ctrl->output;

    return ctrl->output;
}

#endif // _USE_DEBUG_DISCRETE_PID

// get discrete pid output
GMP_STATIC_INLINE
ctrl_gt ctl_get_discrete_pid_output(discrete_pid_t *ctrl)
{
    return ctrl->output;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_PI_H_
