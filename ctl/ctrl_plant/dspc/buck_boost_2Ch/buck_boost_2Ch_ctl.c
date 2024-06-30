
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>


// This function may initialize a buck_boost_2ch object
void init_buck_boost_2ch_ctl(buck_boost_2ch_ctl_object_t* ctl_obj)
{
	int i;
	ctrl_gt max_slope;

	// init input & output parameters
	for (i = 0; i < 6; ++i)
	{
		init_adc_channel(&ctl_obj->adc_results[i]);

		if (i < 3) // for current sensor
			setup_adc_channel(&ctl_obj->adc_results[i],
				CTRL_T(CONTROLLER_I_ADC_GAIN), // ADC GAIN: from ADC result to p.u.
				CTRL_T(CONTROLLER_I_ADC_BIAS), // ADC BIAS: from A to ADC result
				ADC_RESOLUTION,
				ADC_IQN
			);
		else // for voltage sensor
			setup_adc_channel(&ctl_obj->adc_results[i],
				CTRL_T(CONTROLLER_V_ADC_GAIN), // ADC GAIN: from ADC result to p.u.
				CTRL_T(CONTROLLER_V_ADC_BIAS), // ADC BIAS: from A to ADC result
				ADC_RESOLUTION,
				ADC_IQN
			);
	}

	for (i = 0; i < 2; ++i)
	{
		init_pwm_channel(&ctl_obj->pwm[i]);
	}

	// Buck Phase
	setup_pwm_channel(&ctl_obj->pwm[0], 0, PWM_FULL_SCALE);

	// Boost Phase
	setup_pwm_channel(&ctl_obj->pwm[1], 0, PWM_FULL_SCALE);

	// per unit parameters
	ctl_obj->ctrl.I_pu2Current = CONTROLLER_I_SCALEFACTOR;
	ctl_obj->ctrl.U_pu2Voltage = CONTROLLER_V_SCALEFACTOR;

	// Initilize Controller

	// current inner controller
	ctl_init_divider(&ctl_obj->ctrl.div_current);

	ctl_init_slope_limit(&ctl_obj->ctrl.traj_current);

	max_slope = CTRL_T(CONTROLLER_I_SLOPE_MAX / CONTROLLER_I_BASE / ((float)ISR_FREQUENCY / (float)1e3));
	ctl_setup_slope_limit(&ctl_obj->ctrl.traj_current,
		-max_slope, max_slope,
		0, CTRL_T(CONTROLLER_I_MAX / CONTROLLER_I_BASE));

	ctl_init_pid(&ctl_obj->ctrl.pid_current);

	ctl_setup_pid(&ctl_obj->ctrl.pid_current,
		CTRL_T(25.0f), CTRL_T(0.1f), 0, // PID default parameters
		0, CTRL_T(1.5f));

	// voltage outer controller
	ctl_init_divider(&ctl_obj->ctrl.div_voltage);

	ctl_init_slope_limit(&ctl_obj->ctrl.traj_voltage);

	max_slope = CTRL_T(CONTROLLER_U_SLOPE_MAX / CONTROLLER_U_BASE / ((float)ISR_FREQUENCY / (float)1e3));
	ctl_setup_slope_limit(&ctl_obj->ctrl.traj_voltage,
		-max_slope, max_slope,
		0, CTRL_T(CONTROLLER_U_MAX / CONTROLLER_U_BASE));

	ctl_init_pid(&ctl_obj->ctrl.pid_voltage);

	ctl_setup_pid(&ctl_obj->ctrl.pid_voltage,
		CTRL_T(4.5f), CTRL_T(0.15f), 0, // PID default parameters
		0, CTRL_T(1.0f));

	// init monitor object
	init_buck_boost_monitor(&ctl_obj->monitor);

	// init protect module
	//for (i = 0; i < 5; ++i)
	//{
	//	ctl_init_bipolar_fusing(&ctl_obj->protect.fusing[i]);

	//	// define 0x01 is over current or over voltage protect
	//	ctl_bipolar_fusing_bind(&ctl_obj->protect.fusing[i], &ctl_obj->adc_results[i].value,0x01);
	//}

	//ctl_set_bipolar_fusing_bound(&ctl_obj->protect.fusing[I_in], 
	//	CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_I_IN / CONTROLLER_I_BASE));

	//ctl_set_bipolar_fusing_bound(&ctl_obj->protect.fusing[I_out],
	//	CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_I_OUT / CONTROLLER_I_BASE));

	//ctl_set_bipolar_fusing_bound(&ctl_obj->protect.fusing[I_L],
	//	CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_I_L / CONTROLLER_I_BASE));

	//ctl_set_bipolar_fusing_bound(&ctl_obj->protect.fusing[U_in],
	//	CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_U_IN / CONTROLLER_U_BASE));

	//ctl_set_bipolar_fusing_bound(&ctl_obj->protect.fusing[U_out],
	//	CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_U_OUT / CONTROLLER_U_BASE));

	// init controller nona object
	init_ctl_obj_nano_header((ctl_object_nano_t*)ctl_obj);

	// Setup controller nano object
	setup_ctl_obj_nano_header((ctl_object_nano_t*)ctl_obj, ISR_FREQUENCY);

}

