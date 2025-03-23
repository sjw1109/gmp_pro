

#include <ctl/component/motor_control/basic/std_sil_motor_interface.h>

#include "peripheral.h"

#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // device related functions


#ifndef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

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

        //simulink_tx_buffer.monitor_port[0] = pmsm_ctrl.idq0.dat[phase_d];
        simulink_tx_buffer.monitor_port[0] = pmsm_ctrl.idq_set.dat[phase_q];
        simulink_tx_buffer.monitor_port[1] = pmsm_ctrl.idq0.dat[phase_q];

        simulink_tx_buffer.monitor_port[2] = pmsm_ctrl.vdq_set.dat[phase_d];
        //simulink_tx_buffer.monitor_port[3] = pmsm_ctrl.vdq_set.dat[phase_q];
        
        
        //simulink_tx_buffer.monitor_port[3] = pmsm_ctrl.mtr_interface.position->elec_position;
        simulink_tx_buffer.monitor_port[3] = pmsm_ctrl.mtr_interface.velocity->speed;
            

    }

#else // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

GMP_STATIC_INLINE
void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
    pmsm_fm_t *pmsm = (pmsm_fm_t *)pctl_obj;

    ctl_input_pmsm_framework(pmsm, gmp_csp_sl_get_rx_buffer()->iabc[phase_A], gmp_csp_sl_get_rx_buffer()->iabc[phase_B],
                             gmp_csp_sl_get_rx_buffer()->iabc[phase_C]);

    ctl_step_adc_tri_channel(&pmsm->vabc_input, gmp_csp_sl_get_rx_buffer()->uabc[phase_A],
                             gmp_csp_sl_get_rx_buffer()->uabc[phase_B], gmp_csp_sl_get_rx_buffer()->uabc[phase_C]);

    ctl_step_pos_encoder(&pos_enc, gmp_csp_sl_get_rx_buffer()->encoder);
}

GMP_STATIC_INLINE
void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
    pmsm_fm_t *pmsm = (pmsm_fm_t *)pctl_obj;

    // Write PWM compare register
    gmp_csp_sl_get_tx_buffer()->tabc[phase_A] = ctl_get_pmsm_modulation(pmsm, phase_A);
    gmp_csp_sl_get_tx_buffer()->tabc[phase_B] = ctl_get_pmsm_modulation(pmsm, phase_B);
    gmp_csp_sl_get_tx_buffer()->tabc[phase_C] = ctl_get_pmsm_modulation(pmsm, phase_C);

#if BUILD_LEVEL == 1
    gmp_csp_sl_get_tx_buffer()->monitor_port[0] = pmsm->pos_enc->position;
    // gmp_csp_sl_get_tx_buffer()->monitor_port[1] = pmsm->pos_enc->elec_position;
    // gmp_csp_sl_get_tx_buffer()->monitor_port[0] = pos_enc.encif.position;
    gmp_csp_sl_get_tx_buffer()->monitor_port[1] = pos_enc.encif.elec_position;
    gmp_csp_sl_get_tx_buffer()->monitor_port[2] = pmsm->current_ctrl.idq0.dat[phase_d];
    gmp_csp_sl_get_tx_buffer()->monitor_port[3] = pmsm->current_ctrl.idq0.dat[phase_q];
#elif BUILD_LEVEL == 2
    gmp_csp_sl_get_tx_buffer()->monitor_port[0] = pos_enc.encif.position;
    gmp_csp_sl_get_tx_buffer()->monitor_port[1] = spd_enc.encif.speed;
    gmp_csp_sl_get_tx_buffer()->monitor_port[2] = pmsm->current_ctrl.idq0.dat[phase_d];
    gmp_csp_sl_get_tx_buffer()->monitor_port[3] = pmsm->current_ctrl.idq0.dat[phase_q];

#elif BUILD_LEVEL == 3

    gmp_csp_sl_get_tx_buffer()->monitor_port[2] = pmsm->current_ctrl.idq0.dat[phase_d];
    gmp_csp_sl_get_tx_buffer()->monitor_port[3] = pmsm->current_ctrl.idq0.dat[phase_q];

#elif BUILD_LEVEL == 4
    gmp_csp_sl_get_tx_buffer()->monitor_port[0] = pmsm->pos_enc->position;
    gmp_csp_sl_get_tx_buffer()->monitor_port[1] = pmsm->spd_enc->speed;
    gmp_csp_sl_get_tx_buffer()->monitor_port[2] = pmsm->current_ctrl.idq0.dat[phase_d];
    gmp_csp_sl_get_tx_buffer()->monitor_port[3] = pmsm->current_ctrl.idq0.dat[phase_q];

#endif // BUILD_LEVEL
}

GMP_STATIC_INLINE
void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

GMP_STATIC_INLINE
void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_enable_output();
}

GMP_STATIC_INLINE
void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_disable_output();
}

GMP_STATIC_INLINE
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
