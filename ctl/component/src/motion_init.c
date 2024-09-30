/**
 * @file motion_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */


#include <core/gmp_core.h>
#include <ctl/ctl_core.h>


//////////////////////////////////////////////////////////////////////////
// Basic Pos loop

#include <ctl/component/motion/basic_pos_loop_p.h>

void ctl_init_pos_loop_p_ctrl(ctl_pos_loop_p_ctrl_t* pos_ctrl)
{
	ctl_init_divider(&pos_ctrl->div);

	pos_ctrl->target_pos = 0;
	pos_ctrl->target_ang = 0;

	pos_ctrl->actual_pos = 0;
	pos_ctrl->actual_ang = 0;

	pos_ctrl->kp = CTRL_T(10.0);
	pos_ctrl->speed_limit = GMP_CONST_1;
}

void ctl_setup_pos_loop_p_ctrl(ctl_pos_loop_p_ctrl_t* pos_ctrl,
	ctrl_gt kp, ctrl_gt speed_limit, uint32_t division)
{
	pos_ctrl->kp = kp;
	pos_ctrl->speed_limit = speed_limit;
	ctl_setup_divider(&pos_ctrl->div, division);
}


//////////////////////////////////////////////////////////////////////////
// Knob Wrapper

#include <ctl/component/motion/knob_pos_loop.h>

void ctl_init_knob_pos_loop(ctl_pmsm_knob_pos_loop* knob)
{
	knob->target_pos = 0;
	knob->target_ang = 0;

	knob->actual_pos = 0;
	knob->actual_ang = 0;

	knob->kp = CTRL_T(0.15);
	knob->speed_limit = CTRL_T(0.1);

	knob->knob_step = 4;
	knob->output_pos = 0;
}

void ctl_setup_knob_pos_loop(ctl_pmsm_knob_pos_loop* knob,
	ctrl_gt kp, ctrl_gt current_limit, uint16_t knob_step,
  uint32_t division)
{
	knob->kp = kp;
	knob->speed_limit = current_limit;
	knob->knob_step = knob_step;
	ctl_setup_divider(&knob->div, division); 
}
