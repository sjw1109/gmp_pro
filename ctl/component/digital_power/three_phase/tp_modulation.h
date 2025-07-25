
#ifndef _FILE_THREE_PHASE_MODULATION_H_
#define _FILE_THREE_PHASE_MODULATION_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_three_phase_bridge_modulation
{
    //
    // output
    //

    // three phase PWM compare result
    pwm_gt phase_A;
    pwm_gt phase_B;
    pwm_gt phase_C;

    //
    // parameters
    //

    // full scale PWM compare
    pwm_gt pwm_full_scale;
    // deadband of PWM
    pwm_gt pwm_deadband;
    // current hysteresis comparator
    ctrl_gt current_deadband;

    //
    // internal state
    //

    // current direction
    // 1 is positive, -1 is negative, 0 is reset state
    fast_gt current_dir_A;
    fast_gt current_dir_B;
    fast_gt current_dir_C;
} three_phase_bridge_modulation_t;

GMP_STATIC_INLINE
void ctl_clear_three_phase_bridge_modulation(three_phase_bridge_modulation_t *bridge)
{
    bridge->phase_A = 0;
    bridge->phase_B = 0;
    bridge->phase_C = 0;

    bridge->current_dir_A = 0;
    bridge->current_dir_B = 0;
    bridge->current_dir_C = 0;
}

void ctl_init_three_phase_bridge_modulation(
    // three phase bridge handle
    three_phase_bridge_modulation_t *bridge,
    // full scale pwm compare
    pwm_gt pwm_full_scale,
    // deadband of PWM compare
    pwm_gt pwm_deadband,
    // current deadband
    ctrl_gt current_deadband);

GMP_STATIC_INLINE 
void ctl_step_three_phase_bridge_modulation(
    // handle of three phase bridge
    three_phase_bridge_modulation_t *bridge,
    // voltage
    ctrl_gt uA, ctrl_gt uB, ctrl_gt uC,
    // current
    ctrl_gt iA, ctrl_gt iB, ctrl_gt iC)
{

    pwm_gt comp_val_A = 0, comp_val_B = 0, comp_val_C = 0;
    pwm_gt deadband_half = bridge->pwm_deadband / 2;

    // --- Phase A Processing ---
    if (iA > bridge->current_deadband)
        bridge->current_dir_A = 1;
    else if (iA < -bridge->current_deadband)
        bridge->current_dir_A = -1;

    if (bridge->current_dir_A == 1)
        comp_val_A = deadband_half;
    else if (bridge->current_dir_A == -1)
        comp_val_A = -deadband_half;

    ctrl_gt duty_A = ctl_div2(1.0f + uA);
    pwm_gt phase_A_raw = pwm_mul(duty_A, bridge->pwm_full_scale);
    bridge->phase_A = pwm_sat(phase_A_raw + comp_val_A, bridge->pwm_full_scale, 0);

    // --- Phase B Processing ---
    if (iB > bridge->current_deadband)
        bridge->current_dir_B = 1;
    else if (iB < -bridge->current_deadband)
        bridge->current_dir_B = -1;

    if (bridge->current_dir_B == 1)
        comp_val_B = deadband_half;
    else if (bridge->current_dir_B == -1)
        comp_val_B = -deadband_half;

    ctrl_gt duty_B = ctl_div2(1.0f + uB);
    pwm_gt phase_B_raw = pwm_mul(duty_B, bridge->pwm_full_scale);
    bridge->phase_B = pwm_sat(phase_B_raw + comp_val_B, bridge->pwm_full_scale, 0);

    // --- Phase C Processing ---
    if (iC > bridge->current_deadband)
        bridge->current_dir_C = 1;
    else if (iC < -bridge->current_deadband)
        bridge->current_dir_C = -1;

    if (bridge->current_dir_C == 1)
        comp_val_C = deadband_half;
    else if (bridge->current_dir_C == -1)
        comp_val_C = -deadband_half;

    ctrl_gt duty_C = ctl_div2(1.0f + uC);
    pwm_gt phase_C_raw = pwm_mul(duty_C, bridge->pwm_full_scale);
    bridge->phase_C = pwm_sat(phase_C_raw + comp_val_C, bridge->pwm_full_scale, 0);
}

// get modulation result
GMP_STATIC_INLINE 
pwm_gt ctl_get_three_phase_modulation_result(
    // bridge handle
    three_phase_bridge_modulation_t *bridge,
    // index of phases, 0-2:ABC
    fast_gt index)
{
    if (index == phase_A)
        return bridge->phase_A;
    else if (index == phase_B)
        return bridge->phase_B;
    else if (index == phase_C)
        return bridge->phase_C;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_THREE_PHASE_MODULATION_H_
