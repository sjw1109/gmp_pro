
// This file provide a set of algorithm to calibrate encoders.

#include <ctl/component/motor_control/basic/encoder_if.h>
#include <ctl/component/motor_control/basic/motor_current_ctrl.h>

#ifndef _FILE_ENCODER_CALIBRATE_H_
#define _FILE_ENCODER_CALIBRATE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    ////////////////////////////////////////////////////////////////////////
    // Absolute Position Encoder calibrate

    typedef struct _tag_position_enc_calibrate
    {
        // output: encoder calibrate result
        ctrl_gt offset;

        // target motor current controller
        ctl_motor_current_ctrl_t *mc;

        // target motor position encoder
        ctl_rotation_encif_t *pos_enc;



        // parameters:

        // when running position calibrate task,
        // this current may treated as the target
        ctrl_gt current_target;

        // when current exceed the limit current
        // the controller output would disable
        ctrl_gt current_limit;

        // target position error
        ctrl_gt position_delta_target;

        // old motor position
        ctrl_gt old_position;

        // convergence flag
        fast_gt flag_position_convergence;

        // state machine flag
        // each bit is count,
        // 0 is meet error constrain,
        // 1 is doesn't meet error constrain.
        fast_gt flag_stage1;

        // state machine flag
        // records the error convergence time
        time_gt switch_time;

    } ctl_position_enc_calibrate_t;

    ec_gt ctl_task_position_encoder_offset_calibrate(ctl_position_enc_calibrate_t *obj)
    {
        // Step I: set current target & voltage target
        ctl_set_motor_current_ctrl_idq_ref(&obj->mc, obj->current_target, 0);
        ctl_set_motor_current_ctrl_vdq_ff(&obj->mc, 0, 0);

        // TODO: Safe routine
        // judge if current is over current limit

        // Step II: wait till encoder output is stable.
        // encoder output delta is less than position delta target (default is 0.5%),
        // set `flag_position_convergence = 1`
        if ((obj->flag_stage1 == 0))
        {
            // judge position convergency
            if (ctl_get_encoder_position(obj->encoder) - obj->old_position < obj->position_delta_target)
            {
                obj->flag_position_convergence = obj->flag_position_convergence << 1 | 0x00;
            }
            else // position still not convergency
            {
                obj->flag_position_convergence = obj->flag_position_convergence << 1 | 0x01;
            }

            // save old position
            obj->old_position = ctl_get_encoder_position(obj->encoder);

            // judge position convergence sequence is full
            if ((obj->flag_position_convergence & 0x0F) == 0)
            {
                obj->switch_time = gmp_base_get_system_tick();
                obj->flag_stage1 = 1;
            }

            // Still running
            return GMP_EC_OK;
        }

        // Step III: wait to infinity
        // after output delta is satisfied the target, wait another 1 second.
        else
        {
            if (gmp_base_get_system_tick() - obj->switch_time)
            {
                // offset
                ctrl_gt offset = obj->encoder->offset + ctl_get_encoder_position(obj->encoder);
                offset = ctl_sat_1(offset);

                // save valibrate result
                obj->encoder->offset = offset;

                // complete task
                return 1;
            }
        }
    }

    ec_gt ctl_clear_position_encoder_calibrator(ctl_position_enc_calibrate_t *obj)
    {
        obj->old_position = 0;
        obj->flag_position_convergence = ~0;
        obj->flag_stage1 = 0;

        ctl_set_motor_current_ctrl_idq_ref(&obj->mc, 0, 0);
        ctl_set_motor_current_ctrl_vdq_ff(&obj->mc, 0, 0);
    }

#ifdef __cplusplus
}
#endif // __cplusplus
