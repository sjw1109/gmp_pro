/**
 * @file ctl_dp_three_phase.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-07-19
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// three Phase PLL
#include <ctl/component/digital_power/three_phase/pll.h>

void ctl_init_pll_3ph(
    // controller object
    three_phase_pll_t *pll,
    // per unit frequency base, Hz
    parameter_gt f_base,
    // PLL PI controller parameters
    ctrl_gt pid_kp, ctrl_gt pid_Ti, ctrl_gt pid_Td,
    // controller frequency
    parameter_gt f_ctrl)
{
    // init pll pid controller
    ctl_init_pid(&pll->pid_pll, pid_kp, pid_Ti, pid_Td, f_ctrl);

    // init parameters
    pll->freq_sf = float2ctrl(f_base / f_ctrl);

    // clear controller
    ctl_clear_pll_3ph(pll);
}

//////////////////////////////////////////////////////////////////////////
// three phase modulation
#include <ctl/component/digital_power/three_phase/tp_modulation.h>

void ctl_init_three_phase_bridge_modulation(
    // three phase bridge handle
    three_phase_bridge_modulation_t *bridge,
    // full scale pwm compare
    pwm_gt pwm_full_scale,
    // deadband of PWM compare
    pwm_gt pwm_deadband,
    // current deadband
    ctrl_gt current_deadband)
{
    bridge->pwm_full_scale = pwm_full_scale;
    bridge->pwm_deadband = pwm_deadband;
    bridge->current_deadband = current_deadband;

    ctl_clear_three_phase_bridge_modulation(bridge);
}
