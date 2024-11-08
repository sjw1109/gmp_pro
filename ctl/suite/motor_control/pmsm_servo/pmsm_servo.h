

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

// controller encoder interface
#include <ctl/component/motor_control/basic/encoder.h>

#ifndef _FILE_PMSM_SERVO_H_
#define _FILE_PMSM_SERVO_H_

#ifdef __cplusplus
extern "C"
{
#endif

    // Steps for setup a motor
    // + init and setup the motor encoder objects
    //    a struct that is derived from `ctl_speed_encif_t` to calculate speed
    //    a struct that is derived from `ctl_rotation_encif_t` to calculate position
    //      meanwhile provide electrical position
    // + initialize the PMSM servo framework,
    //    call function `ctl_init_pmsm_servo_framework`
    // + setup PMSM servo framework object
    //    call function `ctl_setup_pmsm_servo_framework`
    // + before start running the motor, call clear function to clear all the old informations.
    //    call function ``
    // + controller routine
    //    call function `ctl_input_motor_current_ctrl()` to set current sensor result

    typedef struct _tag_pmsm_servo_framework
    {
        // Controller Framework
        ctl_object_nano_t base;

        // target motor position encoder
        ctl_rotation_encif_t *pos_enc;

        // target motor speed encoder
        ctl_speed_encif_t *spd_enc;

        // motor current controller
        ctl_motor_current_ctrl_t current_ctrl;

        // motor speed controller
        fast_gt flag_enable_spd_ctrl;
        ctrl_gt spd_target;
        ctl_track_pid_t spd_ctrl;

        // ADC input
        adc_tri_channel_t iabc_input;
        adc_channel_t udc_input;

        // PWM output
        pwm_tri_channel_t uabc;

        // consultant objects

        // Monitor Objects

        // Protect Objects
        ec_gt error_code;

        // ADC Calibrate Objects
        adc_bias_calibrator_t calibrator;
        uint32_t calibrate_progress;

    } pmsm_servo_fm_t;

    ec_gt ctl_init_pmsm_servo_framework(pmsm_servo_fm_t *pmsm);

    ec_gt ctl_setup_pmsm_servo_framework(
        // handle of pmsm servo objects
        pmsm_servo_fm_t *pmsm,
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
        pwm_gt pwm_full_scale);

    void ctl_clear_pmsm_servo_framework(pmsm_servo_fm_t *pmsm);

    GMP_STATIC_INLINE
    void ctl_input_pmsm_servo_framework(pmsm_servo_fm_t *pmsm, adc_gt raw1, adc_gt raw2, adc_gt raw3)
    {
        ctl_step_adc_tri_channel(&pmsm->iabc_input, raw1, raw2, raw3);
        ctl_input_motor_current_ctrl(&pmsm->current_ctrl, &pmsm->iabc_input);
    }

    GMP_STATIC_INLINE
    void ctl_step_pmsm_servo_framework(pmsm_servo_fm_t *pmsm)
    {
        // speed feedback
        ctrl_gt speed_fbk = ctl_get_encoder_speed(pmsm->spd_enc);

        // position feedback
        ctrl_gt position_fbk = ctl_get_encoder_elec_postion(pmsm->pos_enc);

        // speed controller
        if (pmsm->flag_enable_spd_ctrl)
        {
            ctl_step_track_pid(&pmsm->spd_ctrl, pmsm->spd_target, speed_fbk);
        }

        // current controller & current transform
        ctl_step_motor_current_ctrl(&pmsm->current_ctrl, position_fbk);

        // modulation
        ctl_calc_pwm_tri_channel(&pmsm->uabc, &pmsm->current_ctrl.Tabc);
    }

    // phase : Phase_U. Phase_V, Phase_W
    // This function should be called in output stage
    GMP_STATIC_INLINE
    pwm_gt ctl_get_pmsm_servo_modulation(pmsm_servo_fm_t* pmsm, uint32_t phase)
    {
        return pmsm->uabc.value[phase];
    }

    // speed target mode
    GMP_STATIC_INLINE
    void ctl_set_pmsm_servo_spd_mode(pmsm_servo_fm_t *pmsm)
    {
        pmsm->flag_enable_spd_ctrl = 1;
        ctl_enable_motor_current_controller(&pmsm->current_ctrl);
    }

    // current target mode
    GMP_STATIC_INLINE
    void ctl_set_pmsm_servo_current_mode(pmsm_servo_fm_t *pmsm)
    {
        pmsm->flag_enable_spd_ctrl = 0;
        ctl_enable_motor_current_controller(&pmsm->current_ctrl);
    }

    // voltage target mode
    GMP_STATIC_INLINE
    void ctl_set_pmsm_servo_voltage_mode(pmsm_servo_fm_t *pmsm)
    {
        pmsm->flag_enable_spd_ctrl = 0;
        ctl_disable_motor_current_controller(&pmsm->current_ctrl);
    }

    // set voltage target
    GMP_STATIC_INLINE
    void ctl_set_pmsm_servo_ff_voltage(pmsm_servo_fm_t *pmsm, ctl_vector2_t *vdq)
    {
        pmsm->current_ctrl.vdq_ff.dat[0] = vdq->dat[0];
        pmsm->current_ctrl.vdq_ff.dat[1] = vdq->dat[1];
    }

#ifdef __cplusplus
}
#endif

#endif // _FILE_PMSM_SERVO_H_
