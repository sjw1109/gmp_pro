
//////////////////////////////////////////////////////////////////////////
// The following parameters are for used for p.u. base value
// 
// Constrains of motor rated parameters
// These parameters will be used as pu base value
// 
// Motor Rated parameters:  

// 5010-360KV Motor

// Motor rated poles pairs parameters
// unit, 1
#define MOTOR_RATED_POLE_PAIRS ((7))


	// Motor rated parameters
	// unit, rpm
#define MOTOR_RATED_SPEED_RPM ((1000.0))


	// Motor rated parameters
	// unit, SI, W
#define MOTOR_RATED_OUTPUT_POWER ((300.0))


	// Motor rated parameters
	// unit, SI, V rms (Line voltage)
#define MOTOR_RATED_VOLTAGE   ((24.0))


	// Motor rated parameters
	// unit, SI, A rms (Line current)
#define MOTOR_RATED_CURRENT   ((20.0))

	// Motor rated parameters
	// unit, SI, Hz
#define MOTOR_RATED_FREQUENCY ((700.0))

	// Motor rated parameters
	// unit, SI, N.m
#define MOTOR_RATED_TORQUE    ((0.8))

	// Motor Power factor
	// unit, %
#define MOTOR_RATED_POWER_FACTOR ((0.8))

	// Motor rated efficiency
#define MOTOR_RATED_EFFICIENCY   ((0.9))


	//////////////////////////////////////////////////////////////////////////
	// The following parameters are for controller auto tuning.

	// Motor Design parameters, stator parameters
	// unit, SI Ohm
#define MOTOR_DSN_STATOR_RES  ((0.12))

	// Motor Design parameters, Armature inductance Ld
	// unit, SI H
#define MOTOR_DSN_LD          ((5e-5))

	// Motor Design parameters, Armature inductance Lq
	// unit, SI H
#define MOTOR_DSN_LQ          ((5e-5))

	// Motor Design parameters, flux linkage
	// unit, Wb, SI V.s
#define MOTOR_DSN_FLUX_WB      ((0.22))

	// Motor Design parameters, inertia
	// unit, kg.m/s2
#define MOTOR_DSN_INERTIA      ((0.05))

	// Motor Design parameters, damping
	// unit, N.m.s
#define MOTOR_DSN_DAMP			((0.001))

//////////////////////////////////////////////////////////////////////////
// Motor Driver parameters.
	// These parameters will be used to generate protect parameters


	// Motor Driver parameters, DC bus voltage
	// unit, SI V
#define MOTOR_DRIVER_RATED_DC_VOLTAGE ((24.0))

	// Motor Driver parameters, Current pu base
	// unit, SI A
#define MOTOR_DRIVER_RATED_DC_CURRNET ((20.0))


	// Motor Driver parameters, maximum current
	// unit, SI A, transient value
#define MOTOR_DRIVER_MAX_BRIDGE_CURRENT ((10.0))

	// Motor Driver parameters, maximum current
	// unit, SI A, transient value
#define MOTOR_DRIVER_MAX_DC_CURRENT ((10.0))

	// Motor Driver parameters, PWM frequency
	// unit, SI Hz
#define MOTOR_DRIVER_PWM_FREQUENCY  ((10e3))

	// Motor Driver parameters, Control law frequency
	// unit, SI Hz
#define MOTOR_DRIVER_CONTROL_FREQUENCY ((10e3))

	// Motor Driver parameters, PWM generator half period
#define MOTOR_DRIVER_PWM_HALF_CYCLE ((8500))

	// Current controller loop bandwidth
	// unit, Hz
#define MOTOR_DRIVER_CURRENT_BW ((200))

	// Speed Controller loop bandwidth
	// unit, Hz
#define MOTOR_DRIVER_SPEED_BW ((50))

	// Speed controller division
	// unit, 1 (times)
#define MOTOR_DRIVER_SPEED_DIV ((10))

	// Speed calculator division
	// unit 1 (times)
#define MOTOR_DRIVER_SPEED_CALC_DIV ((10))

	// ADC performance resolution
#define MOTOR_DRIVER_ADC_RESOLUTION ((12))

	// ADC Voltage Full scale Range
#define MOTOR_DRIVER_ADC_VOLTAGE_FULL_SCALE ((80.0))

	// ADC Voltage Bias p.u.
#define MOTOR_DIRVER_ADC_VOLTAGE_BIAS_PU    (0.0)

	// ADC Current Full scale Range
#define MOTOR_DRIVER_ADC_CURRENT_FULL_SCALE ((33.0))

	// ADC Current Bias p.u.
#define MOTOR_DIRVER_ADC_CURRENT_BIAS_PU    (0.5)

	// ADC Voltage Full scale Range
#define MOTOR_DRIVER_ADC_VOLTAGE_DC_FULL_SCALE ((80.0))

	// ADC Voltage Bias p.u.
#define MOTOR_DIRVER_ADC_VOLTAGE_DC_BIAS_PU    (0.0)

	// ADC Current Full scale Range
#define MOTOR_DRIVER_ADC_CURRENT_DC_FULL_SCALE ((33.0))

	// ADC Current Bias p.u.
#define MOTOR_DIRVER_ADC_CURRENT_DC_BIAS_PU    (0.0)


	// Encoder Base
#define MOTOR_DRIVER_ENCODER_BASE              ((1<<15))

	// rotor acceleration limit, unit rpm/s
#define MOTOR_DRIVER_ACCLERATION               ((MOTOR_RATED_SPEED_RPM))

	// jerk limit, p.u./ms
#define MOTOR_DRIVER_JERK                      ((0.02))

