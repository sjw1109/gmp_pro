#include <gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// Buck Control
#include <ctl/component/digital_power/basic/buck.h>

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
    parameter_gt fs)
{
    ctl_disable_buck_ctrl(buck);

    ctl_init_saturation(&buck->modulation_saturation, uin_min, uin_max);
    ctl_init_pid_ser(&buck->current_pid, i_kp, i_Ti, i_Td, fs);
    ctl_init_pid_ser(&buck->voltage_pid, v_kp, v_Ti, v_Td, fs);

    ctl_clear_buck_ctrl(buck);
}

void ctl_attach_buck_ctrl_input(
    // Buck controller
    buck_ctrl_t *buck,
    // output capacitor voltage
    adc_ift *uo,
    // inductor current
    adc_ift *il,
    // input voltage
    adc_ift *uin)
{
    buck->adc_il = il;
    buck->adc_ui = uin;
    buck->adc_uo = uo;
}

//////////////////////////////////////////////////////////////////////////
// BOOST Control
#include <ctl/component/digital_power/basic/boost.h>

void ctl_init_boost_ctrl(
    // Boost controller
    boost_ctrl_t *boost,
    // Voltage PID controller
    parameter_gt v_kp, parameter_gt v_Ti, parameter_gt v_Td,
    // Current PID controller
    parameter_gt i_kp, parameter_gt i_Ti, parameter_gt i_Td,
    // valid voltage input range
    parameter_gt vo_min, parameter_gt vo_max,
    // Controller frequency, Hz
    parameter_gt fs)
{
    ctl_disable_boost_ctrl(boost);

    ctl_init_pid(
        // continuous PID handle
        &boost->current_pid,
        // PID parameters
        v_kp, v_Ti, v_Td,
        // controller frequency
        fs);

    ctl_init_pid(
        // continuous PID handle
        &boost->voltage_pid,
        // PID parameters
        i_kp, i_Ti, i_Td,
        // controller frequency
        fs);

    ctl_init_saturation(&boost->modulation_saturation, vo_min, vo_max);

    ctl_clear_boost_ctrl(boost);
}

void ctl_attach_boost_ctrl_input(
    // Boost controller
    boost_ctrl_t *boost,
    // output capacitor voltage
    adc_ift *uc_port,
    // inductor current
    adc_ift *il_port,
    // input voltage
    adc_ift *uin_port)
{
    boost->adc_uo = uc_port;
    boost->adc_il = il_port;
    boost->adc_ui = uin_port;
}

//////////////////////////////////////////////////////////////////////////
// Protection strategy

#include <ctl/component/digital_power/basic/protectoion_strategy.h>

void ctl_attach_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj,
    // output voltage
    adc_ift *uo,
    // output current
    adc_ift *io)
{
    obj->adc_io = io;
    obj->adc_uo = uo;
}

void ctl_init_vip_protection(
    // Voltage - Current - Power Protection
    std_vip_protection_t *obj,
    // Power measurement filter cut frequency
    parameter_gt power_f_cut,
    // Voltage measurement filter cut frequency
    parameter_gt voltage_f_cut,
    // Current measurement filter cut frequency
    parameter_gt current_f_cut,
    // voltage maximum, voltage base value
    parameter_gt v_max, parameter_gt v_base,
    // current maximum, current base value
    parameter_gt i_max, parameter_gt i_base,
    // power maximum
    parameter_gt p_max,
    // sample frequency
    parameter_gt fs)
{
    obj->voltage_max = float2ctrl(v_max / v_base);
    obj->current_max = float2ctrl(i_max / i_base);
    obj->power_max = float2ctrl(p_max / v_base / i_base);

    ctl_init_lp_filter(&obj->power_filter, fs, power_f_cut);
    ctl_init_lp_filter(&obj->voltage_filter, fs, voltage_f_cut);
    ctl_init_lp_filter(&obj->current_filter, fs, current_f_cut);
}
