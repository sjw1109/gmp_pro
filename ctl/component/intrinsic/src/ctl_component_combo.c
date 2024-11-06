
#include <gmp_core.h>

#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// Track_PID.h
//

#include <ctl/component/intrinsic/combo/track_pid.h>

ec_gt ctl_init_track_pid(ctl_track_pid_t *tp)
{
    ctl_init_divider(&tp->div);
    ctl_init_slope_limit(&tp->traj);
    ctl_init_pid(&tp->pid);

    return GMP_EC_OK;
}

ec_gt ctl_setup_track_pid(ctl_track_pid_t *tp,
                          // pid parameters
                          ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
                          // saturation limit
                          ctrl_gt sat_min, ctrl_gt sat_max,
                          // slope limit
                          ctrl_gt slope_min, ctrl_gt slope_max,
                          // division factor
                          uint32_t division)
{
    ctl_setup_slope_limit(&tp->traj, slope_min, slope_max);
    ctl_setup_divider(&tp->div, division);
    ctl_setup_pid(&tp->pid, kp, ki, kd, sat_min, sat_max);

    return GMP_EC_OK;
}
