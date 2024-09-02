
#ifndef _FILE_PMSM_CONSULTANT_H_
#define _FILE_PMSM_CONSULTANT_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define CTL_CONST_PARAM_2_SQRT_6     ((4.89897948556635619))
#define CTL_CONST_PARAM_100PI_OVER_3 ((104.7197551196597746))
#define CTL_CONST_PARAM_SQRT_2       ((1.4142135623730950488))
#define CTL_CONST_PARAM_3_OVER_2     ((1.5))

	//////////////////////////////////////////////////////////////////////////
	// The following parameters are for used for p.u. base value
	// 
	// Constrains of motor rated parameters
	// These parameters will be used as pu base value
	// 
	// Motor Rated parameters:  
	//tex:
	// Motor speed: $n_N$, motor power: $P_N$, Motor Voltage: $V_N$,
	// Motor Current: $I_N$, motor frequency: $f_N$, motor torque: $T_N$,
	// Motor power factor: $\varphi$.

	// Motor Design parameters:
	//tex:
	// Motor pole pair: $p$, motor flux linkage: $\psi_f$ 

	// Constrains:
	//tex:
	// Speed and frequency: $n_N =60\cdot f_N/p$
	//tex:
	// Power: $P_N = \sqrt{3}\cdot I_N\cdot U_N$
	//tex:
	// torque: $T_N = \frac{30}{\pi} \cdot \frac{P_N}{n_N} $
	//tex:
	// Back electromotive constant: $\psi_f = \frac{V_N-(R_s\cdot I_N-jI_dX_d-jI_qX_q)}{2\pi f_N}$


	// Motor rated parameters
	// unit, 1
#ifndef MOTOR_RATED_POLE_PAIRS
#define MOTOR_RATED_POLE_PAIRS ((4))
#endif // MOTOR_RATED_POLE_PAIRS


	// Motor rated parameters
	// unit, rpm
#ifndef MOTOR_RATED_SPEED_RPM
#define MOTOR_RATED_SPEED_RPM ((3000.0))
#endif // MOTOR_RATED_SPEED_RPM

	// Motor rated parameters
	// unit, SI, W
#ifndef MOTOR_RATED_OUTPUT_POWER
#define MOTOR_RATED_OUTPUT_POWER ((245.0))
#endif // MOTOR_RATED_OUTPUT_POWER

	// Motor rated parameters
	// unit, SI, V rms (Line voltage)
#ifndef MOTOR_RATED_VOLTAGE
#define MOTOR_RATED_VOLTAGE   ((300.0))
#endif // MOTOR_RATED_VOLTAGE

	// Motor rated parameters
	// unit, SI, A rms (Line current)
#ifndef MOTOR_RATED_CURRENT
#define MOTOR_RATED_CURRENT   ((44.1))
#endif // MOTOR_RATED_CURRENT

	// Motor rated parameters
	// unit, SI, Hz
#ifndef MOTOR_RATED_FREQUENCY
#define MOTOR_RATED_FREQUENCY ((200.0))
#endif // MOTOR_RATED_FREQUENCY

	// Motor rated parameters
	// unit, SI, N.m
#ifndef MOTOR_RATED_TORQUE
#define MOTOR_RATED_TORQUE    ((0.8))
#endif // MOTOR_RATED_TORQUE

	// Motor Power factor
	// unit, %
#ifndef MOTOR_RATED_POWER_FACTOR
#define MOTOR_RATED_POWER_FACTOR ((0.8))
#endif // MOTOR_RATED_POWER_FACTOR

	// Motor rated efficiency
#ifndef MOTOR_RATED_EFFICIENCY
#define MOTOR_RATED_EFFICIENCY   ((0.9))
#endif // MOTOR_RATED_EFFICIENCY

	// With this, it's no need to use init and setup function 
