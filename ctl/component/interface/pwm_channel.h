/**
 * @file pwm_channel.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/component/interface/interface_base.h>

#ifndef _FILE_PWM_CHANNEL_H_
#define _FILE_PWM_CHANNEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // Single channel PWM channel
    typedef struct _tag_pwm_channel
    {
        // INPUT raw data from control law
        pwm_ift raw;

        // param full scale
        pwm_gt full_scale;

        // param phase shift
        pwm_gt phase;

        // OUTPUT the PWM output channel
        pwm_gt value;

    } pwm_channel_t;

    //// init object function
    // ec_gt ctl_init_pwm_channel(pwm_channel_t *pwm_obj);

    //// setup the pwm object
    // ec_gt ctl_setup_pwm_channel(pwm_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    void ctl_init_pwm_channel(pwm_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    // calculate function
    pwm_gt ctl_calc_pwm_channel(pwm_channel_t *pwm_obj, ctrl_gt raw)
    {
        pwm_obj->raw.value = raw;

        pwm_obj->value = pwm_mul(pwm_obj->raw.value, pwm_obj->full_scale) + pwm_obj->phase;
        pwm_obj->value = pwm_sat(pwm_obj->value, pwm_obj->full_scale, 0);

        return pwm_obj->value;
    }
    pwm_gt ctl_calc_pwm_channel_warp(pwm_channel_t *pwm_obj, ctrl_gt raw)
    {
        pwm_obj->raw.value = raw;

        pwm_obj->value = pwm_mul(pwm_obj->raw.value, pwm_obj->full_scale) + pwm_obj->phase;
        pwm_obj->value = pwm_obj->value % pwm_obj->full_scale;

        return pwm_obj->value;
    }
    pwm_gt ctl_calc_pwm_channel_inv(pwm_channel_t *pwm_obj, ctrl_gt raw)
    {
        pwm_obj->raw.value = raw;

        pwm_obj->value = pwm_obj->full_scale - pwm_mul(pwm_obj->raw.value, pwm_obj->full_scale);
        pwm_obj->value = pwm_sat(pwm_obj->value, pwm_obj->full_scale, 0);

        return pwm_obj->value;
    }

    // get control port handle
    GMP_STATIC_INLINE
    pwm_ift *ctl_get_pwm_channel_ctrl_port(pwm_channel_t *pwm)
    {
        return &pwm->raw;
    }

    //////////////////////////////////////////////////////////////////////////
    // Dual channel PWM channel

    typedef struct _tag_pwm_dual_channel
    {
        // input raw data from control law
        dual_pwm_ift raw;

        // param full scale
        pwm_gt full_scale;

        // param phase shift
        pwm_gt phase;

        // OUTPUT the PWM output channel
        pwm_gt value[2];

    } pwm_dual_channel_t;

    //// init funciton for dual channel PWM channel
    // ec_gt ctl_init_pwm_dual_channel(pwm_dual_channel_t *pwm_obj);

    //// setup pwm object
    // ec_gt ctl_setup_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    void ctl_init_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    // calculate function
    GMP_STATIC_INLINE
    void ctl_calc_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw)
    {
        int i;

        for (i = 0; i < 2; ++i)
        {
            pwm_obj->raw.value.dat[i] = raw->dat[i];

            pwm_obj->value[i] = pwm_mul(pwm_obj->raw.value.dat[i], pwm_obj->full_scale) + pwm_obj->phase;
            pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
        }
    }

    GMP_STATIC_INLINE
    void ctl_calc_pwm_dual_channel_warp(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw)
    {
        int i;

        for (i = 0; i < 2; ++i)
        {
            pwm_obj->raw.value.dat[i] = raw->dat[i];

            pwm_obj->value[i] = pwm_mul(pwm_obj->raw.value.dat[i], pwm_obj->full_scale) + pwm_obj->phase;
            pwm_obj->value[i] = pwm_obj->value[i] % pwm_obj->full_scale;
        }
    }

    GMP_STATIC_INLINE
    void ctl_calc_pwm_dual_channel_inv(pwm_dual_channel_t* pwm_obj, ctl_vector2_t* raw)
    {
        int i;

        for (i = 0; i < 2; ++i)
        {
            pwm_obj->raw.value.dat[i] = raw->dat[i];

            pwm_obj->value[i] = pwm_obj->full_scale - pwm_mul(pwm_obj->raw.value.dat[i], pwm_obj->full_scale);
            pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
        }
    }

    GMP_STATIC_INLINE
    dual_pwm_ift *ctl_get_dual_pwm_channel_ctrl_port(pwm_dual_channel_t *pwm)
    {
        return &pwm->raw;
    }

    //////////////////////////////////////////////////////////////////////////
    // Tri channel PWM channel

    typedef struct _tag_pwm_tri_channel
    {
        // input raw data from control law
        tri_pwm_ift raw;

        // param full scale
        pwm_gt full_scale;

        // param phase shift
        pwm_gt phase;

        // OUTPUT the PWM output channel
        pwm_gt value[3];

    } pwm_tri_channel_t;

    // init function for dual channel PWM channel
    //ec_gt ctl_init_pwm_tri_channel(pwm_tri_channel_t *pwm_obj);

    //// setup pwm object
    //ec_gt ctl_setup_pwm_tri_channel(pwm_tri_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    void ctl_init_pwm_tri_channel(pwm_tri_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    // calculate function
    void ctl_calc_pwm_tri_channel(pwm_tri_channel_t* pwm_obj, ctl_vector3_t* raw)
    {
        fast_gt i;

        for (i = 0; i < 3; ++i)
        {
            pwm_obj->raw.value.dat[i] = raw->dat[i];

            pwm_obj->value[i] = pwm_mul(pwm_obj->raw.value.dat[i], pwm_obj->full_scale) + pwm_obj->phase;
            pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
        }
    }
    void ctl_calc_pwm_tri_channel_warp(pwm_tri_channel_t* pwm_obj, ctl_vector3_t* raw)
    {
        fast_gt i;

        for (i = 0; i < 3; ++i)
        {
            pwm_obj->raw.value.dat[i] = raw->dat[i];

            pwm_obj->value[i] = pwm_mul(pwm_obj->raw.value.dat[i], pwm_obj->full_scale) + pwm_obj->phase;
            pwm_obj->value[i] = pwm_obj->value[i] % pwm_obj->full_scale;
        }
    }
    void ctl_calc_pwm_tri_channel_inv(pwm_tri_channel_t* pwm_obj, ctl_vector3_t* raw)
    {
        fast_gt i;

        for (i = 0; i < 3; ++i)
        {
            pwm_obj->raw.value.dat[i] = raw->dat[i];

            pwm_obj->value[i] = pwm_obj->full_scale - pwm_mul(pwm_obj->raw.value.dat[i], pwm_obj->full_scale);
            pwm_obj->value[i] = pwm_sat(pwm_obj->value[i], pwm_obj->full_scale, 0);
        }
    }

    //// stop pwm output
    // void ctl_stop_pwm_tri_channel(pwm_tri_channel_t *pwm_obj);
    // void ctl_start_pwm_tri_channel(pwm_tri_channel_t *pwm_obj);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PWM_CHANNEL_H_
