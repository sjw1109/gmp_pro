
#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

// Given 3.3V voltage reference
#define ADC_REFERENCE ((3.3))

// Controller Frequency
#define CONTROLLER_FREQUENCY (10000)

// PWM depth
#define GONTROLLER_PWM_CMP_MAX (6000)



// User may set (get handle) TX content via `gmp_csp_sl_get_tx_buffer`
// User may get (get handle) RX content via `gmp_csp_sl_get_rx_buffer`

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // device related functions

    extern ctl_pos_encoder_t pos_enc;

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

    GMP_STATIC_INLINE
    void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
    {
        pmsm_servo_fm_t *pmsm = (pmsm_servo_fm_t *)pctl_obj;

        ctl_input_pmsm_servo_framework(pmsm, gmp_csp_sl_get_rx_buffer()->iabc[phase_A],
            gmp_csp_sl_get_rx_buffer()->iabc[phase_B], gmp_csp_sl_get_rx_buffer()->iabc[phase_C]);

        ctl_step_adc_tri_channel(&pmsm->vabc_input, gmp_csp_sl_get_rx_buffer()->uabc[phase_A],
                                 gmp_csp_sl_get_rx_buffer()->uabc[phase_B], gmp_csp_sl_get_rx_buffer()->uabc[phase_C]);

        ctl_step_pos_encoder(&pos_enc, gmp_csp_sl_get_rx_buffer()->encoder);
    }

    GMP_STATIC_INLINE
    void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
    {
        pmsm_servo_fm_t *pmsm = (pmsm_servo_fm_t *)pctl_obj;

        // Write PWM compare register
        gmp_csp_sl_get_tx_buffer()->tabc[phase_A] = ctl_get_pmsm_servo_modulation(pmsm, phase_A);
        gmp_csp_sl_get_tx_buffer()->tabc[phase_B] = ctl_get_pmsm_servo_modulation(pmsm, phase_B);
        gmp_csp_sl_get_tx_buffer()->tabc[phase_C] = ctl_get_pmsm_servo_modulation(pmsm, phase_C);

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
