
#include <ctl/component/interface/interface_base.h>
#include <ctl/component/intrinsic/continuous/continuous_pid.h>

#ifndef _FILE_BOOST_CTRL_H_
#define _FILE_BOOST_CTRL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// You may enable this macro to enter debug mode.
// Duty of Boost will not calculated by Uout.
// #define CTL_BOOST_CTRL_OUTPUT_WITHOUT_UOUT
//

// BOOST CONTROLLER Global parameters
#ifndef CTL_BOOST_CTRL_UIN_MIN
#define CTL_BOOST_CTRL_UIN_MIN ((float2ctrl(0.01)))
#endif // CTL_BOOST_CTRL_UIN_MIN

// BOOST HALF BRIDGE
// if this macro is enable,  PWM output will control upper side
// M = 1 / D
// if this macro is disabled, PWM output will control lower side
// M = 1 / (1-D)
#define CTL_BOOST_CTRL_HALF_BRIDGE

// Boost Controller
typedef struct _tag_boost_ctrl_type
{
    //
    // Controller port
    //

    // capacitor voltage
    adc_ift *adc_uo;

    // inductor current
    adc_ift *adc_il;

    // input voltage
    adc_ift *adc_ui;

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

    // input filter
    ctl_low_pass_filter_t lpf_ui;
    ctl_low_pass_filter_t lpf_uo;
    ctl_low_pass_filter_t lpf_il;

    // modulate uin saturation
    // in order to avoid divided by 0
    ctl_saturation_t modulation_saturation;

    ctrl_gt vo_sat;

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
    // valid voltage output range
    parameter_gt vo_min, parameter_gt vo_max,
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
    ctl_step_lowpass_filter(&boost->lpf_il, boost->adc_il->value);
    ctl_step_lowpass_filter(&boost->lpf_uo, boost->adc_uo->value);
    ctl_step_lowpass_filter(&boost->lpf_ui, boost->adc_ui->value);

    if (boost->flag_enable_system)
    {

        if (boost->flag_enable_voltage_ctrl)
        {
            boost->current_set =
                ctl_step_pid_ser(&boost->voltage_pid, boost->voltage_set - boost->lpf_uo.out) + boost->current_ff;
        }

        if (boost->flag_enable_current_ctrl)
        {
            boost->voltage_out =
                ctl_step_pid_ser(&boost->current_pid, boost->current_set - boost->lpf_il.out) + boost->voltage_ff;
        }

#ifdef CTL_BOOST_CTRL_OUTPUT_WITHOUT_UOUT

#if defined CTL_BOOST_CTRL_HALF_BRIDGE
        // Boost duty is generated without input voltage
        boost->pwm_out_pu = float2ctrl(1) - boost->voltage_out;
#else
        boost->pwm_out_pu = boost->voltage_out;
#endif // CTL_BOOST_CTRL_HALF_BRIDGE

#else // CTL_BOOST_CTRL_OUTPUT_WITHOUT_UOUT

        boost->vo_sat = ctl_step_saturation(&boost->modulation_saturation, boost->lpf_uo.out);
#if defined CTL_BOOST_CTRL_HALF_BRIDGE
        // upper bridge is controlled
        boost->pwm_out_pu = ctl_div(boost->lpf_ui.out, boost->vo_sat);
#else
        // lower bridge is controlled
        boost->pwm_out_pu = GMP_CONST_1 - ctl_div(boost->lpf_ui.out, boost->vo_sat);
#endif // CTL_BOOST_CTRL_HALF_BRIDGE

#endif // CTL_BOOST_CTRL_OUTPUT_WITHOUT_UOUT
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
void ctl_clear_boost_ctrl(boost_ctrl_t *boost)
{
    ctl_clear_pid(&boost->voltage_pid);
    ctl_clear_pid(&boost->current_pid);

    boost->current_ff = 0;
    boost->voltage_ff = 0;
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

GMP_STATIC_INLINE
void ctl_disable_boost_ctrl(boost_ctrl_t *boost)
{
    boost->flag_enable_system = 0;
}

GMP_STATIC_INLINE
void ctl_enable_boost_ctrl(boost_ctrl_t *boost)
{
    boost->flag_enable_system = 1;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BOOST_CTRL_H_
