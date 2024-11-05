/**
 * @file svpwm.h
 * @author Javnson (javnson@zju.edu.cn), GUO Qichen
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// Space Vector Pulse Width Modulation, SVPWM module

#ifndef _FILE_SVPWM_H_
#define _FILE_SVPWM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_svpwm_channel_t
{
	// input variables, $$U_\alpha$$, $$U_\beta$$
	ctrl_gt Ualpha;
	ctrl_gt Ubeta;

	// output stage I variables: Ta, Tb, Tc
	ctrl_gt T[3];

	// parameters: PWM time base period
	pwm_gt pwm_period;

	// output stage II variables: modulation output
	pwm_gt pwm_cmp[3];

} ctl_svpwm_channel_t;


void ctl_init_svpwm(ctl_svpwm_channel_t *svpwm);

void ctl_setup_svpwm(ctl_svpwm_channel_t *svpwm, pwm_gt pwm_period);

GMP_STATIC_INLINE
void ctl_set_svpwm_via_ab(ctl_svpwm_channel_t *svpwm, ctl_vector3_t *ab)
{
	svpwm->Ualpha = ab->dat[0];
	svpwm->Ubeta = ab->dat[1];
}

// SVPWM calculation stage I
// alpha-beta -> Tabc
GMP_STATIC_INLINE
void ctl_svpwm_calc(ctl_svpwm_channel_t *svpwm)
{
	ctrl_gt Ua, Ub, Uc; // Uabc three phase parameters
	ctrl_gt Umax, Umin, Ucom;

	ctrl_gt Ualpha_tmp = -ctrl_div2(svpwm->Ualpha);
	ctrl_gt Ubeta_tmp = ctrl_mpy(svpwm->Ubeta, GMP_CONST_SQRT_3_OVER_2);

	//tex: $$
	//U_a = U_\alpha, \\
	//U_b = -U_\alpha /2 + \sqrt{3}/2\cdot U_\beta, \\
	//U_c = -U_\alpha /2 - \sqrt{3}/2\cdot U_\beta,
	//$$

	Ua = svpwm->Ualpha;
	Ub = Ualpha_tmp + Ubeta_tmp;
	Uc = Ualpha_tmp - Ubeta_tmp;

	// find Vmax & Vmin
	if (Ua > Ub)
	{
		Umax = Ua;
		Umin = Ub;
	}
	else
	{
		Umax = Ub;
		Umin = Ua;
	}

	if (Uc > Umax)
		Umax = Uc;
	else if (Uc < Umin)
		Umin = Uc;

	// get common mode term
	Ucom = ctrl_div2(Umax + Umin);

	// get SVPWM modulation result
	svpwm->T[0] = Ua - Ucom;
	svpwm->T[1] = Ub - Ucom;
	svpwm->T[2] = Uc - Ucom;
}


// SVPWM calculation stage I
// another formal algorithm
// alpha-beta -> Tabc
GMP_STATIC_INLINE
void ctl_svpwm_calc2(ctl_svpwm_channel_t *svpwm)
{

	// u2s: Ualpha ,Ubeta
	ctrl_gt X, Y, Z, T1, T2, Ta, Tb, Tc;
	uint16_t N;
	ctrl_gt Uabc[3] = { 0 };

	Uabc[0] = svpwm->Ubeta;
	Uabc[1] = ctrl_mpy(GMP_CONST_SQRT_3_OVER_2,svpwm->Ualpha) - ctrl_div2(svpwm->Ubeta);
	Uabc[2] = -ctrl_mpy(GMP_CONST_SQRT_3_OVER_2, svpwm->Ualpha) - ctrl_div2(svpwm->Ubeta);

	N = ((Uabc[0] > 0)) + ((Uabc[1] > 0) << 1) + ((Uabc[2] > 0) << 2);
	X = ctrl_mpy(GMP_CONST_SQRT_3, svpwm->Ubeta);
	Y = ctrl_mpy(GMP_CONST_3_OVER_2, svpwm->Ualpha) + ctrl_mpy(GMP_CONST_SQRT_3_OVER_2, svpwm->Ubeta);
	Z = -ctrl_mpy(GMP_CONST_3_OVER_2, svpwm->Ualpha) + ctrl_mpy(GMP_CONST_SQRT_3_OVER_2, svpwm->Ubeta);

	switch (N)
	{
	case 1:
		T1 = Z;
		T2 = Y;
		break;
	case 2:
		T1 = Y;

		T2 = -X;
		break;
	case 3:
		T1 = -Z;
		T2 = X;
		break;
	case 4:
		T1 = -X;
		T2 = Z;
		break;
	case 5:
		T1 = X;
		T2 = -Y;
		break;
	case 6:
		T1 = -Y;
		T2 = -Z;
		break;
	default:
		break;
	}
	if ((T1 + T2) > GMP_CONST_1)
	{
		T1 = ctrl_div(T1 , (T1 + T2));
		T2 = GMP_CONST_1 - T1;
	}
	Ta = ctrl_div4(GMP_CONST_1 - T1 - T2);
	Tb = Ta + ctrl_div2(T1);
	Tc = Tb + ctrl_div2(T2);

	Ta *= 2;
	Tb *= 2;
	Tc *= 2;

	Ta = -Ta;
	Tb = -Tb;
	Tc = -Tc;

	Ta += GMP_CONST_1_OVER_2;
	Tb += GMP_CONST_1_OVER_2;
	Tc += GMP_CONST_1_OVER_2;

	switch (N)
	{
	case 0:
		svpwm->T[0] = 0;
		svpwm->T[1] = 0;
		svpwm->T[2] = 0;
		break;
	case 1:
		svpwm->T[0] = Tb;
		svpwm->T[1] = Ta;
		svpwm->T[2] = Tc;
		break;
	case 2:
		svpwm->T[0] = Ta;
		svpwm->T[1] = Tc;
		svpwm->T[2] = Tb;
		break;
	case 3:
		svpwm->T[0] = Ta;
		svpwm->T[1] = Tb;
		svpwm->T[2] = Tc;
		break;
	case 4:
		svpwm->T[0] = Tc;
		svpwm->T[1] = Tb;
		svpwm->T[2] = Ta;
		break;
	case 5:
		svpwm->T[0] = Tc;
		svpwm->T[1] = Ta;
		svpwm->T[2] = Tb;
		break;
	case 6:
		svpwm->T[0] = Tb;
		svpwm->T[1] = Tc;
		svpwm->T[2] = Ta;
		break;
	default:
		svpwm->T[0] = 0;
		svpwm->T[1] = 0;
		svpwm->T[2] = 0;
		break;
	}

	return;
}

// SVPWM generation Stage II
// Tabc -> cmp vlaue
GMP_STATIC_INLINE
void ctl_svpwm_modulation(ctl_svpwm_channel_t *svpwm)
{
	int i = 0;

	ctrl_gt pwm_data; // -pwm
	pwm_gt pwm_output;


	for (i = 0; i < 3; ++i)
	{
		//pwm_data = ctrl_mpy(svpwm->T[i], CTRL_T(-1.0)) + CTRL_T(0.5f);
		pwm_data = svpwm->T[i] + CTRL_T(0.5f);
		pwm_data = pwm_data < 0 ? 0 : pwm_data; // prevent data error
		pwm_output = (pwm_gt)ctrl_mpy(pwm_data, svpwm->pwm_period);
		svpwm->pwm_cmp[i] = pwm_output > svpwm->pwm_period ? svpwm->pwm_period : pwm_output;
	}
}

// SVPWM generation Stage II
// with inverse modulation output
// Tabc -> cmp vlaue
GMP_STATIC_INLINE
void ctl_svpwm_inv_modulation(ctl_svpwm_channel_t *svpwm)
{
	int i = 0;

	ctrl_gt pwm_data; // -pwm
	pwm_gt pwm_output;


	for (i = 0; i < 3; ++i)
	{
		// change modulation direction
		pwm_data = ctrl_mpy(svpwm->T[i], CTRL_T(-1.0)) + CTRL_T(0.5f);

		// common modulation routine
		pwm_data = svpwm->T[i] + CTRL_T(0.5f);
		pwm_data = pwm_data < 0 ? 0 : pwm_data; // prevent data error
		pwm_output = (pwm_gt)ctrl_mpy(pwm_data, svpwm->pwm_period);
		svpwm->pwm_cmp[i] = pwm_output > svpwm->pwm_period ? svpwm->pwm_period : pwm_output;
	}
}


#ifdef __cplusplus
}
#endif


#endif // _FILE_SVPWM_H_