#define MOTOR_PMSM_NAMEPLATE_CONSULTANT_WRAPPER {\
	MOTOR_RATED_POLE_PAIRS,\
	MOTOR_RATED_OUTPUT_POWER,\
	MOTOR_RATED_VOLTAGE,\
	MOTOR_RATED_CURRENT,\
	MOTOR_RATED_POWER_FACTOR, \
	MOTOR_RATED_FREQUENCY,\
	MOTOR_RATED_SPEED_RPM, \
	MOTOR_RATED_TORQUE,\
	MOTOR_RATED_EFFICIENCY \ 
	}

	typedef struct _tag_pmsm_nameplate_consultant_t
	{
		// Motor rated parameters
		// unit 1
		uint16_t pole_pairs;

		// Motor rated parameters
		// unit, SI, W
		parameter_gt rated_output_power;

		// Motor rated parameters
		// unit, SI, V rms (L-L voltage)
		parameter_gt rated_voltage;

		//Motor rated parameters
		// unit, SI, A rms (L-L current)
		parameter_gt rated_current;

		// Motor parameters, power factor
		// unit, 1
		parameter_gt power_factor;

		// Motor rated parameters
		// unit, SI, Hz
		parameter_gt rated_freq;

		// Motor rated parameters
		// unit, rpm
		parameter_gt rated_speed_rpm;

		// Motor rated parameters
		// unit, SI, N.m
		parameter_gt rated_torque;

		// Motor parameters, efficiency
		parameter_gt eta;
	}ctl_pmsm_nameplate_consultant_t;

	// init pmsm nameplate consultant object
	void ctl_init_pmsm_nameplate_consultant(ctl_pmsm_nameplate_consultant_t* np);

	// setup pmsm nameplate consultant object
	void ctl_setup_pmsm_nameplate_consultant(ctl_pmsm_nameplate_consultant_t* np,
		parameter_gt rated_output_power, parameter_gt rated_voltage, parameter_gt rated_current,
		parameter_gt rated_freq, parameter_gt rated_speed, parameter_gt rated_torque,
		parameter_gt power_factor, parameter_gt efficiency
	);



	//////////////////////////////////////////////////////////////////////////
	// The following parameters are for controller auto tuning.
	// 
	// Motor Design parameters, Pole pairs
	// unit, 1
#ifndef MOTOR_DSN_POLE_PAIR
#define MOTOR_DSN_POLE_PAIR   MOTOR_RATED_POLE_PAIRS
#endif // MOTOR_DSN_POLE_PAIR

	// Motor Design parameters, stator parameters
	// unit, SI Ohm
#ifndef MOTOR_DSN_STATOR_RES
#define MOTOR_DSN_STATOR_RES  ((0.18))
#endif // MOTOR_DSN_STATOR_RES

	// Motor Design parameters, Armature inductance Ld
	// unit, SI H
#ifndef MOTOR_DSN_LD
#define MOTOR_DSN_LD          ((0.835e-3))
#endif // MOTOR_DSN_LD

	// Motor Design parameters, Armature inductance Lq
	// unit, SI H
#ifndef MOTOR_DSN_LQ
#define MOTOR_DSN_LQ          ((0.835e-3))
#endif // MOTOR_DSN_LQ

	// Motor Design parameters, flux linkage
	// unit, Wb, SI V.s
#ifndef MOTOR_DSN_FLUX_WB
#define MOTOR_DSN_FLUX_WB      ((0.0714394))
#endif // MOTOR_DSN_FLUX_WB

	// Motor Design parameters, inertia
	// unit, kg.m/s2
#ifndef MOTOR_DSN_INERTIA
#define MOTOR_DSN_INERTIA      ((0.621417e-3))
#endif // MOTOR_DSN_INERTIA

	// Motor Design parameters, damping
	// unit, N.m.s
#ifndef MOTOR_DSN_DAMP
#define MOTOR_DSN_DAMP			((5e-6))
#endif // MOTOR_DSN_DAMP


	// With this, it's no need to use init and setup function 
