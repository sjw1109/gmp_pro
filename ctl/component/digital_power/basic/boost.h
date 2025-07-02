
#include <ctl/component/interface/interface_base.h>
#include <ctl/component/intrinsic/continuous/continuous_pid.h>

#ifndef _FILE_BOOST_CTRL_H_
#define _FILE_BOOST_CTRL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// You may enable this macro to enter debug mode.
// Duty of Boost will not calculated by Uin.
#define CTL_BOOST_CTRL_OUTPUT_WITHOUT_UIN

// BOOST CONTROLLER Global parameters
#ifndef CTL_BOOST_CTRL_UIN_MIN
#define CTL_BOOST_CTRL_UIN_MIN ((float2ctrl(0.01)))
#endif // CTL_BOOST_CTRL_UIN_MIN

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

void ctl_attach_boost_ctrl_input(
    // Boost controller
    boost_ctrl_t *boost,
    // output capacitor voltage
    adc_ift *uc,
    // inductor current
    adc_ift *il,
    // input voltage
    adc_ift *uin);

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
#ifdef CTL_BOOST_CTRL_OUTPUT_WITHOUT_UIN
            // Boost duty is generated without input voltage
            boost->pwm_out_pu = float2ctrl(1) - boost->voltage_out;
#else
            if (boost->uin->value > CTL_BOOST_CTRL_UIN_MIN)
            {
                // Boost duty is generated with voltage input
                boost->pwm_out_pu = (boost->voltage_out - boost->uin->value) / boost->voltage_out;
            }
            else
            {
                // if boost input is too low, just output.
                boost->pwm_out_pu = float2ctrl(1);
            }

#endif // CTL_BOOST_CTRL_OUTPUT_WITHOUT_UIN
        }
        else
        {
            // For safety design considerations,
            // it is necessary to make Boost bridge conductive by default.
            boost->pwm_out_pu = float2ctrl(1);
        }
    }
    else
    {
        // For safety design considerations,
        // it is necessary to make Boost bridge conductive by default.
        boost->pwm_out_pu = float2ctrl(1);
    }

    return boost->pwm_out_pu;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_boost_ctrl_modulation(boost_ctrl_t *boost)
{
    return boost->pwm_out_pu;
}

GMP_STATIC_INLINE
void ctl_disable_boost_ctrl_output(boost_ctrl_t *boost)
{
    boost->flag_enable_output = 0;
}

GMP_STATIC_INLINE
void ctl_enable_boost_ctrl_output(boost_ctrl_t *boost)
{
    boost->flag_enable_output = 1;
}

GMP_STATIC_INLINE
void ctl_clear_boost_ctrl(boost_ctrl_t *boost)
{
    ctl_clear_pid(&boost->voltage_pid);
    ctl_clear_pid(&boost->current_pid);

    boost->current_ff = 0;
    boost->voltage_ff = 0;

    ctl_disable_boost_ctrl_output(boost);
}

// .....................................................................//
// Voltage mode
//

// BOOST controller run in voltage mode,
// user should specify voltage by function ctl_set_boost_ctrl_voltage
GMP_STATIC_INLINE
void ctl_boost_ctrl_voltage_mode(boost_ctrl_t *boost)
{
    // enable controller
    boost->flag_enable_system = 1;

    // enable output
    // boost->flag_enable_output = 1;

    // enable current controller
    boost->flag_enable_current_ctrl = 1;

    // enable voltage controller
    boost->flag_enable_voltage_ctrl = 1;
}

// Set motor target v alpha and v beta.
// only in voltage mode this function counts.
GMP_STATIC_INLINE
void ctl_set_boost_ctrl_voltage(boost_ctrl_t *boost, ctrl_gt v_set)
{
    boost->voltage_set = v_set;
}

// .....................................................................//
// Current mode
//

// BOOST controller run in current mode,
// user should specify current by function ctl_set_boost_ctrl_current
GMP_STATIC_INLINE
void ctl_boost_ctrl_current_mode(boost_ctrl_t *boost)
{
    // enable controller
    boost->flag_enable_system = 1;

    // enable output
    // boost->flag_enable_output = 1;

    // enable current controller
    boost->flag_enable_current_ctrl = 1;

    // enable voltage controller
    boost->flag_enable_voltage_ctrl = 0;
}

// Set motor target current.
// only in current mode this function counts.
GMP_STATIC_INLINE
void ctl_set_boost_ctrl_current(boost_ctrl_t *boost, ctrl_gt i_set)
{
    boost->current_set = i_set;
}

// .....................................................................//
// Voltage open loop mode
//

// BOOST controller run in open loop voltage mode,
// user should specify voltage by function ctl_set_boost_ctrl_voltage_openloop
GMP_STATIC_INLINE
void ctl_boost_ctrl_openloop_mode(boost_ctrl_t *boost)
{
    // enable controller
    boost->flag_enable_system = 1;

    // enable output
    // boost->flag_enable_output = 1;

    // enable current controller
    boost->flag_enable_current_ctrl = 0;

    // enable voltage controller
    boost->flag_enable_voltage_ctrl = 0;
}

// Set motor target Voltage (Duty).
// only in Voltage open loop mode this function counts.
GMP_STATIC_INLINE
void ctl_set_boost_ctrl_voltage_openloop(boost_ctrl_t *boost, ctrl_gt v_set)
{
    boost->voltage_out = v_set;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BOOST_CTRL_H_
