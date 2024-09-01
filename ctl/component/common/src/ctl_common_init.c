
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>


//////////////////////////////////////////////////////////////////////////
// PID regular 

#include <ctl/component/common/pid.h>

void ctl_init_pid(pid_regular_t* hpid)
{

	hpid->out = 0;

	hpid->kp = CTRL_T(1.0f);
	hpid->ki = 0;
	hpid->kd = 0;

	hpid->out_min = CTRL_T(-1.0f);
	hpid->out_max = CTRL_T(1.0f);

	hpid->dn = 0;
	hpid->sn = 0;
}

void ctl_setup_pid(pid_regular_t* hpid,
	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
	ctrl_gt out_min, ctrl_gt out_max)
{
	hpid->kp = kp;
	hpid->ki = ki;
	hpid->kd = kd;

	hpid->out_min = out_min;
	hpid->out_max = out_max;
}


void ctl_set_pid_parameter(
	pid_regular_t* hpid,
	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd
)
{
	hpid->kp = kp;
	hpid->ki = ki;
	hpid->kd = kd;
}

void ctl_set_pid_limit(
	pid_regular_t* hpid,
	ctrl_gt out_min, ctrl_gt out_max
)
{
	hpid->out_min = out_min;
	hpid->out_max = out_max;
}


//////////////////////////////////////////////////////////////////////////
// Slope Limiter

#include <ctl/component/common/slope_lim.h>

void ctl_init_slope_limit(slope_lim_t* obj)
{
	obj->slope_min = CTRL_T(-1.0f);
	obj->slope_max = CTRL_T(1.0f);

	obj->out_min = CTRL_T(-1.0f);
	obj->out_max = CTRL_T(1.0f);
}

void ctl_setup_slope_limit(
	slope_lim_t* obj,
	ctrl_gt slope_min, ctrl_gt slope_max,
	ctrl_gt out_min, ctrl_gt out_max
)
{
	obj->slope_min = slope_min;
	obj->slope_max = slope_max;

	obj->out_min = out_min;
	obj->out_max = out_max;
}

void ctl_set_sl_slope(
	slope_lim_t* obj,
	ctrl_gt slope_min, ctrl_gt slope_max
)
{
	obj->slope_min = slope_min;
	obj->slope_max = slope_max;
}

void ctl_set_sl_limit(
	slope_lim_t* obj,
	ctrl_gt out_min, ctrl_gt out_max
)
{
	obj->out_min = out_min;
	obj->out_max = out_max;
}


//////////////////////////////////////////////////////////////////////////
// Divider

#include <ctl/component/common/divider.h>

void ctl_init_divider(divider_t* obj)
{
	obj->counter = 0;

	// No division
	obj->target = 0;

	// No bypass
	obj->flag_bypass = 0;
}

void ctl_set_divider(divider_t* obj, uint32_t counter_period)
{
	obj->target = counter_period;
}

//////////////////////////////////////////////////////////////////////////
// Fusing

#include <ctl/component/common/fusing.h>

void ctl_init_bipolar_fusing(bipolar_fusing_t* fusing)
{
	fusing->target_object = NULL;

	fusing->upper_bound = CTRL_T(1.0f);
	fusing->lower_bound = -CTRL_T(1.0f);

	fusing->last_tick = 0;

	fusing->error_code = 1;

}

void ctl_bind_bipolar_fusing(bipolar_fusing_t* obj,
	ctrl_gt* target, gmp_stat_t error_code)
{
	obj->target_object = target;
	obj->error_code = error_code;
}

void ctl_set_bipolar_fusing_bound(bipolar_fusing_t* obj,
	ctrl_gt lower_bound, ctrl_gt upper_bound)
{
	obj->lower_bound = lower_bound;
	obj->upper_bound = upper_bound;
}

//////////////////////////////////////////////////////////////////////////
// Filter IIR2

#include <ctl/component/common/filter.h>
#include <math.h> // support for sinf and cosf

void ctl_clear_filter(filter_IIR2_t* obj)
{
	int i = 0;
	obj->out = 0;

	for (i = 0; i < 2; ++i)
	{
		obj->x[i] = 0;
		obj->y[i] = 0;
	}
}


void ctl_init_filter_iir2(filter_IIR2_t* obj)
{
	int i;

	obj->out = 0;

	for (i = 0; i < 2; ++i)
	{
		obj->x[i] = 0;
		obj->y[i] = 0;
		obj->a[i] = 0;
		obj->b[i] = 0;
	}

	obj->b[2] = 0;
}

