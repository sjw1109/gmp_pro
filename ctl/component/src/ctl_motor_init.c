
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>


//////////////////////////////////////////////////////////////////////////
// SVPWM module

#include <ctl/component/motor/svpwm.h>

void ctl_init_svpwm(svpwm_channel_t* svpwm)
{
	svpwm->Ualpha = 0;
	svpwm->Ubeta = 0;

	svpwm->pwm_period = 0;

	for (int i = 0; i < 3; ++i)
	{
		svpwm->T[i] = 0;
		svpwm->pwm_cmp[i] = 0;
	}
}

void ctl_setup_svpwm(svpwm_channel_t* svpwm, pwm_gt pwm_period)
{
	svpwm->pwm_period = pwm_period;

	// re-calculate SVPWM comparator
	ctl_svpwm_modulation(svpwm);
}

//void ctl_svpwm_set_Uab(svpwm_channel_t* svpwm, ctl_vector3_t Uab)
//{
//	svpwm->Ualpha = Uab.dat[0];
//	svpwm->Ubeta = Uab.dat[1];
//
//}

//////////////////////////////////////////////////////////////////////////
// Encoder module

#include <ctl/component/motor/encoder.h>

void ctl_init_pos_encoder(ctl_pos_encoder_t* pos_encoder)
{
	pos_encoder->position = 0;
	pos_encoder->offset = 0;
	pos_encoder->poles = 0;
}

void ctl_setup_pos_encoder(ctl_pos_encoder_t* enc,
	uint16_t poles, uint32_t position_base)
{
	enc->poles = poles;
	enc->position_base = position_base;
}

void ctl_init_spd_encoder(ctl_spd_encoder_t* spd_encoder)
{
	spd_encoder->speed_base = 3000;
	spd_encoder->speed = 0;
	spd_encoder->speed_krpm = 0;
}

void ctl_setup_spd_encoder(ctl_spd_encoder_t* enc, parameter_gt speed_base)
{
	enc->speed_base = speed_base;
}

void ctl_init_spd_calculator(ctl_spd_calculator_t* sc)
{
	sc->position = 0;
	sc->old_position = 0;
	sc->scale_factor = CTRL_T(1.0);
	sc->speed = 0;
	ctl_init_lp_filter(&sc->spd_filter);
	ctl_init_divider(&sc->div);
	ctl_enable_spd_calc(sc);
}

void ctl_setup_spd_calculator(ctl_spd_calculator_t* sc,
	parameter_gt control_law_freq,  // control law frequency, unit Hz
	uint16_t speed_calc_div,  // division of control law frequency, unit ticks
	parameter_gt rated_speed_rpm,  // Speed per unit base value, unit rpm
	uint16_t pole_pairs,		   // pole pairs, if you pass a elec angle,
	// just set this value to 1.
	parameter_gt speed_filter_fc)  // generally, speed_filter_fc approx to speed_calc freq divided by 10
{
	uint16_t maximum_div = rated_speed_rpm / 30;
	if (speed_calc_div < maximum_div)
	{
		maximum_div = speed_calc_div;
	}

	sc->scale_factor = CTRL_T(60.0 / 2 / PI * control_law_freq / maximum_div / pole_pairs / rated_speed_rpm);
	ctl_setup_lp_filter(&sc->spd_filter, control_law_freq / maximum_div, speed_filter_fc);
	ctl_setup_divider(&sc->div, maximum_div);
	ctl_enable_spd_calc(sc);
}


//////////////////////////////////////////////////////////////////////////
// PMSM consultant

#include <ctl/component/motor/pmsm_consultant.h>

void ctl_init_pmsm_nameplate_consultant(ctl_pmsm_nameplate_consultant_t* np)
{
	np->rated_speed_rpm = 3000;
	np->rated_output_power = 1000;
	np->rated_voltage = 300;
	np->rated_current = 300;
	np->power_factor = 0.9;
	np->rated_freq = 50;
	np->rated_torque = 0.3;
	np->eta = 0.9;
}

