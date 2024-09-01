
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

//////////////////////////////////////////////////////////////////////////
//  Init Setion
// This function may initialize a buck_boost_2ch object
void init_buck_boost_2ch_ctl(buck_boost_2ch_ctl_object_t* ctl_obj)
{
	int i;
	ctrl_gt max_slope;

	// init input & output parameters
	for (i = 0; i < 6; ++i)
		init_adc_channel(&ctl_obj->adc_results[i]);

	for (i = 0; i < 6; ++i)
	{
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
				CTRL_T(CONTROLLER_V_ADC_BIAS), // ADC BIAS: from V to ADC result
				ADC_RESOLUTION,
				ADC_IQN
			);
	}

	for (i = 0; i < 2; ++i)
	{
		init_pwm_channel(&ctl_obj->pwm[i]);
	}

	// Buck Phase
	setup_pwm_channel(&ctl_obj->pwm[0], 0, PWM_FULL_SCALE - 1);

	// Boost Phase
	setup_pwm_channel(&ctl_obj->pwm[1], 0, PWM_FULL_SCALE - 1);

	// per unit parameters
	ctl_obj->ctrl.I_pu2Current = CONTROLLER_I_SCALEFACTOR;
	ctl_obj->ctrl.U_pu2Voltage = CONTROLLER_V_SCALEFACTOR;

	// Initilize Controller

	ctl_obj->ctrl.enable_current_controller = 1;
	ctl_obj->ctrl.enable_voltage_controller = 1;

	// Default: current regular and voltage regular is switched on
	//ctl_obj->ctrl.enable_current_controller = 0;
	//ctl_obj->ctrl.enable_voltage_controller = 0;



	// current inner controller
	ctl_init_divider(&ctl_obj->ctrl.div_current);

	ctl_init_slope_limit(&ctl_obj->ctrl.traj_current);

	max_slope = CTRL_T(CONTROLLER_I_SLOPE_MAX / CONTROLLER_I_BASE / ((float)ISR_FREQUENCY / (float)1e3));
	ctl_setup_slope_limit(&ctl_obj->ctrl.traj_current,
		-max_slope, max_slope,
		0, CTRL_T(CONTROLLER_I_MAX / CONTROLLER_I_BASE));

	ctl_init_pid(&ctl_obj->ctrl.pid_current);

	ctl_setup_pid(&ctl_obj->ctrl.pid_current,
		CTRL_T(25.0f / 100), CTRL_T(0.1f/100), 0, // PID default parameters
		0, CTRL_T(0.90f));

	// voltage outer controller
	ctl_init_divider(&ctl_obj->ctrl.div_voltage);

	// f_voltage = f_current / 2
	ctl_obj->ctrl.div_voltage.target = 2;

	ctl_init_slope_limit(&ctl_obj->ctrl.traj_voltage);

	max_slope = CTRL_T(CONTROLLER_U_SLOPE_MAX / CONTROLLER_U_BASE / ((float)ISR_FREQUENCY / (float)1e3));
	ctl_setup_slope_limit(&ctl_obj->ctrl.traj_voltage,
		-max_slope, max_slope,
		-CTRL_T(0.5f * CONTROLLER_I_MAX / CONTROLLER_I_BASE), CTRL_T(CONTROLLER_U_MAX / CONTROLLER_U_BASE));

	ctl_init_pid(&ctl_obj->ctrl.pid_voltage);

	ctl_setup_pid(&ctl_obj->ctrl.pid_voltage,
		CTRL_T(4.5f/100), CTRL_T(0.15f/10), 0, // PID default parameters
		-CTRL_T(0.5f), CTRL_T(1.0f));

	// disable the current and voltage controller 
	// user may use M_taget to control the output
	// stop the controller by divider bypass
	ctl_obj->ctrl.div_voltage.flag_bypass = 1;
	ctl_obj->ctrl.div_current.flag_bypass = 1;

	// init monitor object
	init_buck_boost_monitor(&ctl_obj->monitor);

	// init protect module
	init_fusing_module(ctl_obj);

	// init calibrate object
	init_calibrate_module(&ctl_obj->calibrator);
	ctl_obj->calibrae_progress = 0;

	// init controller nona object
	init_ctl_obj_nano_header((ctl_object_nano_t*)ctl_obj);

	// Setup controller nano object
	setup_ctl_obj_nano_header((ctl_object_nano_t*)ctl_obj, ISR_FREQUENCY);


	ctl_obj->base.switch_security_routine = 0;

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


	obj->target_voltage = 0.0f;
	obj->target_current = 0.0f;

}

void init_fusing_module(buck_boost_2ch_ctl_object_t* obj)
{
	int i;

	for (i = 0; i < 5; ++i)
	{
		ctl_init_bipolar_fusing(&obj->protect.fusing[i]);

		// define 0x01 is over current or over voltage protect
		ctl_bipolar_fusing_bind(&obj->protect.fusing[i], &obj->adc_results[i].value, i+1);
	}

	ctl_set_bipolar_fusing_bound(&obj->protect.fusing[I_in],
		CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_I_IN / CONTROLLER_I_BASE));

	ctl_set_bipolar_fusing_bound(&obj->protect.fusing[I_out],
		CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_I_OUT / CONTROLLER_I_BASE));

	ctl_set_bipolar_fusing_bound(&obj->protect.fusing[I_L],
		CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_I_L / CONTROLLER_I_BASE));

	ctl_set_bipolar_fusing_bound(&obj->protect.fusing[U_in],
		CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_U_IN / CONTROLLER_U_BASE));

	ctl_set_bipolar_fusing_bound(&obj->protect.fusing[U_out],
		CTRL_T(-0.2f), CTRL_T(CONTROLLER_FUSING_U_OUT / CONTROLLER_U_BASE));
}

