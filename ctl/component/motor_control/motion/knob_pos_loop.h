/**
 * @file knob_pos_loop.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// This is a position loop P controller

#include <ctl/component/motor_control/motion/basic_pos_loop_p.h>

#ifndef _FILE_KNOB_POS_LOOP_H_
#define _FILE_KNOB_POS_LOOP_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_pmsm_knob_pos_loop
{
    // output position tick
    int16_t output_pos;

    // knob step
    uint16_t knob_step;

    // position controller
    //		ctl_pos_loop_p_ctrl_t pos_ctrl;

    // target position
    int32_t target_pos;
    ctrl_gt target_ang;

    // actual feedback position
    int32_t actual_pos;
    ctrl_gt actual_ang;

    // current gain
    ctrl_gt kp;

    ctrl_gt speed_limit;

    // output current
    ctrl_gt speed_ref;

    // divider
    ctl_divider_t div;

} ctl_pmsm_knob_pos_loop;

void ctl_init_knob_pos_loop(ctl_pmsm_knob_pos_loop *knob);

void ctl_setup_knob_pos_loop(ctl_pmsm_knob_pos_loop *knob, ctrl_gt kp, ctrl_gt current_limit, uint16_t knob_step,
                             uint32_t division);

GMP_STATIC_INLINE
void ctl_step_knob_pos_loop(ctl_pmsm_knob_pos_loop *knob)
{
    if (ctl_step_divider(&knob->div))
    {
        // Step 0: Calculate half step length
        ctrl_gt knob_step = GMP_CONST_1 / knob->knob_step;
        ctrl_gt knob_half_step = knob_step / 2;

        // Step I: Get Target Position
        knob->output_pos = (int16_t)(knob->actual_ang / knob_step);

        knob->target_ang = knob->output_pos * knob_step + knob_half_step;
        knob->target_pos = knob->actual_pos;

        // step II: invoke pos controller
        int32_t delta_pos = knob->target_pos - knob->actual_pos;

        delta_pos = delta_pos > 2 ? 2 : delta_pos;
        delta_pos = delta_pos < -2 ? -2 : delta_pos;

        ctrl_gt position_error = GMP_CONST_2_PI * delta_pos + knob->target_ang - knob->actual_ang;

        knob->speed_ref = ctl_mul(knob->kp, position_error);

        knob->speed_ref = ctl_sat(knob->speed_ref, knob->speed_limit, -knob->speed_limit);
    }
}

GMP_STATIC_INLINE
void ctl_input_knob_pos(ctl_pmsm_knob_pos_loop *knob, int32_t actual_pos, ctrl_gt actual_ang)
{
    knob->actual_pos = actual_pos;
    knob->actual_ang = actual_ang;
}

GMP_STATIC_INLINE
void ctl_input_knob_pos_via_only_ang(ctl_pmsm_knob_pos_loop *knob, ctrl_gt actual_ang)
{
    ctrl_gt delta_ang = actual_ang - knob->actual_ang;
    knob->actual_ang = actual_ang;

    // direction correction
    if (delta_ang < -GMP_CONST_1_OVER_2)
    {
        knob->actual_pos += 1;
    }
    else if (delta_ang > GMP_CONST_1_OVER_2)
    {
        knob->actual_pos -= 1;
    }
}

GMP_STATIC_INLINE
int16_t ctl_get_knob_index(ctl_pmsm_knob_pos_loop *knob)
{
    return knob->output_pos;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_knob_target_speed(ctl_pmsm_knob_pos_loop *knob)
{
    return knob->speed_ref;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_knob_target_angle(ctl_pmsm_knob_pos_loop *knob)
{
    return knob->target_ang;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_KNOB_POS_LOOP_H_
