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
        ctrl_gt raw;

        // param full scale
        pwm_gt full_scale;

        // param phase shift
        pwm_gt phase;

        // OUTPUT the PWM output channel
        pwm_gt value;

    } pwm_channel_t;

    // init object function
    ec_gt ctl_init_pwm_channel(pwm_channel_t *pwm_obj);

    // setup the pwm object
    ec_gt ctl_setup_pwm_channel(pwm_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    // calculate function
    pwm_gt ctl_calc_pwm_channel(pwm_channel_t *pwm_obj, ctrl_gt raw);
    pwm_gt ctl_calc_pwm_channel_warp(pwm_channel_t *pwm_obj, ctrl_gt raw);
    pwm_gt ctl_calc_pwm_channel_inv(pwm_channel_t *pwm_obj, ctrl_gt raw);

    //////////////////////////////////////////////////////////////////////////
    // Dual channel PWM channel

    typedef struct _tag_pwm_dual_channel
    {
        // input raw data from control law
        ctrl_gt raw[2];

        // param full scale
        pwm_gt full_scale;

        // param phase shift
        pwm_gt phase;

        // OUTPUT the PWM output channel
        pwm_gt value[2];

    } pwm_dual_channel_t;

    // init funciton for dual channel PWM channel
    ec_gt ctl_init_pwm_dual_channel(pwm_dual_channel_t *pwm_obj);

    // setup pwm object
    ec_gt ctl_setup_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    // calculate function 
    void ctl_calc_pwm_dual_channel(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw);
    void ctl_calc_pwm_dual_channel_warp(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw);
    void ctl_calc_pwm_dual_channel_inv(pwm_dual_channel_t *pwm_obj, ctl_vector2_t *raw);

    //////////////////////////////////////////////////////////////////////////
    // Tri channel PWM channel

    typedef struct _tag_pwm_tri_channel
    {
        // input raw data from control law
        ctrl_gt raw[3];

        // param full scale
        pwm_gt full_scale;

        // param phase shift
        pwm_gt phase;

        // OUTPUT the PWM output channel
        pwm_gt value[3];

    } pwm_tri_channel_t;

    // init funciton for dual channel PWM channel
    ec_gt ctl_init_pwm_tri_channel(pwm_tri_channel_t *pwm_obj);

    // setup pwm object
    ec_gt ctl_setup_pwm_tri_channel(pwm_tri_channel_t *pwm_obj, pwm_gt phase, pwm_gt full_scale);

    // calculate function
    void ctl_calc_pwm_tri_channel(pwm_tri_channel_t *pwm_obj, ctl_vector3_t *raw);
    void ctl_calc_pwm_tri_channel_warp(pwm_tri_channel_t *pwm_obj, ctl_vector3_t *raw);
    void ctl_calc_pwm_tri_channel_inv(pwm_tri_channel_t *pwm_obj, ctl_vector3_t *raw);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PWM_CHANNEL_H_
