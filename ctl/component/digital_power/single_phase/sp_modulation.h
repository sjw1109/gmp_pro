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
    pwm_gt pwm_full_scale;

    // PWM dead band, real value
    // this value means half of real dead band
    pwm_gt pwm_deadband;

    // Current direction Deadband
    ctrl_gt current_deadband;

    // current direction
    fast_gt current_dir;

} single_phase_H_modulation_t;

GMP_STATIC_INLINE
void ctl_clear_single_phase_H_modulation(
    // handle of modulation object
    single_phase_H_modulation_t *bridge)
{
    bridge->phase_L = 0;
    bridge->phase_N = 0;
    bridge->current_dir = 0;
}

void ctl_init_single_phase_H_modulation(
    // handle of modulation object
    single_phase_H_modulation_t *bridge,
    // PWM depth
    pwm_gt pwm_full_scale,
    // PWM dead band real value
    pwm_gt pwm_deadband,
    // current dead band
    ctrl_gt current_deadband);

GMP_STATIC_INLINE
void ctl_step_single_phase_H_modulation(single_phase_H_modulation_t *bridge, ctrl_gt u_target, ctrl_gt inverter_current)
{
    // hysteresis comparator
    if (inverter_current > bridge->current_deadband)
    {
        bridge->current_dir = 1;
    }
    else if (inverter_current < -bridge->current_deadband)
    {
        bridge->current_dir = -1;
    }

    ctrl_gt modulate_target_L = ctl_sat(ctl_div2(-u_target + float2ctrl(1)), 0, float2ctrl(1));
    ctrl_gt modulate_target_N = ctl_sat(ctl_div2(u_target + float2ctrl(1)), 0, float2ctrl(1));

    if (bridge->current_dir == 1)
    {
        bridge->phase_L = pwm_mul(modulate_target_L, bridge->pwm_full_scale) - bridge->pwm_deadband;
        bridge->phase_N = pwm_mul(modulate_target_N, bridge->pwm_full_scale) + bridge->pwm_deadband;
    }
    else if (bridge->current_dir == -1)
    {
        bridge->phase_L = pwm_mul(modulate_target_L, bridge->pwm_full_scale) + bridge->pwm_deadband;
        bridge->phase_N = pwm_mul(modulate_target_N, bridge->pwm_full_scale) - bridge->pwm_deadband;
    }
    else
    {
        bridge->phase_L = pwm_mul(modulate_target_L, bridge->pwm_full_scale);
        bridge->phase_N = pwm_mul(modulate_target_N, bridge->pwm_full_scale);
    }

    bridge->phase_L = pwm_sat(bridge->phase_L, bridge->pwm_full_scale, 0);
    bridge->phase_N = pwm_sat(bridge->phase_N, bridge->pwm_full_scale, 0);
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
