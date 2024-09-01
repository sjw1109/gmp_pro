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

	divider_t div;
	slope_lim_t traj;

	ctrl_gt out;
}track_pid_t;

void ctl_init_track_pid(track_pid_t* tp);

void ctl_setup_track_pid(track_pid_t* tp,
	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // pid parameters
	ctrl_gt sat_min, ctrl_gt sat_min, // saturation limit
	ctrl_gt slope_min, ctrl_gt slope_max, // slope limit
	uint32_t division //division factor
);

GMP_STATIC_INLINE
void track_pid_step(track_pid_t* tp,
	ctrl_gt target, ctrl_gt now)
{
	if (ctl_divider_run(&tp->div))
	{
		ctl_slope_limit(&tp->traj, target);

		ctl_pid_ser(&tp->pid,
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


#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_TRACK_PID_H_

