// Single Phase Modulation

#ifndef _FILE_SINGLE_PHASE_MODULATION_H_
#define _FILE_SINGLE_PHASE_MODULATION_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Single phase modulation module

// Standard H bridge topology

// input [-1, 1] modulation depth -> compare value
typedef struct _tag_single_phase_H_modulation
{
    //
    // input
    //

    //
    // output
    //
    pwm_gt phase_L;
    pwm_gt phase_N;

    //
    // parameters
    //

    // PWM
    pwm_gt pwm_depth;

    // PWM dead band, per unit value
    ctrl_gt pwm_deadband;

} single_phase_H_modulation_t;

GMP_STATIC_INLINE
void ctl_clear_single_phase_H_modulation(
    // handle of modulation object
    single_phase_H_modulation_t *bridge)
{
    bridge->phase_L = 0;
    bridge->phase_N = 0;
}

void ctl_init_single_phase_H_modulation(
    // handle of modulation object
    single_phase_H_modulation_t *bridge,
    // PWM depth
    pwm_gt pwm_depth,
    // PWM dead band, per unit value, generally greater than 0.02
    ctrl_gt pwm_deadband)
{
    bridge->pwm_depth = pwm_depth;
    bridge->pwm_deadband = pwm_deadband;

    ctl_clear_single_phase_H_modulation(bridge);
}

GMP_STATIC_INLINE
void ctl_step_single_phase_H_modulation(single_phase_H_modulation_t *bridge, ctrl_gt u_target)
{
    // modulation event isn't in PWM dead band
    if (u_target >= bridge->pwm_deadband || u_target <= -bridge->pwm_deadband)
    {
        bridge->phase_L = pwm_mul(bridge->pwm_depth, ctl_div2(1.0f + u_target));
        bridge->phase_N = pwm_mul(bridge->pwm_depth, ctl_div2(1.0f - u_target));
    }
    // modulation in dead band do nothing
    else
    {
    }
}

GMP_STATIC_INLINE
pwm_gt ctl_get_single_phase_modulation_L_phase(single_phase_H_modulation_t *bridge)
{
    return bridge->phase_L;
}

GMP_STATIC_INLINE
pwm_gt ctl_get_single_phase_modulation_N_phase(single_phase_H_modulation_t *bridge)
{
    return bridge->phase_N;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_SINGLE_PHASE_MODULATION_H_