void init_buck_boost_monitor(buck_boost_monitor_t* obj)
{
	int i;

	for (i = 0; i < 6; ++i)
		obj->adc_result_real[i] = 0.0f;

	obj->voltage_sf = CONTROLLER_V_SCALEFACTOR;
	obj->current_sf = CONTROLLER_I_SCALEFACTOR;

	obj->buck_duty = 0.0f;
	obj->boost_duty = 0.0f;

}

void buck_modulator(buck_boost_2ch_ctl_object_t* obj)
{
	// For Buck Controller D = M
	obj->pwm[buck_phase].raw = obj->ctrl.M_target;
	obj->pwm[boost_phase].raw = CTRL_T(1.0f);

	return;
}


// Here implement the core controller for buck boost controller
void ctl_core_stage_routine(ctl_object_nano_t* pctl_obj)
{
	int i = 0;
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;

	// input prepare
	for (i = 0; i < 6; ++i)
	{
		calc_adc_channel(&obj->adc_results[i]);
	}

	// Controller body

	// Voltage outer loop
	if (ctl_divider_run(&obj->ctrl.div_voltage))
	{
		ctl_slope_limit(&obj->ctrl.traj_voltage, obj->ctrl.V_target);

		ctl_pid_par(&obj->ctrl.pid_voltage,
			obj->ctrl.traj_voltage.out - obj->adc_results[U_out].value);

		obj->ctrl.I_target = obj->ctrl.pid_voltage.out;
	}

	// Current inner loop
	if (ctl_divider_run(&obj->ctrl.div_current))
	{
		ctl_slope_limit(&obj->ctrl.traj_current, obj->ctrl.I_target);

		ctl_pid_par(&obj->ctrl.pid_current,
			obj->ctrl.traj_current.out - obj->adc_results[I_out].value);

		obj->ctrl.M_target = obj->ctrl.pid_current.out;
	}

	// modulator
	buck_modulator(obj);

	// output prepare
	calc_pwm_channel(&obj->pwm[0]);
	calc_inv_pwm_channel(&obj->pwm[1]);

	return;
}


void controller_monitor_routine(ctl_object_nano_t* pctl_obj)
{
	int i = 0;
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;

	for (i = 0; i < 6; ++i)
	{
		if (i < 3) // for current sensor
			obj->monitor.adc_result_real[i] = obj->monitor.current_sf * ctrl2float(obj->adc_results[i].value);
		else // for voltage sensor
			obj->monitor.adc_result_real[i] = obj->monitor.voltage_sf * ctrl2float(obj->adc_results[i].value);
	}

	obj->monitor.buck_duty = obj->pwm[0].value;
	obj->monitor.boost_duty = obj->pwm[1].value;

	return;
}

void controller_security_routine(ctl_object_nano_t* pctl_obj)
{
	int i = 0;
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;

	for (i = 0; i < 5; ++i)
		ctl_check_fusing(pctl_obj, &obj->protect.fusing[i]);

	return;
}

