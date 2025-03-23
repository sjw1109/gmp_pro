/**
 * @file slope_lim.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#ifndef _FILE_SLOPE_LIM_H_
#define _FILE_SLOPE_LIM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct _tag_slope_limit_t
    {
        // output
        ctrl_gt out;

        // parameters

        // slope limit
        ctrl_gt slope_min;
        ctrl_gt slope_max;

    } ctl_slope_lim_t;

    GMP_STATIC_INLINE
    ctrl_gt ctl_step_slope_limit(ctl_slope_lim_t *obj, ctrl_gt input)
    {
        // Calculate slope delta
        ctrl_gt delta = ctl_sat(input - obj->out, obj->slope_max, obj->slope_min);

        // calculate output data
        obj->out = obj->out + delta;

        // obj->out = ctrl_sat(obj->out + delta, obj->out_max, obj->out_min);

        return obj->out;
    }

    void ctl_init_slope_limit(
        // slope limit object
        ctl_slope_lim_t *obj,
        // slope range
        ctrl_gt slope_min, ctrl_gt slope_max);

    // ec_gt ctl_init_slope_limit(ctl_slope_lim_t* obj);

    // ec_gt ctl_setup_slope_limit(
    //     ctl_slope_lim_t* obj,
    //     ctrl_gt slope_min, ctrl_gt slope_max
    //);

    // void ctl_init_slope_limit(ctl_slope_lim_t *obj, ctrl_gt slope_min, ctrl_gt slope_max)
    //{

    //    obj->slope_min = slope_min;
    //    obj->slope_max = slope_max;
    //}

    GMP_STATIC_INLINE
    void ctl_set_sl_slope(ctl_slope_lim_t *obj, ctrl_gt slope_min, ctrl_gt slope_max)
    {
        obj->slope_min = slope_min;
        obj->slope_max = slope_max;
    }

    GMP_STATIC_INLINE
    void ctl_clear_limit_slope(ctl_slope_lim_t *obj)
    {
        obj->out = 0;
    }

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_SLOPE_LIM_H_
