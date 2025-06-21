// PLL for Single Phase
// Orthogonal Signal Generator Phase Lock Loop for Single Phase (SPLL)
//

#include <ctl/component/intrinsic/discrete/discrete_filter.h>
#include <ctl/component/intrinsic/discrete/discrete_sogi.h>
#include <ctl/math_block/coordinate/coord_trans.h>

#ifndef _FILE_SINGLE_PHASE_PLL_H_
#define _FILE_SINGLE_PHASE_PLL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_single_phase_pll
{
    //
    // Input Section
    //

    //
    // Output Section
    //

    // output: Output frequency, per unit
    // per unit base value is Normal Frequency Hz
    ctrl_gt frequency;

    // output: angle output, per unit
    ctrl_gt theta;

    // output: sin theta & cos theta
    ctrl_gt phasor;

    //
    // Intrinsic Variables
    //
    vector2_gt udq;

    //
    // Parameters
    //

    // frequency scale factor, frequency per unit -> delta theta per unit
    ctrl_gt frequency_sf;

    // PLL Gain
    ctrl_gt pll_gain;

    //
    // Submodules
    //

    // u -> ud, uq
    discrete_sogi_t sogi;

    // filter for ud&uq
    // ctl_low_pass_filter_t filter_ud;
    ctl_low_pass_filter_t filter_uq;

} ctl_single_phase_pll;

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
    parameter_gt fs);


GMP_STATIC_INLINE
void ctl_clear_single_phase_pll(
    // handle of Single phase PLL object
    ctl_single_phase_pll *spll)
{
    ctl_clear_lowpass_filter(&spll->filter_uq);
    ctl_clear_discrete_sogi(&spll->sogi);

    spll->theta = float2ctrl(0.0f);
    ctl_set_phasor_via_angle(spll->theta, &spll->phasor);
}

GMP_STATIC_INLINE
void ctl_step_single_phase_pll(
    // handle of Single phase PLL object
    ctl_single_phase_pll *spll,
    // input AC value
    ctrl_gt ac_input)
{
    //
    // Orthogonal Signal Generator
    //
    ctl_step_discrete_sogi(&spll->sogi, ac_input);

    //
    // Park Transform from alpha beta to d-q axis
    //
    vector2_gt uab;
    uab.dat[phase_alpha] = ctl_get_discrete_sogi_ds(&spll->sogi);
    uab.dat[phase_beta] = ctl_get_discrete_sogi_qs(&spll->sogi);

    ctl_ct_park2(&uab, &spll->phasor, &spll->udq);

    //
    // Loop Filter
    //

    // ctl_step_lowpass_filter(&spll->filter_ud, spll->udq.dat[phase_d]);
    ctl_step_lowpass_filter(&spll->filter_uq, spll->udq.dat[phase_q]);

    //
    // VCO
    //
    spll->frequency = 1 + ctl_mul(ctl_get_lowpass_filter_result(&spll->filter_uq), pll_gain);
    spll->theta = ctrl_mod_1(spll->theta + GMP_CONST_1 + ctl_mul(spll->frequency, spll->frequency_sf));

    ctl_set_phasor_via_angle(spll->theta, spll->phasor);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_SINGLE_PHASE_PLL_H_
