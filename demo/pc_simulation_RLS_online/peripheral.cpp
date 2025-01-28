/**
 * @file peripheral_mapping.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <gmp_core.h>

#include <ctl/ctl_core.h>

#include <math.h>

#include <ctl/suite/motor_control/pmsm_servo/pmsm_servo.h>

#include <ctl/component/motor_control/basic/encoder.h>

// const F & VF controller
#include <ctl/component/motor_control/basic/vf_generator.h>

extern "C"
{

    // position encoder
    extern ctl_pos_encoder_t pos_enc;

    // speed encoder
    extern ctl_spd_calculator_t spd_enc;

    // PMSM servo objects
    extern pmsm_servo_fm_t pmsm_servo;

    // PMSM const frequency controller
    extern ctl_const_f_controller const_f;
}

//////////////////////////////////////////////////////////////////////////
// Devices on the peripheral

void setup_peripheral()
{
}

//////////////////////////////////////////////////////////////////////////
// device related functions

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
    // current sensor
    ctl_input_pmsm_servo_framework(&pmsm_servo,
                                   // current input
                                   gmp_csp_sl_get_rx_buffer()->iabc[phase_U], gmp_csp_sl_get_rx_buffer()->iabc[phase_V],
                                   gmp_csp_sl_get_rx_buffer()->iabc[phase_W]);

    // position encoder
    ctl_step_pos_encoder(&pos_enc, gmp_csp_sl_get_rx_buffer()->encoder);
    ctl_step_spd_calc(&spd_enc);
}

// ctl_vector2_t phasor;

void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
    //    ctl_fmif_output_enable(pctl_obj);

    gmp_csp_sl_get_tx_buffer()->tabc[phase_U] = ctl_get_pmsm_servo_modulation(&pmsm_servo, phase_U);
    gmp_csp_sl_get_tx_buffer()->tabc[phase_V] = ctl_get_pmsm_servo_modulation(&pmsm_servo, phase_V);
    gmp_csp_sl_get_tx_buffer()->tabc[phase_W] = ctl_get_pmsm_servo_modulation(&pmsm_servo, phase_W);

    //     gmp_csp_sl_get_tx_buffer()->monitor_port[0] = pmsm_servo.current_ctrl.Tabc.dat[0];
    //     gmp_csp_sl_get_tx_buffer()->monitor_port[1] = pmsm_servo.current_ctrl.Tabc.dat[1];

    // gmp_csp_sl_get_tx_buffer()->monitor_port[2] = gmp_csp_sl_get_rx_buffer()->encoder;
    gmp_csp_sl_get_tx_buffer()->monitor_port[0] = spd_enc.encif.speed;
    gmp_csp_sl_get_tx_buffer()->monitor_port[1] = pos_enc.encif.elec_position;

    gmp_csp_sl_get_tx_buffer()->monitor_port[2] = ctl_get_motor_current_controller_id(&pmsm_servo.current_ctrl);
    gmp_csp_sl_get_tx_buffer()->monitor_port[3] = ctl_get_motor_current_controller_iq(&pmsm_servo.current_ctrl);

    // gmp_csp_sl_get_tx_buffer()->monitor_port[0] = phasor.dat[0];
    // gmp_csp_sl_get_tx_buffer()->monitor_port[1] = phasor.dat[1];
}

void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_enable_output();
}

void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_disable_output();
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here
