
// necessary headers
#include <ctl/component/intrinsic/discrete/discrete_pid.h>
#include <ctl/component/intrinsic/discrete/divider.h>
#include <ctl/component/intrinsic/discrete/slope_lim.h>

#ifndef _FILE_TRACK_DISCRETE_PID_H_
#define _FILE_TRACK_DISCRETE_PID_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct _tag_track_discrete_pid_t
    {
        discrete_pid_t pid;

        ctl_divider_t div;
        ctl_slope_lim_t traj;

    } track_discrete_pid_t;

    void ctl_init_discrete_track_pid(
        // pointer to track pid object
        track_discrete_pid_t *tp,
        // pid parameters, unit sec
        parameter_gt kp, parameter_gt Ti, parameter_gt Td,
        // saturation limit
        ctrl_gt sat_max, ctrl_gt sat_min,
        // slope limit, unit: p.u./sec
        parameter_gt slope_max, parameter_gt slope_min,
        // division factor:
        uint32_t division,
        // controller frequency, unit Hz
        parameter_gt fs);

    // reset discrete track pid object
    GMP_STATIC_INLINE
    void ctl_clear_discrete_track_pid(track_discrete_pid_t *tp)
    {
        ctl_clear_discrete_pid(&tp->pid);
        ctl_clear_divider(&tp->div);
        ctl_clear_limit_slope(&tp->traj);
    }

    // step discrete track pid object
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_discrete_track_pid(track_discrete_pid_t *tp, ctrl_gt target, ctrl_gt fbk)
    {
        if (ctl_step_divider(&tp->div))
        {
            ctl_step_slope_limit(&tp->traj, target);
            return ctl_step_discrete_pid(&tp->pid, tp->traj.out - fbk);
        }

        return ctl_get_discrete_pid_output(&tp->pid);
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_TRACK_DISCRETE_PID_H_
