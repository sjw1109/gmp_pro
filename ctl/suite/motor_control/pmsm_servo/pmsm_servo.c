
#include <ctl/suite/motor_control/pmsm_servo/pmsm_servo.h>

ec_gt ctl_init_pmsm_servo_framework(pmsm_servo_fm_t *pmsm)
{
    ctl_fm_init_nano_header(&pmsm->base);

    ctl_init_motor_current_ctrl(&pmsm->current_ctrl);
    ctl_init_track_pid(&pmsm->spd_ctrl);

    ctl_init_adc_tri_channel(&pmsm->iabc_input);
    ctl_init_adc_channel(&pmsm->udc_input);

    ctl_init_pwm_tri_channel(&pmsm->uabc);

    ctl_init_adc_bias_calibrator(&pmsm->calibrator);
    pmsm->calibrate_progress = 0;
    //pmsm->flag_calibrate_running = 0;

    // enable speed controller
    pmsm->flag_enable_spd_ctrl = 1;

    // enable current controller
    ctl_enable_motor_current_controller(&pmsm->current_ctrl);

    // set current feed forward
    pmsm->idq_ff.dat[0] = 0;
    pmsm->idq_ff.dat[1] = 0;

    return GMP_EC_OK;
}

ec_gt ctl_setup_pmsm_servo_framework(pmsm_servo_fm_t *pmsm,
                                     // target motor position encoder
                                     ctl_rotation_encif_t *pos_enc,
                                     // target motor speed encoder
                                     ctl_speed_encif_t *spd_enc,
                                     // control law frequency
                                     uint32_t ctrl_freq,
                                     // PID parameter for motor current controller
                                     ctrl_gt cur_kp, ctrl_gt cur_ki, ctrl_gt cur_kd,
                                     // PID saturation parameter for motor current controller
                                     ctrl_gt cur_out_min, ctrl_gt cur_out_max,
                                     // PID parameter for motor speed controller
                                     ctrl_gt spd_kp, ctrl_gt spd_ki, ctrl_gt spd_kd,
                                     // PID saturation parameter for motor speed controller
                                     ctrl_gt spd_out_min, ctrl_gt spd_out_max,
                                     // PID input traj parameter
                                     ctrl_gt spd_slope_min, ctrl_gt spd_slope_max,
                                     // speed controller divider
                                     uint32_t spd_div,
                                     // ADC general parameters
                                     fast_gt adc_resolution, fast_gt adc_iqn,
                                     // Current sensor parameters
                                     ctrl_gt current_adc_gain, ctrl_gt current_adc_bias,
                                     // Udc sensor parameters
                                     ctrl_gt udc_adc_gain, ctrl_gt udc_adc_bias,
                                     // PWM modulator full scale
                                     pwm_gt pwm_full_scale)
{
    ctl_fm_setup_nano_header(&pmsm->base, ctrl_freq);

    // save encoder interface
    pmsm->pos_enc = pos_enc;
    pmsm->spd_enc = spd_enc;

    ctl_setup_motor_current_ctrl(&pmsm->current_ctrl, cur_kp, cur_ki, cur_kd, cur_out_min, cur_out_max);
    ctl_setup_track_pid(&pmsm->spd_ctrl, spd_kp, spd_ki, spd_kd, spd_out_min, spd_out_max, spd_slope_min, spd_slope_max,
                        spd_div);

    ctl_setup_adc_tri_channel(&pmsm->iabc_input, current_adc_gain, current_adc_bias, adc_resolution, adc_iqn);
    ctl_setup_adc_channel(&pmsm->udc_input, udc_adc_gain, udc_adc_bias, adc_resolution, adc_iqn);

    ctl_setup_pwm_tri_channel(&pmsm->uabc, 0, pwm_full_scale);

    // ADC bias filter
    ctl_filter_IIR2_setup_t adc_bias_filter;
    adc_bias_filter.fc = 20;
    adc_bias_filter.fs = (parameter_gt)pmsm->base.ctrl_freq;
    adc_bias_filter.filter_type = FILTER_IIR2_TYPE_LOWPASS;
    adc_bias_filter.gain = (1.0);
    adc_bias_filter.q = (0.707);

    ctl_setup_adc_bias_calibrator(&pmsm->calibrator, &adc_bias_filter);

    return GMP_EC_OK;
}

void ctl_clear_pmsm_servo_framework(pmsm_servo_fm_t *pmsm)
{
    // clear current controller
    ctl_clear_motor_current_ctrl(&pmsm->current_ctrl);

    // clear speed controller
    ctl_clear_track_pid(&pmsm->spd_ctrl);

    // clear error code
    pmsm->error_code = GMP_EC_OK;
}