void ctl_setup_pmsm_nameplate_consultant(ctl_pmsm_nameplate_consultant_t* np,
	parameter_gt rated_output_power, parameter_gt rated_voltage, parameter_gt rated_current,
	parameter_gt rated_freq, parameter_gt rated_speed, parameter_gt rated_torque,
	parameter_gt power_factor, parameter_gt efficiency
)
{
	np->rated_output_power = rated_output_power;
	np->rated_voltage = rated_voltage;
	np->rated_current = rated_current;

	np->rated_freq = rated_freq;
	np->rated_speed_rpm = rated_speed;
	np->rated_torque = rated_torque;

	np->power_factor = power_factor;
	np->eta = efficiency;
}


void ctl_init_pmsm_dsn_consultant(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	pmsm_dsn->pole_pair = 1;

	pmsm_dsn->Rs = 0;

	pmsm_dsn->Ld = 0;

	pmsm_dsn->Lq = 0;

	pmsm_dsn->flux = 0;

	pmsm_dsn->inertia = 0;

	pmsm_dsn->damp = 0;
}

void ctl_import_pmsm_dsn_default_param(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	pmsm_dsn->pole_pair = MOTOR_DSN_POLE_PAIR;

	pmsm_dsn->Rs = MOTOR_DSN_STATOR_RES;

	pmsm_dsn->Ld = MOTOR_DSN_LD;

	pmsm_dsn->Lq = MOTOR_DSN_LQ;

	pmsm_dsn->flux = MOTOR_DSN_FLUX_WB;

	pmsm_dsn->inertia = MOTOR_DSN_INERTIA;

	pmsm_dsn->damp = MOTOR_DSN_DAMP;
}

void ctl_setup_pmsm_dsn_consultant(ctl_pmsm_dsn_consultant_t* pmsm_dsn,
	uint16_t pole_pair,
	parameter_gt Rs,
	parameter_gt Ld, parameter_gt Lq,
	parameter_gt flux,
	parameter_gt inertia, parameter_gt damp)
{
	pmsm_dsn->pole_pair = pole_pair;

	pmsm_dsn->Rs = Rs;

	pmsm_dsn->Ld = Ld;

	pmsm_dsn->Lq = Lq;

	pmsm_dsn->flux = flux;

	pmsm_dsn->inertia = inertia;

	pmsm_dsn->damp = damp;
}

void ctl_dsn_pmsm_Rs_via_RsLL(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Rll)
{
	pmsm_dsn->Rs = Rll / 2;
}

void ctl_dsn_pmsm_Ls_via_Lsll(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Lll)
{
	pmsm_dsn->Ld = Lll / 2;
	pmsm_dsn->Lq = Lll / 2;
}

void ctl_dsn_pmsm_load(ctl_pmsm_dsn_consultant_t* pmsm_dsn,
	parameter_gt inertia, parameter_gt damp)
{
	pmsm_dsn->inertia = inertia;
	pmsm_dsn->damp = damp;
}

parameter_gt ctl_consult_pmsm_flux_Wb(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	return pmsm_dsn->flux;
}

parameter_gt ctl_consult_pmsm_Ke(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	return pmsm_dsn->flux * CTL_CONST_PARAM_2_SQRT_6;
}

parameter_gt ctl_consult_pmsm_Ke_Vp_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	return pmsm_dsn->flux * CTL_CONST_PARAM_2_SQRT_6
		* CTL_CONST_PARAM_100PI_OVER_3 * CTL_CONST_PARAM_SQRT_2;
}

parameter_gt ctl_consult_pmsm_Ke_Vrms_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	return pmsm_dsn->flux * CTL_CONST_PARAM_2_SQRT_6 * CTL_CONST_PARAM_100PI_OVER_3;
}

parameter_gt ctl_consult_pmsm_Kt(ctl_pmsm_dsn_consultant_t* pmsm_dsn)
{
	return pmsm_dsn->flux * CTL_CONST_PARAM_3_OVER_2 * pmsm_dsn->pole_pair;
}

void ctl_set_pmsm_flux_Wb(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt flux)
{
	pmsm_dsn->flux = flux;
}

