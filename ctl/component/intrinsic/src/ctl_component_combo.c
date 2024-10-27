


//////////////////////////////////////////////////////////////////////////
// Track_PID.h
//

#include <ctl/component/common/track_pid.h>

void ctl_init_track_pid(track_pid_t *tp)
{
    ctl_init_divider(&tp->div);
    ctl_init_slope_limit(&tp->traj);
    ctl_init_pid(&tp->pid);
    tp->out = 0;
}

void ctl_setup_track_pid(track_pid_t *tp, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // pid parameters
                         ctrl_gt sat_min, ctrl_gt sat_max,                    // saturation limit
                         ctrl_gt slope_min, ctrl_gt slope_max,                // slope limit
                         uint32_t division                                    // division factor
)
{
    ctl_setup_slope_limit(&tp->traj, slope_min, slope_max, sat_min, sat_max);
    ctl_setup_divider(&tp->div, division);
    ctl_setup_pid(&tp->pid, kp, ki, kd, sat_min, sat_max);
}
