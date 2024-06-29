
// This is a Controller Template Library nano extension for buck boost circuit






#ifndef _FILE_BUCK_BOOST_2CH_CTL_H_
#define _FILE_BUCK_BOOST_2CH_CTL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//////////////////////////////////////////////////////////////////////////
	// Global macros

	// Main ISR for controller frequency, unit Hz
#define ISR_FREQUENCY (20e3)


	// PMW 100% output 
#define PWM_FULL_SCALE (13600u)

// Controller I base value, Unit A
// Physical significance: Current(A) -> Current(p.u.)
#define CONTROLLER_I_BASE (10.0f)

// Controller U base value, Unit V
// Physical significance: Voltage(A) ->Voltage(p.u.)
#define CONTROLLER_U_BASE (48.0f)

// Absolute Maximum I, unit A
#define CONTROLLER_I_ABSOLUTE_MAX (15.0f)

// Absolute Maximum U, Unit V
#define CONTROLLER_U_ABSOLUTE_MAX (60.0f)

// Current Slope Limit, Unit A/ms
#define CONTROLLER_I_SLOPE_MAX (1.0f)

// Voltage Slope Limit, Unit V/ms
#define CONTROLLER_U_SLOPE_MAX (2.0f)

// Define Default ADC resolution
#define ADC_RESOLUTION (12)

// Define Default ADC result significant number of digits
#define ADC_IQN (12)

// Controller parameter: maximum voltage (reference voltage) of ADC, unit V
#define CONTROLLER_ADC_REF_VOLTAGE (3.3f)

//////////////////////////////////////////////////////////////////////////
// Controller Current Sensor Default Configuration

// Controller parameter: Current Sensor ratio, unit A/V, 
// Physical significance: ADC value(V) -> real Current(A)
#define CONTROLLER_CS_RATIO (1.0f)

// Controller parameter: Current Bias, unit V
// Physical significance: I = 0 => ADC pin Voltage(V)
#define CONTROLLER_CS_BIAS_VOLTAGE (1.25f)

// Controller I full scale value, Unit A
// Physical significance: Total Current(A) range of the ADC
#define CONTROLLER_I_FULLSCALE (CONTROLLER_ADC_REF_VOLTAGE * CONTROLLER_CS_RATIO)

// Controller I bias value, Unit A
// Physical significance: I = 0 => ADC readout Current(A), if without bias
#define CONTROLER_I_BIAS (CONTROLLER_CS_BIAS_VOLTAGE * CONTROLLER_CS_RATIO)

// Attention: Controller I ADC gain
// ADC GAIN: from ADC result to p.u.
// Physical significance: ADC sample result -> Current(p.u.)
// This variable will be used to initialize the ADC object.
#define CONTROLLER_I_ADC_GAIN (CONTROLLER_CS_RATIO / CONTROLLER_I_BASE)

// Attention: Controller I ADC bias, unit: ADC ticks, p.u.
// Here are two type of definition:
// This variable will be used to initialize the ADC object.
#define CONTROLLER_I_ADC_BIAS (CONTROLLER_CS_BIAS_VOLTAGE / CONTROLLER_ADC_REF_VOLTAGE)
//#define CONTROLLER_I_ADC_BIAS (CONTROLER_I_BIAS / CONTROLLER_CS_RATIO / CONTROLLER_ADC_REF_VOLTAGE)

// Attention: Monitor I result scale factor
// Physical significance: Current(p.u.) -> real Current(A)
// This variable will be used to initialize the ADC object.
#define CONTROLLER_I_SCALEFACTOR (CONTROLLER_CS_RATIO * CONTROLLER_ADC_REF_VOLTAGE)

//////////////////////////////////////////////////////////////////////////
// Controller Voltage Sensor Default Configuration

// Controller parameter: Voltage Sensor ratio, unit V(real)/V(adc)
// Physical significance: ADC value(V) -> real Voltage(V)
#define CONTROLLER_VS_RATIO (1.0f)

// Controller parameter: Voltage Bias, Unit V
// Physical significance: V = 0 => ADC pin Voltage(V)
#define CONTROLLER_VS_BIAS_VOLTAGE (1.2f)

// Controller U full scale value, Unit V
// Physical significance: Total Voltage(V) range of the ADC
#define CONTROLLER_U_FULLSCALE (CONTROLLER_ADC_REF_VOLTAGE * CONTROLLER_VS_RATIO)

// Controller U bias value, Unit V
// Physical significance: I = 0 => ADC readout Voltage(V), if without bias
#define CONTROLLER_U_BIAS (CONTROLLER_VS_BIAS_VOLTAGE * CONTROLLER_VS_RATIO)

// Attention: Controller V ADC gain
// ADC GAIN: from ADC result to p.u.
// Physical significance: ADC sample result -> Voltage(p.u.)
// This variable will be used to initialize the ADC object.
#define CONTROLLER_V_ADC_GAIN (CONTROLLER_VS_RATIO / CONTROLLER_U_BASE)

// Attention: Controller V ADC bias
// Here are two type of definition:
// This variable will be used to initialize the ADC object.
#define CONTROLLER_V_ADC_BIAS (CONTROLLER_VS_BIAS_VOLTAGE / CONTROLLER_ADC_REF_VOLTAGE)
//#define CONTROLLER_V_ADC_BIAS (CONTROLER_U_BIAS / CONTROLLER_VS_RATIO / CONTROLLER_ADC_REF_VOLTAGE)

// Attention: Monitor V result scale factor
// Physical significance: Voltage(p.u.) -> real Voltage(V)
// This variable will be used to initialize the ADC object.
#define CONTROLLER_V_SCALEFACTOR (CONTROLLER_VS_RATIO * CONTROLLER_ADC_REF_VOLTAGE)

	enum adc_results_enum
	{
		I_in = 0,
		I_out = 1,
		I_L = 2,
		U_in = 3,
		U_out = 4,
		U_L = 5
	};

	enum pwm_phase_enum
	{
		buck_phase = 0,
		boost_phase = 1
	};

	typedef struct _tag_buck_boost_controller
	{
		// current controller 
		divider_t div_current;
		slope_lim_t traj_current;
		pid_regular_t pid_current;

		// voltage controller
		divider_t div_voltage;
		slope_lim_t traj_voltage;
		pid_regular_t pid_voltage;

		// Controller parameters
		ctrl_gt V_target;
		ctrl_gt I_target;
		ctrl_gt M_target;

		// per unit parameters
		parameter_gt I_pu2Current;
		parameter_gt U_pu2Voltage;



	}buck_boost_controller_t;


	typedef struct _tag_buck_boost_monitor_t
	{
		// current and voltage parameters
		float adc_result_real[6];

		// current variables scale factor
		float current_sf;

		// voltage variables scale factor
		float voltage_sf;

		// buck duty
		float buck_duty;

		// boost duty
		float boost_duty;

	}buck_boost_monitor_t;

	typedef struct _tag_buck_boost_2ch_ctl_object
	{
		ctl_object_nano_t base;

		// input ADC channels
		adc_channel_t adc_results[6];

		// output PWM channels
		pwm_channel_t pwm[2];

		// Buck boost controller
		buck_boost_controller_t ctrl;

		// Monitor Objects
		buck_boost_monitor_t monitor;

	}buck_boost_2ch_ctl_object_t;


	//// Build Level
	//#define BUILD_LEVEL 1

	void init_buck_boost_monitor(buck_boost_monitor_t* obj);

	void init_buck_boost_2ch_ctl(buck_boost_2ch_ctl_object_t* ctl_obj);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BUCK_BOOST_2CH_CTL_H_
