/**
 * @file basic_pos_loop_p.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
// This is a position loop P controller

#include <ctl/component/common/divider.h>


#ifndef _FILE_BASIC_SPEED_LOOP_P_H_
#define _FILE_BASIC_SPEED_LOOP_P_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef struct _tag_pos_loop_p_ctrl
	{
		// output speed ref
		ctrl_gt speed_ref;

		// divider
		ctl_divider_t div;

		ctrl_gt kp;

		ctrl_gt speed_limit;

		// target position
		int32_t target_pos;
		ctrl_gt target_ang;

		// actual feedback position
		int32_t actual_pos;
		ctrl_gt actual_ang;

	}ctl_pos_loop_p_ctrl_t;

	void ctl_init_pos_loop_p_ctrl(ctl_pos_loop_p_ctrl_t* pos_ctrl);

	void ctl_setup_pos_loop_p_ctrl(ctl_pos_loop_p_ctrl_t* pos_ctrl,
		ctrl_gt kp, ctrl_gt speed_limit, uint32_t division);

	GMP_STATIC_INLINE
		void ctl_set_target_position_for_pos_loop_p_ctrl(
			ctl_pos_loop_p_ctrl_t* pos_ctrl,
			int32_t target_pos, ctrl_gt target_ang)
	{
		pos_ctrl->target_pos = target_pos;
		pos_ctrl->target_ang = target_ang;
	}

	GMP_STATIC_INLINE
		void ctl_step_pos_loop_p_ctrl(ctl_pos_loop_p_ctrl_t* pos_ctrl)
	{
		int32_t delta_pos = pos_ctrl->target_pos - pos_ctrl->actual_pos;
		ctrl_gt position_error;
		
		if (ctl_step_divider(&pos_ctrl->div))
		{
			if (delta_pos > 50)
			{
				pos_ctrl->speed_ref = pos_ctrl->speed_limit;
			}
			else if (delta_pos < -50)
			{
				pos_ctrl->speed_ref = -pos_ctrl->speed_limit;
			}
			else
			{
				position_error = GMP_CONST_2_PI * delta_pos
					+ pos_ctrl->target_ang - pos_ctrl->actual_ang;

				pos_ctrl->speed_ref = ctrl_mpy(position_error, pos_ctrl->kp);

				pos_ctrl->speed_ref = ctrl_sat(pos_ctrl->speed_ref,
					pos_ctrl->speed_limit, -pos_ctrl->speed_limit);
			}

		}
	}

	// For multi-turn encoder
	GMP_STATIC_INLINE
		void ctl_input_pos_loop_p_ctrl(ctl_pos_loop_p_ctrl_t* pos_ctrl,
			int32_t actual_pos, ctrl_gt actual_ang)
	{
		pos_ctrl->actual_pos = actual_pos;
		pos_ctrl->actual_ang = actual_ang;
	}

	// For single-turn encoder
	GMP_STATIC_INLINE
		void ctl_input_pos_loop_p_ctrl_via_only_ang(ctl_pos_loop_p_ctrl_t* pos_ctrl,
			ctrl_gt actual_ang)
	{
		ctrl_gt delta_ang = actual_ang - pos_ctrl->actual_ang;
		pos_ctrl->actual_ang = actual_ang;

		// direction correction
		if (delta_ang < -GMP_CONST_1_OVER_2)
		{
			pos_ctrl->actual_pos += 1;
		}
		else if (delta_ang > GMP_CONST_1_OVER_2)
		{
			pos_ctrl->actual_pos -= 1;
		}
	}

	GMP_STATIC_INLINE
	ctrl_gt ctl_get_pos_loop_p_ctrl_target_spd(ctl_pos_loop_p_ctrl_t* pos_ctrl)
	{
		return pos_ctrl->speed_ref;
	}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BASIC_SPEED_LOOP_P_H_
