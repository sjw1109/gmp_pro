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

#include <ctl/component/intrinsic/continuous/continuous_pid.h>
#include <ctl/component/intrinsic/discrete/divider.h>
#include <ctl/component/intrinsic/discrete/slope_lim.h>

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
    } track_pid_t;

    
void ctl_init_track_pid(
        // handle of track pid
        track_pid_t *tp,
        // pid parameters
        ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
        // saturation limit
        ctrl_gt sat_max, ctrl_gt sat_min,
        // slope limit
        ctrl_gt slope_max, ctrl_gt slope_min,
        // division factor
        uint32_t division,
        // controller frequency
        parameter_gt fs);

    GMP_STATIC_INLINE
    void ctl_clear_track_pid(track_pid_t *tp)
    {
        ctl_clear_pid(&tp->pid);
        ctl_clear_divider(&tp->div);
        ctl_clear_limit_slope(&tp->traj);
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_step_track_pid(track_pid_t *tp, ctrl_gt target, ctrl_gt now)
    {
        if (ctl_step_divider(&tp->div))
        {
            ctl_step_slope_limit(&tp->traj, target);

            return ctl_step_pid_ser(&tp->pid, tp->traj.out - now);
        }

        return ctl_get_pid_output(&tp->pid);
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_track_pid_output(track_pid_t *tp)
    {
        return tp->pid.out;
    }

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_TRACK_PID_H_
