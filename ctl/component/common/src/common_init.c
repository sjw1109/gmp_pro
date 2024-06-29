
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// ADC channel

#include <ctl/component/common/adc_channel.h>

void init_adc_channel(adc_channel_t* adc_obj)
{
	adc_obj->raw = 0;
	adc_obj->resolution = 0;
	adc_obj->iqn = 0;
	adc_obj->bias = 0;
	adc_obj->gain = CTRL_T(1.0f);
	adc_obj->value = 0;
}

void setup_adc_channel(adc_channel_t* adc_obj,
	ctrl_gt gain,
	ctrl_gt bias,
	fast_gt resolution,
	fast_gt iqn
)
{
	adc_obj->bias = bias;
	adc_obj->bias = gain;
	adc_obj->iqn = iqn;
	adc_obj->resolution = resolution;
}

void calc_adc_channel(adc_channel_t* adc_obj)
{
	// transfer ADC data to _IQ24
	ctrl_gt raw_data = adc_obj->raw << (GLOBAL_Q - adc_obj->iqn);

	// remove bias
	ctrl_gt raw_without_bias = raw_data - adc_obj->bias;

	// Gain
	adc_obj->value = ctrl_mpy(raw_without_bias, adc_obj->gain);

	return;
}


//////////////////////////////////////////////////////////////////////////
// PWM channel

#include <ctl/component/common/pwm_channel.h>

void init_pwm_channel(pwm_channel_t* pwm_obj)
{
	pwm_obj->raw = 0;
	pwm_obj->value = 0;
}

void setup_pwm_channel(pwm_channel_t* pwm_obj, pwm_gt phase, pwm_gt full_scale)
{
	pwm_obj->full_scale = full_scale;
	pwm_obj->phase = phase;
}


void calc_pwm_channel(pwm_channel_t* pwm_obj)
{
	pwm_obj->value = pwm_mpy(pwm_obj->raw, pwm_obj->full_scale) + pwm_obj->phase;
	pwm_obj->value = pwm_sat(pwm_obj->value, pwm_obj->full_scale, 0);
}

void calc_pwm_channel_warp(pwm_channel_t* pwm_obj)
{
	pwm_obj->value = pwm_mpy(pwm_obj->raw, pwm_obj->full_scale) + pwm_obj->phase;
	pwm_obj->value = pwm_obj->value % pwm_obj->full_scale;
}

void calc_inv_pwm_channel(pwm_channel_t* pwm_obj)
{
	pwm_obj->value = pwm_obj->full_scale - pwm_mpy(pwm_obj->raw, pwm_obj->full_scale);
	pwm_obj->value = pwm_sat(pwm_obj->value, pwm_obj->full_scale, 0);
}



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


void ctl_pid_set_parameter(
	pid_regular_t* hpid,
	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd
)
{
	hpid->kp = kp;
	hpid->ki = ki;
	hpid->kd = kd;
}

void ctl_pid_set_limit(
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

void ctl_set_slope_limit_slope(
	slope_lim_t* obj,
	ctrl_gt slope_min, ctrl_gt slope_max
)
{
	obj->slope_min = slope_min;
	obj->slope_max = slope_max;
}

void ctl_set_limit_limit_slope(
	slope_lim_t* obj,
	ctrl_gt out_min, ctrl_gt out_max
)
{
	obj->out_min = out_min;
	obj->out_max = out_max;
}


//////////////////////////////////////////////////////////////////////////
// Divider

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

