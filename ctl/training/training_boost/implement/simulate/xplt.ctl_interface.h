//
// THIS IS A DEMO SOURCE CODE FOR GMP LIBRARY.
//
// User should add all declarations of controller objects in this file.
//
// User should implement the Main ISR of the controller tasks.
//
// User should ensure that all the controller codes here is platform-independent.
//
// WARNING: This file must be kept in the include search path during compilation.
//

#include <ctl/component/motor_control/basic/std_sil_motor_interface.h>

#include <xplt.peripheral.h>

#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// device related functions
// Controller interface
// 
//
extern ptr_adc_channel_t uin;
extern ptr_adc_channel_t uc;
extern ptr_adc_channel_t il;
extern pwm_channel_t pwm_out;
 
// Functions without controller nano framework.
#ifndef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

// Input Callback
GMP_STATIC_INLINE
void ctl_input_callback(void)
{
    //// invoke ADC p.u. routine
    simulink_tx_buffer.monitor[1] = ctl_step_ptr_adc_channel(&il);
    simulink_tx_buffer.monitor[2] = ctl_step_ptr_adc_channel(&uin);
    simulink_tx_buffer.monitor[3] = ctl_step_ptr_adc_channel(&uc);

}

// Output Callback
GMP_STATIC_INLINE
void ctl_output_callback(void)
{
 
    simulink_tx_buffer.pwm_cmp[0] = ctl_calc_pwm_channel(&pwm_out, ctl_get_boost_ctrl_modulation(&boost_ctrl));

    simulink_tx_buffer.enable = 1;

    simulink_tx_buffer.monitor[0] = il.control_port.value;
    simulink_tx_buffer.monitor[1] = uin.control_port.value;
    simulink_tx_buffer.monitor[2] = uc.control_port.value;

    simulink_tx_buffer.monitor[3] = boost_ctrl.voltage_pid.out;
    simulink_tx_buffer.monitor[4] = boost_ctrl.current_pid.out;


    simulink_tx_buffer.dac[0] = 20;
}

// Enable Motor Controller
// Enable Output
GMP_STATIC_INLINE
void ctl_enable_output()
{
    csp_sl_enable_output();
}

// Disable Output
GMP_STATIC_INLINE
void ctl_disable_output()
{
    csp_sl_disable_output();
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

// Functions with controller nano framework

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

// Controller Nano input stage routine
GMP_STATIC_INLINE
void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
    // invoke ADC p.u. routine
    ctl_step_tri_ptr_adc_channel(&iabc);
    ctl_step_tri_ptr_adc_channel(&uabc);
    ctl_step_ptr_adc_channel(&idc);
    ctl_step_ptr_adc_channel(&udc);

    // invoke position encoder routine.
    ctl_step_autoturn_pos_encoder(&pos_enc, simulink_rx_buffer.encoder);
}

// Controller Nano output stage routine
GMP_STATIC_INLINE
void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
    ctl_calc_pwm_tri_channel(&pwm_out);

    simulink_tx_buffer.tabc[phase_A] = pwm_out.value[phase_A];
    simulink_tx_buffer.tabc[phase_B] = pwm_out.value[phase_B];
    simulink_tx_buffer.tabc[phase_C] = pwm_out.value[phase_C];

    // simulink_tx_buffer.monitor_port[0] = pmsm_ctrl.idq0.dat[phase_d];
    simulink_tx_buffer.monitor_port[0] = pmsm_ctrl.idq_set.dat[phase_q];
    simulink_tx_buffer.monitor_port[1] = pmsm_ctrl.idq0.dat[phase_q];

    simulink_tx_buffer.monitor_port[2] = pmsm_ctrl.vdq_set.dat[phase_d];
    // simulink_tx_buffer.monitor_port[3] = pmsm_ctrl.vdq_set.dat[phase_q];

    // simulink_tx_buffer.monitor_port[3] = pmsm_ctrl.mtr_interface.position->elec_position;
    simulink_tx_buffer.monitor_port[3] = pmsm_ctrl.mtr_interface.velocity->speed;
}

// Controller Request stage
GMP_STATIC_INLINE
void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

// Enable Output
GMP_STATIC_INLINE
void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_enable_output();
}

// Disable Output
GMP_STATIC_INLINE
void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_disable_output();
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_INTERFACE_H_
