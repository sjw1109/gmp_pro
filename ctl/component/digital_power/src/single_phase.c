/**
 * @file ctl_common_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-06-21
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// Single Phase PLL 
#include <ctl/component/digital_power/single_phase/spll.h>

void ctl_init_single_phase_pll(
    // handle of Single phase PLL object
    ctl_single_phase_pll *spll,
    // gain of SPLL module
    parameter_gt gain,
    // filter cut frequency
    parameter_gt fc,
    // Target frequency, grid frequency, Hz
    parameter_gt fg,
    // isr frequency, Hz
    parameter_gt fs)
{
    // clear single phase PLL objects
    ctl_clear_single_phase_pll(spll);

    // init a discrete SOGI object
    ctl_init_discrete_sogi(&spll->sogi, 0.5, fg, fs);

    // init filter object
    ctl_init_lp_filter(&spll->filter_uq, fc);

    // frequency parameters
    spll->frequency_sf = float2ctrl(fg / fs);

    // gain of SPLL
    spll->pll_gain = float2ctrl(gain);
}
