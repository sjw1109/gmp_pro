

#include <gmp_core.h>

#include <ctl/ctl_core.h>

// Controller framework
#include <ctl/framework/ctl_nano.h>

// motor controller
#include <ctl/component/intrinsic/combo/track_pid.h>
#include <ctl/component/motor_control/basic/motor_current_ctrl.h>

// input & output interface
#include <ctl/component/intrinsic/interface/adc_channel.h>
#include <ctl/component/intrinsic/interface/pwm_channel.h>

// consultant objects
#include <ctl/component/motor_control/consultant/motor_driver_consultant.h>
#include <ctl/component/motor_control/consultant/pmsm_consultant.h>

#ifndef _FILE_PMSM_SERVO_H_
#define _FILE_PMSM_SERVO_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _tag_pmsm_servo_framework
    {
        // Controller Framework
        ctl_object_nano_t base;

        // motor current controller
        ctl_motor_current_ctrl_t current_ctrl;

        // motor speed controller
        ctl_track_pid_t spd_ctrl;

        // ADC input
        adc_tri_channel_t iabc_input;
        adc_channel_t udc_input;

        // PWM output
        pwm_tri_channel_t uabc;

        // consultant objects

        // Monitor Objects

        // Protect Objects

        // ADC Calibrate Objects
        adc_bias_calibrator_t calibrator;
        uint32_t calibrate_progress;

    } pmsm_servo_fm_t;

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

        return GMP_EC_OK;
    }

    ec_gt ctl_setup_pmsm_servo_framework(pmsm_servo_fm_t *pmsm,
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

        ctl_setup_motor_current_ctrl(&pmsm->current_ctrl, cur_kp, cur_ki, cur_kd, cur_out_min, cur_out_max);
        ctl_setup_track_pid(&pmsm->spd_ctrl, spd_kp, spd_ki, spd_kd, spd_out_min, spd_out_max, spd_slope_min,
                            spd_slope_max, spd_div);

        ctl_setup_adc_tri_channel(&pmsm->iabc_input, current_adc_gain, current_adc_bias, adc_resolution, adc_iqn);
        ctl_setup_adc_channel(&pmsm->udc_input, udc_adc_gain, udc_adc_bias, adc_resolution, adc_iqn);

        ctl_setup_pwm_tri_channel(&pmsm->uabc, 0, pwm_full_scale);

        // ADC bias filter
        ctl_filter_IIR2_setup_t adc_bias_filter;
        adc_bias_filter.fc = 20;
        adc_bias_filter.fs = (parameter_gt)pmsm->base.ctrl_freq;
        adc_bias_filter.filter_type = FILTER_IIR2_TYPE_LOWPASS;
        adc_bias_filter.gain = float2ctrl(1.0);
        adc_bias_filter.q = float2ctrl(0.707);

        ctl_setup_adc_bias_calibrator(&pmsm->calibrator, &adc_bias_filter);

        return GMP_EC_OK;
    }

#ifdef __cplusplus
}
#endif

#endif // _FILE_PMSM_SERVO_H_
