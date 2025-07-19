// This module is for three phase PLL

#ifndef _FILE_THREE_PHASE_PLL_H_
#define _FILE_THREE_PHASE_PLL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <ctl/component/intrinsic/continuous/continuous_pid.h>

typedef struct _tag_pll
{
    //
    // input variables
    //

    // alpah axis variables
    ctrl_gt e_alpha;

    // beta axis varibles
    ctrl_gt e_beta;

    //
    // output variables
    //

    // theta
    ctrl_gt theta;

    // phasor for theta
    ctl_vector2_t phasor;

    // frequency
    ctrl_gt freq_pu;

    //
    // intermediate variables
    //

    // PLL Error Items
    ctrl_gt e_error;

    // speed scale factor
    // scale factor: p.u./tick -> p.u.
    ctrl_gt freq_sf;

    //
    // internal controller objects
    //

    // PLL pid controller
    // output is speed delta p.u.
    pid_regular_t pid_pll;

} three_phase_pll_t;

void ctl_init_pll(
    // controller object
    three_phase_pll_t *pll,
    // per unit frequency base, Hz
    parameter_gt f_base,
    // PLL PI controller parameters
    ctrl_gt pid_kp, ctrl_gt pid_Ti, ctrl_gt pid_Td,
    // controller frequency
    parameter_gt f_ctrl);

GMP_STATIC_INLINE
void ctl_clear_pll(
    // controller object
    three_phase_pll_t *pll)
{
    pll->e_alpha = 0;
    pll->e_beta = 0;

    pll->e_error = 0;

    pll->theta = 0;
    pll->freq_pu = float2ctrl(1.0);

    ctl_set_phasor_via_angle(pll->theta, &pll->phasor);

    ctl_clear_pid(&smo->pid_pll);
}

GMP_STATIC_INLINE
ctrl_gt ctl_step_pll(
    // controller object
    three_phase_pll_t *pll,
    // input alpha & beta axis
    ctrl_gt alpha, ctrl_gt beta)
{
    // store input variables
    pll->e_alpha = alpha;
    pll->e_beta = beta;

    // PLL get angle out
    // 0. generate phasor
    ctl_set_phasor_via_angle(pll->theta, &pll->phasor);

    // 1. error signal generate
    pll->e_error = -ctl_mul(pll->e_alpha, pll->phasor.dat[1]) - ctl_mul(pll->e_beta, pll->phasor.dat[0]);

    // 2. PLL speed lock
    ctl_step_pid_par(&pll->pid_pll, pll->e_error);

    // 3. update speed
    pll->freq_pu = 1 + pll->pid_pll.out;

    // 4. update theta
    pll->theta += ctl_mul(pll->freq_pu, pll->freq_sf);
    pll->theta = ctrl_mod_1(pll->theta);

    return pll->theta;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_THREE_PHASE_PLL_H_
