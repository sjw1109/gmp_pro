
#include <gmp_core.h>

#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// pmsm smo

#include <ctl/component/motor_control/observer/pmsm.smo.h>

void ctl_init_pmsm_smo(ctl_pmsm_smo_observer_t *smo)
{
    smo->e_alpha_est = 0;
    smo->e_beta_est = 0;

    smo->z_alpha = 0;
    smo->z_beta = 0;

    smo->i_alpha = 0;
    smo->i_beta = 0;
    smo->u_alpha = 0;
    smo->u_beta = 0;

    smo->i_alpha_est = 0;
    smo->i_beta_est = 0;

    smo->k1 = GMP_CONST_1;
    smo->k2 = float2ctrl(0.01);
    smo->k3 = 0;

    smo->k_filter_e = GMP_CONST_1_OVER_2;
    smo->k_filter_omega = GMP_CONST_1_OVER_2;

    smo->k_slide = float2ctrl(10.0);

    smo->theta_est = 0;
    ctl_set_phasor_via_angle(smo->theta_est, &smo->phasor);

    ctl_init_pid(&smo->pid_pll);

    smo->spd_sf = GMP_CONST_1;
    smo->spd_est_pu = 0;
    smo->wr = 0;
}

void ctl_setup_pmsm_smo(ctl_pmsm_smo_observer_t *smo, parameter_gt Rs, parameter_gt Ld, parameter_gt Lq,
                        parameter_gt f_ctrl, parameter_gt fc_e, parameter_gt fc_omega, ctrl_gt pid_kp, ctrl_gt pid_ki,
                        ctrl_gt pid_kd, ctrl_gt spd_max_limit, ctrl_gt spd_min_limit, // unit p.u.
                        ctrl_gt k_slide, parameter_gt speed_base_rpm, uint16_t pole_pairs)
{
    smo->k1 = float2ctrl(1 / (Ld * f_ctrl));
    smo->k2 = float2ctrl(Rs / (Ld * f_ctrl));
    smo->k3 = float2ctrl((Ld - Lq) / (Ld * f_ctrl));

    smo->k_slide = k_slide;

    smo->k_filter_e = ctl_helper_lp_filter(f_ctrl, fc_e);
    smo->k_filter_omega = ctl_helper_lp_filter(f_ctrl, fc_omega);

    ctl_setup_pid(&smo->pid_pll, pid_kp, pid_ki, pid_kd, spd_min_limit, spd_max_limit);

    smo->spd_sf = float2ctrl((60 / 2 / PI) * f_ctrl / speed_base_rpm / pole_pairs);
}

void ctl_setup_pmsm_smo_via_consultant(ctl_pmsm_smo_observer_t *smo,
                                       // use it to calculate controller parameters
                                       ctl_pmsm_dsn_consultant_t *dsn,
                                       // use it to calculate controller parameters
                                       ctl_motor_driver_consultant_t *drv,
                                       // use it to per unit controller
                                       ctl_pmsm_nameplate_consultant_t *np, ctrl_gt pid_kp, ctrl_gt pid_ki,
                                       ctrl_gt pid_kd, ctrl_gt k_slide)
{
    ctl_setup_pmsm_smo(smo, dsn->Rs, dsn->Ld, dsn->Lq, drv->control_law_freq,
                       drv->speed_closeloop_bw * (parameter_gt)6.28, drv->speed_closeloop_bw * (parameter_gt)6.28,
                       pid_kp, pid_ki, pid_kd, float2ctrl(1.2),
                       -float2ctrl(1.2), // unit p.u.
                       k_slide, np->rated_speed_rpm, dsn->pole_pair);
}