void ctl_dsn_pmsm_flux_via_Ke(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Ke)
{
	pmsm_dsn->flux = Ke / CTL_CONST_PARAM_2_SQRT_6;
}

void ctl_dsn_pmsm_flux_via_Ke_Vp_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Ke)
{
	pmsm_dsn->flux = Ke / CTL_CONST_PARAM_2_SQRT_6
		/ (CTL_CONST_PARAM_100PI_OVER_3 * CTL_CONST_PARAM_SQRT_2);
}

void ctl_dsn_pmsm_flux_via_Ke_Vrms_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Ke)
{
	pmsm_dsn->flux = Ke / CTL_CONST_PARAM_2_SQRT_6
		/ CTL_CONST_PARAM_100PI_OVER_3;
}

void ctl_dsn_pmsm_pmsm_flux_via_Kt(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Kt)
{
	pmsm_dsn->flux = Kt / (CTL_CONST_PARAM_3_OVER_2 * pmsm_dsn->pole_pair);
}


//////////////////////////////////////////////////////////////////////////
// pmsm smo 

#include <ctl/component/motor/pmsm.smo.h>

void ctl_init_pmsm_smo(ctl_pmsm_smo_observer_t* smo)
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
	smo->k2 = CTRL_T(0.01);
	smo->k3 = 0;

	smo->k_filter_e = GMP_CONST_1_OVER_2;
	smo->k_filter_omega = GMP_CONST_1_OVER_2;

	smo->k_slide = CTRL_T(10.0);

	smo->theta_est = 0;
	ctl_set_phasor_via_angle(smo->theta_est, &smo->phasor);

	ctl_init_pid(&smo->pid_pll);

	smo->spd_sf = GMP_CONST_1;
	smo->spd_est_pu = 0;
	smo->wr = 0;
}

void ctl_setup_pmsm_smo(ctl_pmsm_smo_observer_t* smo,
	parameter_gt Rs, parameter_gt Ld, parameter_gt Lq, parameter_gt f_ctrl,
	parameter_gt fc_e, parameter_gt fc_omega,
	ctrl_gt pid_kp, ctrl_gt pid_ki, ctrl_gt pid_kd,
	ctrl_gt spd_max_limit, ctrl_gt spd_min_limit, // unit p.u.
	ctrl_gt k_slide, parameter_gt speed_base_rpm,
	uint16_t pole_pairs)
{
	smo->k1 = CTRL_T(1 / (Ld * f_ctrl));
	smo->k2 = CTRL_T(Rs / (Ld * f_ctrl));
	smo->k3 = CTRL_T((Ld - Lq) / (Ld * f_ctrl));

	smo->k_slide = k_slide;

	smo->k_filter_e = ctl_helper_lp_filter(f_ctrl, fc_e);
	smo->k_filter_omega = ctl_helper_lp_filter(f_ctrl, fc_omega);

	ctl_setup_pid(&smo->pid_pll,
		pid_kp, pid_ki, pid_kd, spd_min_limit, spd_max_limit);

	smo->spd_sf = CTRL_T((60 / 2 / PI) * f_ctrl / speed_base_rpm / pole_pairs);

}

void ctl_setup_pmsm_smo_via_consultant(ctl_pmsm_smo_observer_t* smo,
	// use it to calculate controller parameters
	ctl_pmsm_dsn_consultant_t* dsn,
	// use it to calculate controller parameters
	ctl_motor_driver_consultant_t* drv,
	// use it to per unit controller
	ctl_pmsm_nameplate_consultant_t* np,
	ctrl_gt pid_kp, ctrl_gt pid_ki, ctrl_gt pid_kd,
	ctrl_gt k_slide)
{
	ctl_setup_pmsm_smo(smo,
		dsn->Rs, dsn->Ld, dsn->Lq, drv->control_law_freq,
		drv->speed_closeloop_bw * 6.28, drv->speed_closeloop_bw * 6.28,
		pid_kp, pid_ki, pid_kd,
		CTRL_T(1.2), -CTRL_T(1.2), // unit p.u.
		k_slide, np->rated_speed_rpm, dsn->pole_pair);
}
