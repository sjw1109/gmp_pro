

#include <gmp_core.h>

#include <ctl/ctl_core.h>

// Controller framework
#include <ctl/framework/ctl_nano.h>

#include <ctl/component/intrinsic.h>

#include <ctl/component/motor_control.h>


//// motor controller
//#include <ctl/component/intrinsic/combo/track_pid.h>
//#include <ctl/component/motor_control/basic/motor_current_ctrl.h>
//
//// input & output interface
//#include <ctl/component/intrinsic/interface/adc_channel.h>
//#include <ctl/component/intrinsic/interface/pwm_channel.h>
//
//// consultant objects
//#include <ctl/component/motor_control/consultant/motor_driver_consultant.h>
//#include <ctl/component/motor_control/consultant/pmsm_consultant.h>
//
//// controller encoder interface
//#include <ctl/component/motor_control/basic/encoder.h>

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
        ctl_vector2_t idq_ff;

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
        // calibrator is running
        // fast_gt flag_calibrate_running;

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

    // This function should be called in CLIBRATE state
    GMP_STATIC_INLINE
    ec_gt ctl_cb_pmsm_servo_frmework_current_calibrate(pmsm_servo_fm_t *pmsm)
    {
        if (ctl_fm_is_calibrate(&pmsm->base))
        {
            if (pmsm->calibrate_progress == 0 && (ctl_is_adc_calibration_cmpt(&pmsm->calibrator)))
            {
                // init state
                ctl_restart_adc_bias_calibrator(&pmsm->calibrator);

                // pwm output enable
                ctl_fmif_output_enable(&pmsm->base);

                return GMP_EC_OK;
            }
            else if (pmsm->calibrate_progress <= 3)
            {
                // calibrating current sensor
                if (ctl_is_adc_calibration_cmpt(&pmsm->calibrator) && (ctl_is_adc_calibration_cmpt(&pmsm->calibrator)))
                {
                    if (pmsm->iabc_input.gain[pmsm->calibrate_progress] < 0)
                    {
                        // save result and restart the calibrate controller
                        ctl_set_adc_tri_channel_bias(
                            &pmsm->iabc_input, pmsm->calibrate_progress - 1,
                            ctl_get_tri_adc_bias(&pmsm->iabc_input, pmsm->calibrate_progress - 1) -
                                ctl_get_adc_bias_calibrator_result(&pmsm->calibrator));
                    }
                    else // pmsm->iabc_input.gain > 0
                    {
                        // save result and restart the calibrate controller
                        ctl_set_adc_tri_channel_bias(
                            &pmsm->iabc_input, pmsm->calibrate_progress - 1,
                            ctl_get_tri_adc_bias(&pmsm->iabc_input, pmsm->calibrate_progress - 1) +
                                ctl_get_adc_bias_calibrator_result(&pmsm->calibrator));
                    }

                    // restart calibrator
                    ctl_restart_adc_bias_calibrator(&pmsm->calibrator);

                    // pwm output enable
                    ctl_fmif_output_enable(&pmsm->base);

                    return GMP_EC_OK;
                }
            }
            else if (pmsm->calibrate_progress == 4)
            {
                if (ctl_is_adc_calibration_cmpt(&pmsm->calibrator) && (ctl_is_adc_calibration_cmpt(&pmsm->calibrator)))
                {
                    // save result
                    if (pmsm->udc_input.gain < 0)
                    {
                        ctl_set_adc_channel_bias(&pmsm->udc_input,
                                                 ctl_get_adc_channel_bias(&pmsm->udc_input) -
                                                     ctl_get_adc_bias_calibrator_result(&pmsm->calibrator));
                    }
                    else // gain > 0
                    {
                        ctl_set_adc_channel_bias(&pmsm->udc_input,
                                                 ctl_get_adc_channel_bias(&pmsm->udc_input) +
                                                     ctl_get_adc_bias_calibrator_result(&pmsm->calibrator));
                    }

                    // change to next state
                    return 1;
                }
            }
            else
            {
                // complete or escape from this calibrate stage
                // change to next state
                return 1;
            }
        }

        return GMP_EC_OK;
    }

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

        if (ctl_fm_is_online(&pmsm->base))
        {
            // speed controller
            if (pmsm->flag_enable_spd_ctrl)
            {
                ctl_step_track_pid(&pmsm->spd_ctrl, pmsm->spd_target, speed_fbk);
            }

            // set idq ref
            ctl_set_motor_current_ctrl_idq_ref(&pmsm->current_ctrl, pmsm->idq_ff.dat[phase_d],
                                               pmsm->idq_ff.dat[phase_q] + ctl_get_track_pid_output(&pmsm->spd_ctrl));

            // current controller & current transform
            ctl_step_motor_current_ctrl(&pmsm->current_ctrl, position_fbk);
        }
        else if (ctl_fm_is_calibrate(&pmsm->base))
        {
            pmsm->current_ctrl.Tabc.dat[phase_U] = float2ctrl(0.5);
            pmsm->current_ctrl.Tabc.dat[phase_V] = float2ctrl(0.5);
            pmsm->current_ctrl.Tabc.dat[phase_W] = float2ctrl(0.5);

            if (pmsm->calibrate_progress <= 2)
            {
                // current calibrate routine
                if ((!ctl_is_adc_calibration_cmpt(&pmsm->calibrator)) &&
                    ctl_step_adc_bias_calibrator(&pmsm->calibrator, pmsm->base.isr_tick,
                                                 pmsm->current_ctrl.iabc.dat[pmsm->calibrate_progress]))
                {
                    // calibrate is complete
                    pmsm->calibrate_progress += 1;

                    // pwm output disable
                    ctl_fmif_output_disable(&pmsm->base);
                }
            }
            else if (pmsm->calibrate_progress == 3)
            {
                if ((!ctl_is_adc_calibration_cmpt(&pmsm->calibrator)) &&
                    ctl_step_adc_bias_calibrator(&pmsm->calibrator, pmsm->base.isr_tick, pmsm->current_ctrl.udc))
                {
                    // calibrate is complete
                    pmsm->calibrate_progress += 1;

                    // pwm output disable
                    ctl_fmif_output_disable(&pmsm->base);
                }
            }
        }
        else if (ctl_fm_is_runup(&pmsm->base))
        {
        }
        else
        {
            ctl_set_motor_current_controller_zero_output(&pmsm->current_ctrl);
        }

        // modulation
        ctl_calc_pwm_tri_channel(&pmsm->uabc, &pmsm->current_ctrl.Tabc);
    }

    // phase : Phase_U. Phase_V, Phase_W
    // This function should be called in output stage
    GMP_STATIC_INLINE
    pwm_gt ctl_get_pmsm_servo_modulation(pmsm_servo_fm_t *pmsm, uint32_t phase)
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

    // set current target
    GMP_STATIC_INLINE
    void ctl_set_pmsm_servo_ff_current(pmsm_servo_fm_t *pmsm, ctl_vector2_t *idq)
    {
        pmsm->idq_ff.dat[phase_d] = idq->dat[phase_d];
        pmsm->idq_ff.dat[phase_q] = idq->dat[phase_q];
    }

    // Set speed target
    GMP_STATIC_INLINE
    void ctl_set_pmsm_servo_spd(pmsm_servo_fm_t *pmsm, ctrl_gt spd)
    {

        pmsm->spd_target = spd;
    }

    // Get motor mechanical position
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_pmsm_servo_rotor_pos(pmsm_servo_fm_t *pmsm)
    {
        return pmsm->pos_enc->position;
    }

    // get motor speed
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_pmsm_servo_speed(pmsm_servo_fm_t *pmsm)
    {
        return pmsm->spd_enc->speed;
    }

    // Set position signal source interface for PMSM servo object
    GMP_STATIC_INLINE
    void ctl_set_pmm_servo_pos_enc(pmsm_servo_fm_t *pmsm, ctl_rotation_encif_t *enc)
    {
        pmsm->pos_enc = enc;
    }

#ifdef __cplusplus
}
#endif

#endif // _FILE_PMSM_SERVO_H_
