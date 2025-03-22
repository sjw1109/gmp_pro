/**
 * @file ctl_common_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-03-19
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#include <ctl/ctl_core.h>
#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// PID regular

#include <ctl/component/intrinsic/continuous/continuous_pid.h>

//ec_gt ctl_init_pid(pid_regular_t *hpid)
//{
//    hpid->out = 0;
//
//    hpid->kp = float2ctrl(1.0f);
//    hpid->ki = 0;
//    hpid->kd = 0;
//
//    hpid->out_min = float2ctrl(-1.0f);
//    hpid->out_max = float2ctrl(1.0f);
//
//    hpid->dn = 0;
//    hpid->sn = 0;
//
//    return GMP_EC_OK;
//}

 
void ctl_init_pid(pid_regular_t *hpid, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd)
{
    hpid->kp = kp;
    hpid->ki = ki;
    hpid->kd = kd;

    //hpid->out_min = out_min;
    //hpid->out_max = out_max;

    hpid->out_min = float2ctrl(-1.0f); 
    hpid->out_max = float2ctrl(1.0f);

    hpid->out = 0;
    hpid->dn = 0;
    hpid->sn = 0;
}

//// The following function has move to header file
//void ctl_set_pid_parameter(pid_regular_t *hpid, ctrl_gt kp, ctrl_gt ki, ctrl_gt kd)
//{
//    hpid->kp = kp;
//    hpid->ki = ki;
//    hpid->kd = kd;
//}
//
//void ctl_set_pid_limit(pid_regular_t *hpid, ctrl_gt out_min, ctrl_gt out_max)
//{
//    hpid->out_min = out_min;
//    hpid->out_max = out_max;
//}

//////////////////////////////////////////////////////////////////////////
// Saturation

#include <ctl/component/intrinsic/continuous/saturation.h>

//ec_gt ctl_init_saturation(saturation_t *obj)
//{
//    obj->out_min = float2ctrl(-1.0f);
//    obj->out_max = float2ctrl(1.0f);
//
//    return GMP_EC_OK;
//}

void ctl_init_saturation(ctl_saturation_t *obj, ctrl_gt out_min, ctrl_gt out_max)
{
    obj->out_min = out_min;
    obj->out_max = out_max;
}

//// The following function has move to header file
//void ctl_set_saturation(saturation_t *obj, ctrl_gt out_min, ctrl_gt out_max)
//{
//    obj->out_min = out_min;
//    obj->out_max = out_max;
//}


//////////////////////////////////////////////////////////////////////////
// Track_PID.h
//

#include <ctl/component/intrinsic/continuous/track_pid.h>

// ec_gt ctl_init_track_pid(ctl_track_pid_t *tp)
//{
//     ctl_init_divider(&tp->div);
//     ctl_init_slope_limit(&tp->traj);
//     ctl_init_pid(&tp->pid);
//
//     return GMP_EC_OK;
// }

void ctl_init_track_pid(ctl_track_pid_t *tp,
                        // pid parameters
                        ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
                        // saturation limit
                        ctrl_gt sat_min, ctrl_gt sat_max,
                        // slope limit
                        ctrl_gt slope_min, ctrl_gt slope_max,
                        // division factor
                        uint32_t division)
{
    ctl_init_slope_limit(&tp->traj, slope_min, slope_max);
    ctl_init_divider(&tp->div, division);
    ctl_init_pid(&tp->pid, kp, ki, kd);
    ctl_set_pid_limit(&tp->pid, sat_min, sat_max);
}

//// The following function has moved to header file.
// void ctl_clear_track_pid(ctl_track_pid_t *tp)
//{
//     ctl_clear_pid(&tp->pid);
//     ctl_clear_divider(&tp->div);
//     ctl_clear_limit_slope(&tp->traj);
// }
