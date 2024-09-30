/**
 * @file track_pid.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// This is a PID basic package

#include <ctl/component/common/pid.h>
#include <ctl/component/common/divider.h>
#include <ctl/component/common/slope_lim.h>

#ifndef _FILE_TRACK_PID_H_
#define _FILE_TRACK_PID_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef struct _tag_track_pid_t
{
	ctl_pid_t pid;

	ctl_divider_t div;
	ctl_slope_lim_t traj;

	ctrl_gt out;
}track_pid_t;

void ctl_init_track_pid(track_pid_t* tp);

void ctl_setup_track_pid(track_pid_t* tp,
	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // pid parameters
	ctrl_gt sat_min, ctrl_gt sat_max, // saturation limit
	ctrl_gt slope_min, ctrl_gt slope_max, // slope limit
	uint32_t division //division factor
);

GMP_STATIC_INLINE
void ctl_step_track_pid(track_pid_t* tp,
	ctrl_gt target, ctrl_gt now)
{
	if (ctl_step_divider(&tp->div))
	{
		ctl_step_slope_limit(&tp->traj, target);

		ctl_step_pid_ser(&tp->pid,
			tp->traj.out - now);

		tp->out = tp->pid.out;
	}
}

GMP_STATIC_INLINE
void ctl_enable_track_pid(track_pid_t* tp)
{
	tp->div.flag_bypass = 0;
}

GMP_STATIC_INLINE
void ctl_disable_track_pid(track_pid_t* tp)
{
	tp->div.flag_bypass = 1;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_track_pid_output(track_pid_t* tp)
{
	return tp->out;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_TRACK_PID_H_