void ctl_setup_filter_iir2(filter_IIR2_t* obj, filter_IIR2_setup_t* setup_obj)
{
	// center frequency
	//tex: $$ f_0 = f_c * 2Q$$
	parameter_gt f0 = setup_obj->fc * 2 * setup_obj->q;

	//tex: $$ \theta = 2\pi \frac{f_c}{f_s}$$
	parameter_gt theta = 2.0f * PI * f0 / setup_obj->fs;

	parameter_gt sin_theta = sinf(theta);

	parameter_gt cos_theta = cosf(theta);

	//tex: $$\alpha = \frac{\sin(\theta)}{2Q} $$
	parameter_gt alpha = sin_theta / 2 / setup_obj->q;

	// a_0, a_1, a_2
	parameter_gt a0 = (1.0f + alpha);
	obj->a[0] = CTRL_T(-2.0f * cos_theta / a0);
	obj->a[1] = CTRL_T((1.0f - alpha) / a0);

	switch (setup_obj->filter_type)
	{
	case FILTER_IIR2_TYPE_HIGHPASS:
		obj->b[0] = CTRL_T(setup_obj->gain * (1.0f + cos_theta) / (2 * a0));
		obj->b[1] = CTRL_T(-setup_obj->gain * (1.0f + cos_theta) / a0);
		obj->b[2] = CTRL_T(setup_obj->gain * (1.0f + cos_theta) / (2 * a0));
		break;
	case FILTER_IIR2_TYPE_LOWPASS:
		obj->b[0] = CTRL_T(setup_obj->gain * (1.0f - cos_theta) / (2 * a0));
		obj->b[1] = CTRL_T(setup_obj->gain * (1.0f - cos_theta) / a0);
		obj->b[2] = CTRL_T(setup_obj->gain * (1.0f - cos_theta) / (2 * a0));
		break;
	case FILTER_IIR2_TYPE_BANDPASS:
		obj->b[0] = CTRL_T(setup_obj->gain * sin_theta / (2 * a0));
		obj->b[1] = 0;
		obj->b[2] = CTRL_T(setup_obj->gain * sin_theta / (2 * a0));
		break;
	default:
		// do nothing
		break;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
// Signal Generator

#include <ctl/component/common/stimulate.h>

void ctl_init_sincos_gen(ctl_src_sg_t* sg)
{
	sg->ph_sin = CTRL_T(0);
	sg->ph_cos = CTRL_T(1.0f);

	sg->ph_sin_delta = CTRL_T(0);
	sg->ph_cos_delta = CTRL_T(1.0f);
}

void ctl_setup_sincos_gen(ctl_src_sg_t* sg,
	ctrl_gt init_angle, // rad
	ctrl_gt step_angle) // rad
{
	sg->ph_cos = ctrl_cos(init_angle);
	sg->ph_sin = ctrl_sin(init_angle);

	sg->ph_sin_delta = ctrl_sin(step_angle);
	sg->ph_cos_delta = ctrl_cos(step_angle);
}

void ctl_init_ramp_gen(ctl_src_rg_t* rg)
{
	rg->current = CTRL_T(0);
	rg->maximum = CTRL_T(1.0f);
	rg->minimum = CTRL_T(0);

	rg->slope = CTRL_T(0);
}

void ctl_setup_ramp_gen(ctl_src_rg_t* rg, ctrl_gt slope)
{
	rg->slope = slope;
}

void ctl_setup_ramp_gen_via_amp_freq(ctl_src_rg_t* rg,
	parameter_gt isr_freq, parameter_gt target_freq,
	parameter_gt amp_pos, parameter_gt amp_neg)
{
	rg->maximum = CTRL_T(amp_pos);
	rg->minimum = CTRL_T(amp_neg);

	rg->slope = CTRL_T((amp_pos-amp_neg)/(isr_freq/ target_freq));
}

//////////////////////////////////////////////////////////////////////////
// Track_PID.h
// 

#include <ctl/component/common/track_pid.h>

void ctl_init_track_pid(track_pid_t* tp)
{
	ctl_init_divider(&tp->div);
	ctl_init_slope_limit(&tp->traj);
	ctl_init_pid(&tp->pid);
	tp->out = 0;
}

void setup_init_track_pid(track_pid_t* tp,
	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // pid parameters
	ctrl_gt sat_min, ctrl_gt sat_min, // saturation limit
	ctrl_gt slope_min, ctrl_gt slope_max, // slope limit
	uint32_t division //division factor
)
{
	ctl_setup_slope_limit(&tp->traj, slope_min, slope_max, sat_min, sat_max);
	ctl_set_divider(&tp->div, division);
	ctl_setup_pid(&tp->pid, kp, ki, kd, sat_min, sat_max);
}
