// MTPA controller
// For Salient Pole PMSM, Ld != Lq
// in order to gain maximum torque is is total current command should be ssaperate into two parts

// clang-format off
// 

//tex:
// $$
//  \begin{aligned}
//  i_{d} &= \frac{\sqrt{\psi_{f}^{2}+8\left(L_{d}-L_{q}\right)^{2}i_{s}^{2}}-\psi_{f}}{4\left(L_{d}-L_{q}\right)} \\
//        &= \sqrt{\left(\frac{\psi_f}{4(L_d-L_q)}\right)^2 + \frac{i_s^2}{2}} - \frac{\psi_f}{4(L_d-L_q)}
//  \end{aligned}       
// $$

//
// clang-format on

#ifndef _FILE_PMSM_MTPA_H_
#define _FILE_PMSM_MTPA_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_mpta_current_distributor
{
    // input parameters

    // output id, iq
    ctrl_gt id;
    ctrl_gt iq;

    // Parameters of the analytical formula
    // \frac{\psi_f}{4(L_d-L_q)}
    ctrl_gt coef_psif_over_ldq;

    // (\frac{\psi_f}{4(L_d-L_q)})^2
    ctrl_gt coef_psif_over_ldq_sqr;

    // sign of (Ld-Lq)
    fast_gt sign_ld_lq;

} mtpa_current_distributor_t;

void ctl_init_mtpa_current_distributor(
    // MTPA controller object
    mtpa_current_distributor_t *mtpa,
    // motor parameter
    parameter_gt Ld, parameter_gt Lq, parameter_gt psi_f,
    // Motor Base value
    parameter_gt I_base)
{
    parameter_gt psif_over_ldq = psi_f / 4 / Ld - Lq / I_base;

    if (psif_over_ldq >= 0)
    {
        mtpa->sign_ld_lq = 1;
    }
    else // negative
    {
        mtpa->sign_ld_lq = 0;
        psif_over_ldq = -psif_over_ldq;
    }

    mtpa->coef_psif_over_ldq = float2ctrl(psif_over_ldq);
    mtpa->coef_psif_over_ldq_sqr = float2ctrl(psif_over_ldq * psif_over_ldq);
}

GMP_STATIC_INLINE
ctrl_gt ctl_step_mtpa_current_distributor(
    // MTPA controller object
    mtpa_current_distributor_t *mtpa,
    // input total current
    ctrl_gt is)
{
    ctrl_gt total_current = ctl_sqrt(mtpa->coef_psif_over_ldq_sqr + ctl_div2(ctl_mul(is, is)));
    ctrl_gt id_current = total_current - coef_psif_over_ldq;

    ctrl_gt iq_current = sqrt(ctl_mul(is, is) - ctl_mul(id_current, id_current));

    if (sign_ld_lq)
        mtpa->id = id_current;
    else
        mtpa->id = -id_current;

    mtpa->iq = iq_current;

    return mtpa->id;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PMSM_MTPA_H_
