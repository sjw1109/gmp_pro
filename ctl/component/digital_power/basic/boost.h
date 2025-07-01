
#include <ctl/component/interface/interface_base.h>
#include <ctl/component/intrinsic/continuous/continuous_pid.h>

#ifndef _FILE_BOOST_CTRL_H_
#define _FILE_BOOST_CTRL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Boost Controller
typedef struct _tag_boost_ctrl_type
{
    //
    // Controller port
    //

    // capacitor voltage
    adc_ift *uc;

    // inductor current
    adc_ift *il;

    // input voltage
    adc_ift *uin;

    //
    // Output
    //

    // modulation duty
    ctrl_gt pwm_out_pu;

    //
    // Controller objects
    //
    pid_regular_t current_pid;
    pid_regular_t voltage_pid;

    //
    // feed forward
    //

    // current feed forward
    ctrl_gt current_ff;

    // voltage feed forward
    ctrl_gt voltage_ff;

    //
    // intermediate variables
    //

    // current target
    ctrl_gt current_set;

    // voltage target
    ctrl_gt voltage_set;

    // output voltage, transformation ratio
    ctrl_gt voltage_out;

    //
    // flag stack
    //

    // enable controller
    fast_gt flag_enable_system;

    // enable output
    fast_gt flag_enable_output;

    // enable current controller
    fast_gt flag_enable_current_ctrl;

    // enable voltage controller
    fast_gt flag_enable_voltage_ctrl;

} boost_ctrl_t;

void ctl_init_boost_ctrl(
    // Boost controller
    boost_ctrl_t *boost,
    // Voltage PID controller
    parameter_gt v_kp, parameter_gt v_Ti, parameter_gt v_Td,
    // Current PID controller
    parameter_gt i_kp, parameter_gt i_Ti, parameter_gt i_Td,
    // Controller frequency, Hz
    parameter_gt fs);

GMP_STATIC_INLINE
ctrl_gt ctl_step_boost_ctrl(boost_ctrl_t *boost)
{
    if (boost->flag_enable_system)
    {

        if (boost->flag_enable_voltage_ctrl)
        {
            boost->current_set =
                ctl_step_pid_ser(&boost->voltage_pid, boost->voltage_set - boost->uc->value) + boost->current_ff;
        }

        if (boost->flag_enable_current_ctrl)
        {
            boost->voltage_out =
                ctl_step_pid_ser(&boost->current_pid, boost->current_set - boost->il->value) + boost->voltage_ff;
        }

        if (boost->flag_enable_output)
        {
            // pwm_out_pu = float2ctrl(1) - voltage_set;
            boost->pwm_out_pu = (boost->voltage_out - boost->uin->value) / boost->voltage_out;
        }
        else
        {
            boost->pwm_out_pu = float2ctrl(0);
        }
    }
    else
    {
        boost->pwm_out_pu = float2ctrl(0);
    }

    return boost->pwm_out_pu;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BOOST_CTRL_H_
