#include <ctl/component/interface/interface_base.h>
#include <ctl/component/intrinsic/continuous/continuous_pid.h>

#ifndef _FILE_BUCK_CTRL_H_
#define _FILE_BUCK_CTRL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// You may enable this macro to enter debug mode.
// Duty of Buck will not calculated by Uin.
#define CTL_BUCK_CTRL_OUTPUT_WITHOUT_UIN

// Boost Controller
typedef struct _tag_boost_ctrl_type
{
    //
    // Controller port
    //

    // output capacitor voltage
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

    // input filter
    ctl_low_pass_filter_t lpf_ui;
    ctl_low_pass_filter_t lpf_uo;
    ctl_low_pass_filter_t lpf_il;

    // modulate uin saturation
    // in order to avoid divided by 0
    ctl_saturation_t modulation_saturation;

    // uin after saturation
    ctrl_gt ui_sat;

    //
    // flag stack
    //

    // enable controller
    fast_gt flag_enable_system;

    // enable current controller
    fast_gt flag_enable_current_ctrl;

    // enable voltage controller
    fast_gt flag_enable_voltage_ctrl;

} buck_ctrl_t;

void ctl_init_buck_ctrl(
    // Buck controller
    buck_ctrl_t *buck,
    // Voltage PID controller
    parameter_gt v_kp, parameter_gt v_Ti, parameter_gt v_Td,
    // Current PID controller
    parameter_gt i_kp, parameter_gt i_Ti, parameter_gt i_Td,
    // valid uin range
    parameter_gt uin_min, parameter_gt uin_max,
    // Controller frequency, Hz
    parameter_gt fs);

void ctl_attach_buck_ctrl_input(
    // Buck controller
    buck_ctrl_t *buck,
    // output capacitor voltage
    adc_ift *uo,
    // inductor current
    adc_ift *il,
    // input voltage
    adc_ift *uin);

GMP_STATIC_INLINE
void ctl_clear_buck_ctrl(buck_ctrl_t *buck)
{
    ctl_clear_pid(&buck->current_pid);
    ctl_clear_pid(&buck->voltage_pid);
}

GMP_STATIC_INLINE
ctrl_gt ctl_step_buck_ctrl(buck_ctrl_t *buck)
{
    ctl_step_lowpass_filter(&buck->lpf_il, buck->adc_il->value);
    ctl_step_lowpass_filter(&buck->lpf_uo, buck->adc_uo->value);
    ctl_step_lowpass_filter(&buck->lpf_ui, buck->adc_ui->value);

    if (buck->flag_enable_system)
    {

        if (buck->flag_enable_voltage_ctrl)
        {
            buck->current_set =
                ctl_step_pid_ser(&buck->voltage_pid, buck->voltage_set - buck->lpf_uo->value) + buck->current_ff;
        }

        if (buck->flag_enable_current_ctrl)
        {
            buck->voltage_out =
                ctl_step_pid_ser(&buck->current_pid, buck->current_set - buck->lpf_il->value) + buck->voltage_ff;
        }

#ifdef CTL_BUCK_CTRL_OUTPUT_WITHOUT_UIN
        // Boost duty is generated without input voltage
        buck->pwm_out_pu = buck->voltage_out;
#else
        // saturation uin to avoid divided by 0
        buck->ui_sat = ctl_step_saturation(&buck->modulation_saturation, buck->lpf_ui.out);
        buck->pwm_out_pu = ctl_div(buck->voltage_out, buck->ui_sat);

#endif // CTL_BUCK_CTRL_OUTPUT_WITHOUT_UIN
    }
    else
    {
        // For safety design considerations,
        // it is necessary to make Buck bridge close by default.
        buck->pwm_out_pu = float2ctrl(0);
    }

    return buck->pwm_out_pu;
}

GMP_STATIC_INLINE
void ctl_set_buck_openloop_mode(buck_ctrl_t *buck)
{
    buck->flag_enable_system = 0;

    buck->flag_enable_current_ctrl = 0;
    buck->flag_enable_voltage_ctrl = 0;
}

GMP_STATIC_INLINE
void ctl_set_buck_uo_openloop(buck_ctrl_t *buck, ctrl_gt vo)
{
    buck->voltage_out = vo;
}

GMP_STATIC_INLINE
void ctl_set_buck_current_loop_mode(buck_ctrl_t *buck)
{
    buck->flag_enable_system = 0;

    buck->flag_enable_current_ctrl = 1;
    buck->flag_enable_voltage_ctrl = 0;
}

GMP_STATIC_INLINE
void ctl_set_buck_il(buck_ctrl_t *buck, ctrl_gt il)
{
    buck->current_set = il;
}

GMP_STATIC_INLINE
void ctl_set_buck_voltage_loop_mode(buck_ctrl_t *buck)
{
    buck->flag_enable_system = 0;

    buck->flag_enable_current_ctrl = 1;
    buck->flag_enable_voltage_ctrl = 1;
}

GMP_STATIC_INLINE
void ctl_set_buck_uo(buck_ctrl_t *buck, ctrl_gt uo)
{
    buck->voltage_set = uo;
}

GMP_STATIC_INLINE
void ctl_enable_buck_ctrl(buck_ctrl_t *buck)
{
    buck->flag_enable_system = 1;
}

GMP_STATIC_INLINE
void ctl_disable_buck_ctrl(buck_ctrl_t *buck)
{
    buck->flag_enable_system = 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BUCK_CTRL_H_
