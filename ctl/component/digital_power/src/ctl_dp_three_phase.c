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
// Single Phase PLL 
#include <ctl/component/digital_power/three_phase/pll.h>

void ctl_init_pll(
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
    ctl_init_pid(&pll->pid_pll, pid_kp, pid_Ti, pid_Td);

    // init parameters
    pll->freq_sf = float2ctrl(f_base / f_ctrl);

    // clear controller
    ctl_clear_pll(&pll);
}
