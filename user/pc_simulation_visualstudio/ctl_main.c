/**
 * @file ctl_main.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/ctl_core.h>
#include <gmp_core.h>
#include <math.h>

#include <ctl/suite/motor_control/pmsm_servo/pmsm_servo.h>

#include <ctl/component/motor_control/basic/encoder.h>

// const F & VF controller
#include <ctl/component/motor_control/basic/contant_vf.h>

// User may set (get handle) TX content via `gmp_csp_sl_get_tx_buffer`
// User may get (get handle) RX content via `gmp_csp_sl_get_rx_buffer`

// position encoder
ctl_pos_encoder_t pos_enc;

// speed encoder
ctl_spd_calculator_t spd_enc;

// PMSM servo objects
pmsm_servo_fm_t pmsm_servo;

// PMSM const frequency controller
ctl_const_f_controller const_f;

#define CONTROLLER_FREQUENCY (10000)

// CTL initialize routine
void ctl_init()
{
    ctl_init_pos_encoder(&pos_enc);
    ctl_init_spd_calculator(&spd_enc);

    ctl_init_pmsm_servo_framework(&pmsm_servo);

    ctl_init_const_f_controller(&const_f);

    ctl_setup_const_f_controller(&const_f, 5, CONTROLLER_FREQUENCY);

    ctl_setup_pos_encoder(&pos_enc, 1, (1 << 17) - 1);

    ctl_setup_spd_calculator(&spd_enc, CONTROLLER_FREQUENCY, 5, 3000, 1, 20, &pos_enc.encif);

    ctl_setup_pmsm_servo_framework(
        // link PMSM servo and encoder
        // PMSM servo object, position encoder, speed encoder, control law frequency
        &pmsm_servo, CTL_POSITION_IF(&const_f), CTL_SPEED_IF(&spd_enc), CONTROLLER_FREQUENCY,
        // current controller PID parameter
        // P, I, D, sat_min, sat_max
        float2ctrl(0.8), float2ctrl(0.01), 0, float2ctrl(-0.5), float2ctrl(0.5),
        // speed controller PID parameter
        // P, I, D, sat_min, sat_max
        float2ctrl(2.0), float2ctrl(0.01), 0, float2ctrl(-0.5), float2ctrl(0.5),
        // acceleration
        // negative acceleration, positive acceleration, speed controller division
        float2ctrl(-0.05), float2ctrl(0.05), 5,
        // ADC parameters
        // ADC resolution, iqn, current gain, current bias, voltage gain, voltage bias
        // NOTE iqn parameter is meaningless for float environment
        // NOTE gain is negative value means ADC result is negative
        16, 24, float2ctrl(-2.0), float2ctrl(0.5), float2ctrl(0.1), float2ctrl(0.1),
        // PWM parameters
        10000);

    gmp_base_print("PMSM SERVO struct has been inited, size :%d\r\n", sizeof(pmsm_servo_fm_t));

    ctl_setup_default_ctl_nano_obj(&pmsm_servo.base);

    ctl_set_pmsm_servo_voltage_mode(&pmsm_servo);

    ctl_fm_force_online(&pmsm_servo.base);

    ctl_vector2_t vdq_set = {float2ctrl(0.2), float2ctrl(0.2)};

    ctl_set_pmsm_servo_ff_voltage(&pmsm_servo, &vdq_set);

    // ctl_fm_init_nano_header(&ctl_obj);
    // ctl_fm_setup_nano_header(&ctl_obj, (uint32_t)10e3);

    // ctl_fm_force_online(&ctl_obj);
}

// CTL loop routine
void ctl_dispatch(void)
{

    // User Controller logic here.
}

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
    ctl_input_pmsm_servo_framework(&pmsm_servo,
                                   // current input
                                   gmp_csp_sl_get_rx_buffer()->iabc[phase_U], gmp_csp_sl_get_rx_buffer()->iabc[phase_V],
                                   gmp_csp_sl_get_rx_buffer()->iabc[phase_W]);

    ctl_step_pos_encoder(&pos_enc, gmp_csp_sl_get_rx_buffer()->encoder);
    ctl_step_spd_calc(&spd_enc);
}

void ctl_fmif_core_stage_routine(ctl_object_nano_t *pctl_obj)
{
    ctl_step_const_f_controller(&const_f);

    ctl_step_pmsm_servo_framework(&pmsm_servo);

    // modulation
}

ctl_vector2_t phasor;

void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
    // gmp_csp_sl_get_tx_buffer()->output1 = gmp_csp_sl_get_rx_buffer()->input1;

    simulink_tx_buffer.enable = 1;

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
