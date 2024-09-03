


#ifndef _FILE_CTL_NANO_H_
#define _FILE_CTL_NANO_H_

#ifdef __cplusplus
extern "C"
{
#endif// __cplusplus


	// Controller basic State machine
	typedef enum _tag_ctl_nano_state_machine
	{
		// State machine: system is pending
		// in this stage, all the controller output is strictly prohibited,
		// and the controller is bypassed.
		// User should enable the system then the output may happen.
		CTL_SM_PENDING = 0,

		// State machine: system is initializing
		// in this stage, PWM output with 0 output is permitted.
		// ADC device is ready, and PWM device is ready.
		// All the control law objects will initialize.
	//	CTL_SM_INIT = 1,

		// State machine: system is calibrating
		// in this stage, ADC zero point should be calibrated, 
		// hardware security check is done in this stage
		CTL_SM_CALIBRATE = 2,

		// State machine: Controller is ready to launch
		// The state machine is ready for user to launch
		CTL_SM_READY = 3,

		// State machine: Controller is preparing
		// Auto tuning, auto measurement, auto identify is done here.
		// a set of switches will determine which routine may be invoke
		CTL_SM_RUNUP = 4,

		// State machine: Controller is on-line
		CTL_SM_ONLINE = 5,

		// State machine: Controller is in fault.
		CTL_SM_FAULT = 6
	}ctl_nano_state_machine;

	// endorse for CTL
#define GMP_CTL_ENDORSE ((0x4743544C))

// controller basic objects
	typedef struct _tag_ctl_object_nano
	{
		// This number must equals to GMP_CTL_ENDORSE, that is 0x47_43_54_4C (GCTL)
		// to ensure the controller is initialized correctly.
		uint32_t security_endorse;

		// ISR tick
		uint_least32_t isr_tick;

		// State Machine tick
		uint_least32_t mainloop_tick;

		// Calculating CPU usage for Control law
		uint_least32_t control_law_CPU_usage_tick;

		// Calculating CPU usage for Control Main loop
		uint_least32_t mainloop_CPU_usage_tick;

		// state machine of the controller
		ctl_nano_state_machine state_machine;

		// controller error code
		gmp_stat_t error_code;

		// Control Period Counter
//		uint32_t period_count;

		// Controller Frequency, Unit Hz
		uint32_t ctrl_freq;

		// Controller Monitor divider
		ctl_divider_t div_monitor;

		// Switch of calibrate stage
		// Default is On
		fast_gt switch_calibrate_stage;

		// Switch of calibrate stage
		// Default is Off
		fast_gt switch_runup_stage;

		// Switch of security module
		fast_gt switch_security_routine;


	}ctl_object_nano_t;


	//////////////////////////////////////////////////////////////////////////
	// The following functions should implemented by user

	// deal with all the adc and other input messages
	void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj);

	// implement all the controller routine
	void ctl_core_stage_routine(ctl_object_nano_t* pctl_obj);

	// output all the PWM and other digital or analog signal
	void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj);

	// request other information via peripheral, for instance SPI.
	void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj);

	// enable PWM output
	void controller_output_enable(ctl_object_nano_t* pctl_obj);

	// disable PWM output
	void controller_output_disable(ctl_object_nano_t* pctl_obj);

	// controller monitor routine
	void controller_monitor_routine(ctl_object_nano_t* pctl_obj);

	//////////////////////////////////////////////////////////////////////////
	// The following function should be called by user on time.

	// In Main ISR
	void controller_dispatch(ctl_object_nano_t* pctl_obj);

	// In Main Loop
	void controller_state_dispatch(ctl_object_nano_t* pctl_obj);

	// In Initialize function
	uint32_t controller_inspection(ctl_object_nano_t* pctl_obj);

	//////////////////////////////////////////////////////////////////////////
	// Function Prototypes

	// You should call this function to initialize the object
	void ctl_init_nano_header(ctl_object_nano_t* ctl_obj);

	// Then you may call this function to set the details of the objects
	void ctl_setup_nano_header(ctl_object_nano_t* ctl_obj,
		uint32_t ctrl_freq    // the frequency of the control law, unit Hz
	);

	//////////////////////////////////////////////////////////////////////////
	// Utilities

	// DANGEROUS:
	// This function may damage your controller.
	// Call it iff you is an expert.
	// 
	void ctl_force_nona_header_online(ctl_object_nano_t* ctl_obj);
	
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_NANO_H_
