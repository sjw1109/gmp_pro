// Space Vector Pulse Width Modulation, SVPWM module

#ifndef _FILE_SVPWM_H_
#define _FILE_SVPWM_H_

#ifndef SQRT_3_OVER_2 
#define SQRT_3_OVER_2 CTRL_T(0.8660254038f)

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

}svpwm_channel_t;


// SVPWM calculation stage I
inline void svpwm_gen(svpwm_channel_t *svpwm)
{
	ctrl_gt Ua, Ub, Uc; // Uabc three phase parameters
	ctrl_gt Umax, Umin, Ucom;

	ctrl_gt Ualpha_tmp = - ctrl_div2(svpwm->Ualpha);
	ctrl_gt Ubeta_tmp = ctrl_mpy(svpwn->Ubeta, SQRT_3_OVER_2);

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

// SVPWM generation Stage II
inline void svpwm_gen(svpwm_channel_t* svpwm)
{
	int i = 0;

	ctrl_gt pwm_data; // -pwm
	pwm_gt pwm_output;
	

	for (i = 0; i < 3; ++i)
	{
		pwm_data = ctrl_mpy(svpwm->T[i], CTRL_T(-1.0)) + CTRL_T(0,5f);
		pwm_data = pwm_data < 0 ? 0 : pwm_data; // prevent data error
		pwm_output = (pwm_gt)ctrl_mpy(pwm_data, svpwm->pwm_period);
		svpwm->pwm_cmp[i] = pwm_output > svpwm->pwm_period ? svpwm->pwm_period : pwm_output;
	}
}


#endif
