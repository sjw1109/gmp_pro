
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
	adc_obj->gain = gain;
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

// ADC calibrator
void init_adc_bias_calibrator(adc_bias_calibrator_t* obj)
{
	obj->bias_output = 0;
	obj->flag_output_valid = 0;
	obj->raw = 0;

	obj->total_period = 0;
	obj->current_period = 0;
	obj->start_period = 0;
	obj->filter_tick = 0;

	ctl_init_filter_iir2(&obj->filter);

}

void setup_adc_bias_calibrator(adc_bias_calibrator_t* obj,
	filter_IIR2_setup_t* filter_parameter)
{
	uint32_t total_period = 10 * filter_parameter->fc;

	// setup the filter
	ctl_setup_filter_iir2(&obj->filter, filter_parameter);

	// at least 1000 period
	if (total_period > 1000)
		obj->total_period = total_period;
	else
		obj->total_period = 1000;
	
}

void restart_adc_bias_calibrator(adc_bias_calibrator_t* obj)
{
	obj->start_period = 0;
	obj->filter_tick = 0;
	obj->flag_output_valid = 0;

	clear_filter(&obj->filter);
}

// return value means if the calibration output is valid
fast_gt run_adc_bias_calibrator(
	adc_bias_calibrator_t* obj,
	uint32_t main_isr_tick,
	ctrl_gt adc_value)
{
	if (obj->start_period == 0)
	{
		// need a brand new start
		obj->start_period = main_isr_tick;
		obj->flag_output_valid = 0;
	}

	// if calibration has not complete
	if (!obj->flag_output_valid)
	{
		// need to call the filter once.
		if (obj->current_period != main_isr_tick)
		{
			ctl_filter_iir2_calc(&obj->filter, adc_value);
			obj->filter_tick += 1;
		}
		// break the whole process if current_period == main_isr_tick
		else
		{
			return 0;
		}

		obj->current_period = main_isr_tick;

		// Calculate delta tick
		uint32_t delta_tick;

		if (obj->current_period >= obj->start_period)
		{
			delta_tick = obj->current_period - obj->start_period;
		}
		else
		{
			delta_tick = UINT32_MAX - obj->start_period + obj->current_period;
		}

		// complete the calibrate
		if (delta_tick > obj->total_period)
		{
			obj->bias_output = obj->filter.out;
			obj->flag_output_valid = 1;
			return 1;
		}
		return 0;
	}
	return 1;

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

void ctl_bipolar_fusing_bind(bipolar_fusing_t* obj,
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

void clear_filter(filter_IIR2_t* obj)
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