void init_calibrate_module(adc_bias_calibrator_t* calibrator)
{
	filter_IIR2_setup_t filter_setup;

	filter_setup.fc = 25;
	filter_setup.filter_type = FILTER_IIR2_TYPE_LOWPASS;
	filter_setup.fs = ISR_FREQUENCY;
	filter_setup.gain = 1.0f;
	filter_setup.q = 1.0f;

	init_adc_bias_calibrator(calibrator);

	setup_adc_bias_calibrator(calibrator, &filter_setup);
}

void clear_controller(buck_boost_2ch_ctl_object_t* obj)
{
	// stop the controller by bypass
	obj->ctrl.div_voltage.flag_bypass = 1;
	obj->ctrl.div_current.flag_bypass = 1;

	// clear current controller
	ctl_clear_divider(&obj->ctrl.div_current);
	ctl_clear_limit_slope(&obj->ctrl.traj_current);
	ctl_clear_pid(&obj->ctrl.pid_current);

	// clear voltage controller
	ctl_clear_divider(&obj->ctrl.div_voltage);
	ctl_clear_limit_slope(&obj->ctrl.traj_voltage);
	ctl_clear_pid(&obj->ctrl.pid_voltage);

	// clear output
	obj->ctrl.M_target = 0;

	// restart the controller
	obj->ctrl.div_voltage.flag_bypass = !obj->ctrl.enable_current_controller;
	obj->ctrl.div_current.flag_bypass = !obj->ctrl.enable_voltage_controller;

	return;
}


//////////////////////////////////////////////////////////////////////////
// Main ISR section

void buck_modulator(buck_boost_2ch_ctl_object_t* obj)
{
	// For Buck Controller D = M
	obj->pwm[buck_phase].raw = obj->ctrl.M_target;
	obj->pwm[boost_phase].raw = CTRL_T(0.99f);

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
			obj->ctrl.traj_current.out + obj->adc_results[I_out].value);

		obj->ctrl.M_target = obj->ctrl.pid_current.out;
	}

	// modulator
	buck_modulator(obj);

	// output prepare
	calc_pwm_channel(&obj->pwm[buck_phase]);
	calc_inv_pwm_channel(&obj->pwm[boost_phase]);

	return;
}

//////////////////////////////////////////////////////////////////////////
// State Machine Section

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

	obj->monitor.buck_duty = (float)obj->pwm[0].value / obj->pwm[0].full_scale;
	obj->monitor.boost_duty = (float)obj->pwm[1].value / obj->pwm[1].full_scale;

	obj->monitor.target_voltage = obj->monitor.voltage_sf * ctrl2float(obj->ctrl.V_target);
	obj->monitor.target_current = obj->monitor.current_sf * ctrl2float(obj->ctrl.I_target);

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


// CTL_SM_CALIBRATE
fast_gt ctl_nano_sm_calibrate_routine(ctl_object_nano_t* pctl_obj)
{
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;
	ctrl_gt adc_data;


	if (obj->calibrae_progress < 6)
	{
		adc_data = obj->adc_results[obj->calibrae_progress].raw
			<< (GLOBAL_Q - obj->adc_results[obj->calibrae_progress].iqn);

		if (run_adc_bias_calibrator(&obj->calibrator, obj->base.isr_tick, adc_data))
		{
			// register the calibrate progress
			obj->adc_results[obj->calibrae_progress].bias = obj->calibrator.bias_output;

			// call next calibrate channel
			obj->calibrae_progress += 1;

			// There're still items to be calibrated
			if (obj->calibrae_progress < 6)
				restart_adc_bias_calibrator(&obj->calibrator);
		}

		return 0;
	}
	// The whole calibrate has completed
	else
	{
		return 1;
	}

}


fast_gt ctl_nano_sm_runup_routine(ctl_object_nano_t* pctl_obj)
{
	// clear controller
	clear_controller((buck_boost_2ch_ctl_object_t*)pctl_obj);

	// enable output
	return 1;
}

void ctl_nano_sm_online_routine(ctl_object_nano_t* pctl_obj)
{
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;

	obj->ctrl.div_voltage.flag_bypass = !obj->ctrl.enable_current_controller;
	obj->ctrl.div_current.flag_bypass = !obj->ctrl.enable_voltage_controller;

	// current controller is disabled
	if (!obj->ctrl.enable_current_controller)
	{
		// clear current controller
		ctl_clear_divider(&obj->ctrl.div_current);
		ctl_clear_limit_slope(&obj->ctrl.traj_current);
		ctl_clear_pid(&obj->ctrl.pid_current);
	}

	if (!obj->ctrl.enable_voltage_controller)
	{
		// clear voltage controller
		ctl_clear_divider(&obj->ctrl.div_voltage);
		ctl_clear_limit_slope(&obj->ctrl.traj_voltage);
		ctl_clear_pid(&obj->ctrl.pid_voltage);
	}
}


//////////////////////////////////////////////////////////////////////////
// Service function 

