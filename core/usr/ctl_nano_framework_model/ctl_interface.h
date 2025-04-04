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

#ifndef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
    // Functions without controller nano framework.
    GMP_STATIC_INLINE
    void ctl_input_callback(void)
    {
        // invoke ADC p.u. routine
        ctl_step_tri_ptr_adc_channel(&iabc);
        ctl_step_tri_ptr_adc_channel(&uabc);
        ctl_step_ptr_adc_channel(&idc);
        ctl_step_ptr_adc_channel(&udc);

        // invoke position encoder routine.
        ctl_step_autoturn_pos_encoder(&pos_enc, simulink_rx_buffer.encoder);
    }

    extern ctrl_gt param_test;
    GMP_STATIC_INLINE
    void ctl_output_callback(void)
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
#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

    // Functions with controller nano framework

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

    void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
    {
    }

    fast_gt ctl_fmif_security_routine(ctl_object_nano_t *pctl_obj)
    {
        // not implement
        return GMP_EC_OK;
    }

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_INTERFACE_H_