#define MOTOR_PMSM_DESIGN_CONSULTANT_WRAPPER {\
	MOTOR_DSN_POLE_PAIR,\
	MOTOR_DSN_STATOR_RES,\
	MOTOR_DSN_LD,\
	MOTOR_DSN_LQ,\
	MOTOR_DSN_FLUX_WB, \
	MOTOR_DSN_INERTIA,\
	MOTOR_DSN_DAMP \ 
	}



	typedef struct _tag_pmsm_dsn_consultant
	{
		// Motor Design parameters, Pole pairs
		// unit, 1
		uint16_t pole_pair;

		// Motor Design parameters, stator parameters
		// unit, SI Ohm
		parameter_gt Rs;

		// Motor Design parameters, Armature inductance Ld
		// unit, SI H
		parameter_gt Ld;

		// Motor Design parameters, Armature inductance Lq
		// unit, SI H
		parameter_gt Lq;

		// Motor Design parameters, flux linkage
		// unit, Wb, SI V.s (that is, Vp/(rad/s))
		parameter_gt flux;

		// Motor Design parameters, inertia
		// unit, kg.m/s2
		parameter_gt inertia;

		// Motor Design parameters, damping
		parameter_gt damp;

	}ctl_pmsm_dsn_consultant_t;

	// init the pmsm design consultant object
	void ctl_init_pmsm_dsn_consultant(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// copy parameters from macros
	void ctl_import_pmsm_dsn_default_param(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// setup the pmsm design consultant object
	// if any parameter is unknown, fill 0, except pole_pair
	void ctl_setup_pmsm_dsn_consultant(ctl_pmsm_dsn_consultant_t* pmsm_dsn,
		uint16_t pole_pair,
		parameter_gt Rs,
		parameter_gt Ld, parameter_gt Lq,
		parameter_gt flux,
		parameter_gt inertia, parameter_gt damp);

	// design pmsm parameters via line-line Rs
	void ctl_dsn_pmsm_Rs_via_RsLL(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Rll);

	// design Round-rotor pmsm via line-line Ls
	void ctl_dsn_pmsm_Ls_via_Lsll(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Lll);

	// design Load feature
	void ctl_dsn_pmsm_load(ctl_pmsm_dsn_consultant_t* pmsm_dsn,
		parameter_gt inertia, parameter_gt damp);

	// Get flux parameters from consultant
	parameter_gt ctl_consult_pmsm_flux_Wb(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// Ke: PMSM Voltage Constant, 
	// output unit, Vrms(L-L) / (rad/s)
	parameter_gt ctl_consult_pmsm_Ke(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// Ke: PMSM Voltage Constant,
	// output unit, Vp(L-L) / krpm
	parameter_gt ctl_consult_pmsm_Ke_Vp_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// Ke: PMSM Voltage Constant,
	// output unit, Vrms(L-L) / krpm
	parameter_gt ctl_consult_pmsm_Ke_Vrms_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// Kt: PMSM Torque Constant,
	// output unit, (N.m)/(Ap)
	parameter_gt ctl_consult_pmsm_Kt(ctl_pmsm_dsn_consultant_t* pmsm_dsn);

	// Set pmsm consultant Flux parameters
	// Unit, Wb
	void ctl_set_pmsm_flux_Wb(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt flux);

	// Assign motor flux parameter via Ke
	// Ke unit, Vrms(L-L) / (rad/s)
	void ctl_dsn_pmsm_flux_via_Ke(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Ke);

	// Assign motor flux parameter via Ke
	// Ke unit, Vp(L-L) / krpm
	void ctl_dsn_pmsm_flux_via_Ke_Vp_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Ke);

	// Assign motor flux parameter via Ke
	// Ke unit, rms(L-L) / krpm
	void ctl_dsn_pmsm_flux_via_Ke_Vrms_krpm(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Ke);

	// Assign motor flux parameter via Kt
	// Kt unit, (N.m)/(Ap)
	void ctl_dsn_pmsm_flux_via_Kt(ctl_pmsm_dsn_consultant_t* pmsm_dsn, parameter_gt Kt);

	//void ctl_setup_pmsm_nameplate_via_dsn(ctl_pmsm_nameplate_consultant_t* np,
	//	ctl_pmsm_dsn_consultant_t* pmsm_dsn,
	//	parameter_gt torque_plan, parameter_gt speed_rpm_plan
	//)
	//{
	//	np->rated_current = torque_plan / ctl_consult_pmsm_Kt(&pmsm_dsn);

	//	np->rated_voltage = ctl_consult_pmsm_Ke_Vrms_krpm(&pmsm_dsn) * speed_rpm_plan
	//		+ np->rated_current * Rs + np->rated_current * 2 * 3.14 *;


	//}




#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PMSM_CONSULTANT_H_
