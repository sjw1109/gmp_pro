

// This module is a PMSM SMO module

// include pll module
#include <ctl/component/common/pll.h>
#include <ctl/component/motor/motor_driver_consultant.h>
#include <ctl/component/motor/pmsm_consultant.h>

#ifndef _FILE_PMSM_SMO_H_
#define _FILE_PMSM_SMO_H_

#ifdef __cplusplus
extern"C"
{
#endif // __cplusplus

	typedef struct _tag_pmsm_smo_observer_t
	{
		// input section
		ctrl_gt u_alpha;
		ctrl_gt u_beta;

		ctrl_gt i_alpha;
		ctrl_gt i_beta;

		// output section
		// output estimated theta, unit rad
		ctrl_gt theta_est;
		// output estimated speed, unit p.u.
		ctrl_gt spd_est_pu;
		// phasor for theta
		ctl_vector2_t phasor;

		// intermediate variable section

		// i estimate
		ctrl_gt i_alpha_est;
		ctrl_gt i_beta_est;

		// EMF estimate
		ctrl_gt e_alpha_est;
		ctrl_gt e_beta_est;

		// slide model output 
		ctrl_gt z_alpha;
		ctrl_gt z_beta;


		// parameters
		// angular velocity
		ctrl_gt wr;

		// k1: Ts*1/Ld
		ctrl_gt k1;

		// k2: Ts*Rs/Ld
		ctrl_gt k2;

		// k3: (Ld-Lq)*Ts*1/Ld
		// coupling term
		ctrl_gt k3;

		// k: Slide Model gain
		ctrl_gt k_slide;

		// k filter: E filter
		ctrl_gt k_filter_e;

		// k filter: omega filter
		ctrl_gt k_filter_omega;

		// PLL pid controller
		ctl_pid_t pid_pll;

		// speed scale factor
		// scale factor: rad/tick -> p.u.
		ctrl_gt spd_sf;
	}ctl_pmsm_smo_observer_t;

	void ctl_init_pmsm_smo(ctl_pmsm_smo_observer_t* smo);
	
	void ctl_setup_pmsm_smo(ctl_pmsm_smo_observer_t* smo,
		parameter_gt Rs, parameter_gt Ld, parameter_gt Lq, parameter_gt f_ctrl,
		parameter_gt fc_e, parameter_gt fc_omega,
		ctrl_gt pid_kp, ctrl_gt pid_ki, ctrl_gt pid_kd,
		ctrl_gt spd_max_limit, ctrl_gt spd_min_limit, // unit p.u.
		ctrl_gt k_slide, parameter_gt speed_base_rpm,
		uint16_t pole_pairs);

	void ctl_setup_pmsm_smo_via_consultant(ctl_pmsm_smo_observer_t* smo,
		// use it to calculate controller parameters
		ctl_pmsm_dsn_consultant_t* dsn,
		// use it to calculate controller parameters
		ctl_motor_driver_consultant_t* drv,
		// use it to per unit controller
		ctl_pmsm_nameplate_consultant_t* np,
		ctrl_gt pid_kp, ctrl_gt pid_ki, ctrl_gt pid_kd, 
		ctrl_gt k_slide);

	GMP_STATIC_INLINE
		void ctl_input_pmsm_smo(ctl_pmsm_smo_observer_t* smo,
			ctrl_gt u_alpha, ctrl_gt u_beta, ctrl_gt i_alpha, ctrl_gt i_beta)
	{
		smo->i_alpha = i_alpha;
		smo->i_beta = i_beta;
		smo->u_alpha = u_alpha;
		smo->u_beta = u_beta;
	}

	GMP_STATIC_INLINE
	ctrl_gt ctl_step_pmsm_smo(ctl_pmsm_smo_observer_t* smo)
	{
		// Model
		ctrl_gt delta_i_alpha = ctrl_mpy(smo->k1, smo->u_alpha - smo->e_alpha_est - smo->z_alpha)
			- ctrl_mpy(smo->k2, smo->i_alpha_est)
			- ctrl_mpy(ctrl_mpy(smo->wr, smo->k3), smo->i_beta_est);

		ctrl_gt delta_i_beta = ctrl_mpy(smo->k1, smo->u_beta - smo->e_beta_est - smo->z_beta)
			- ctrl_mpy(smo->k2, smo->i_beta_est)
			+ ctrl_mpy(ctrl_mpy(smo->wr, smo->k3), smo->i_alpha_est);

		// Step i est
		smo->i_alpha_est += delta_i_alpha;
		smo->i_beta_est += delta_i_beta;

		// Slide model
		smo->z_alpha = ctrl_mpy(ctrl_sat(smo->i_alpha_est - smo->i_alpha, CTRL_T(0.01), -CTRL_T(0.01)),
			smo->k_slide);
		smo->z_beta = ctrl_mpy(ctrl_sat(smo->i_beta_est - smo->i_beta, CTRL_T(0.01), -CTRL_T(0.01)),
			smo->k_slide);

		// Filter and get e est
		smo->e_alpha_est = ctrl_mpy(smo->z_alpha, smo->k_filter_e)
			+ ctrl_mpy(smo->e_alpha_est, CTRL_T(1.0) - smo->k_filter_e);
		smo->e_beta_est = ctrl_mpy(smo->z_beta, smo->k_filter_e)
			+ ctrl_mpy(smo->e_beta_est, CTRL_T(1.0) - smo->k_filter_e);

		// PLL get angle out
		// 0. generate phasor
		ctl_set_phasor_via_angle(smo->theta_est, &smo->phasor);

		// 1. error signal generate
		ctrl_gt e_error = -ctrl_mpy(smo->e_alpha_est, smo->phasor.dat[1])
			- ctrl_mpy(smo->e_beta_est, smo->phasor.dat[0]);
		
		// 2. PLL speed lock routine
		ctl_step_pid_ser(&smo->pid_pll, e_error);

		// 3. filter speed, unit rad/tick
		smo->wr = ctrl_mpy(smo->pid_pll.out, smo->k_filter_omega)
			+ ctrl_mpy(smo->wr, CTRL_T(1.0) - smo->k_filter_omega);
		
		// 4. update theta
		smo->theta_est += smo->wr;
		
		if(smo->theta_est > GMP_CONST_2_PI)
			smo->theta_est -= GMP_CONST_2_PI;
		else if(smo->theta_est < 0)
			smo->theta_est += GMP_CONST_2_PI;
		

		// 5. update speed
		smo->spd_est_pu = ctrl_mpy(smo->wr, smo->spd_sf);

		return smo->theta_est;
	}


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PMSM_SMO_H_


