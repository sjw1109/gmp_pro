
#include <encoder.h>

#ifndef _FILE_PMSM_H_
#define _FILE_PMSM_H_


typedef enum _tag_gmp_phase_enum_t
{
	PHASE_U = 0,
	PHASE_V = 1,
	PHASE_W = 2
}gmp_phase_enum_t;

typedef enum _tag_gmp_dq_enum_t
{
	phase_d = 0,
	phase_q = 1
}gmp_dq_enum_t;

typedef enum _tag_gmp_motor_closeloop_t
{
	// Motor will not get output
	MOTOR_DISABLED = 0,
	// User may all other close loop level here.

	// Motor will running in open-loop mode
	OPENLOOP = 16,

	// Motor will running in current-loop mode
	CURRENTLOOP = 64,

	// Motor will running in speed-loop mode
	SPEEDLOOP = 128,

	// Motor will running in position-loop mode
	POSITIONLOOP = 256
}gmp_motor_closeloop_t;

// enum type of motor working duty
typedef enum _tag_gmp_motor_duty_t 
{
	GMP_MOTOR_DUTY_DISABLE = 0, // Disable this param
	GMP_MOTOR_DUTY_S1 = 1,		// Continuous duty
	GMP_MOTOR_DUTY_S2 = 2,		// Short-time duty
	GMP_MOTOR_DUTY_S3 = 3,		// Intermittent periodic duty
	GMP_MOTOR_DUTY_S4 = 4,		// Intermittent periodic duty with starting
	GMP_MOTOR_DUTY_S5 = 5,		// Intermittent periodic duty with electric braking
	GMP_MOTOR_DUTY_S6 = 6,		// Continuous operation with intermittent load
	GMP_MOTOR_DUTY_S7 = 7,		// Continuous operation with electric braking
	GMP_MOTOR_DUTY_S8 = 8,		// Continuous operation with periodic changes in load and speed
}gmp_motor_duty_t;

// general type (_gt) of PMSM inverter intrinsic variables
typedef struct _tag_pmsm_inv_interinsic_t
{
	gmp_ctrl_param_t pwm_freq_kHz;		// PWM frequency, (kHz)
	gmp_ctrl_param_t Udc_V;				// DC bus voltage, SI (V)
}pmsm_inv_intrin_gt;

// general type (_gt) of PMSM intrinsic variables
typedef struct _tag_pmsm_intrinsic_t
{
	uint16_t pairs;						// Number of motor poles

	gmp_ctrl_param_t ctrl_freq_kHz;		// controller frequency, (kHz)
	gmp_ctrl_param_t Pn_kW;				// rated power, SI (kW)
	gmp_ctrl_param_t Un_V;				// rated voltage, SI (V)
	gmp_ctrl_param_t In_A;				// rated current, SI (A)
	gmp_ctrl_param_t fn_Hz;				// rated frequency, SI (Hz)

	gmp_ctrl_param_t Rs;				// SI (Ohm)
	gmp_ctrl_param_t Ls;				// SI (mH, Henry)
	gmp_ctrl_param_t Rd;				// SI (Ohm)
	gmp_ctrl_param_t Ld;				// SI (mH, Henry)
	gmp_ctrl_param_t Rq;				// SI (Ohm)
	gmp_ctrl_param_t Lq;				// SI (mH, Henry)
	gmp_ctrl_param_t flux;				// SI (Wb)
	gmp_ctrl_param_t inertia;			// SI (kg m2), machinery energy capacity
	gmp_ctrl_param_t damp;				// SI (N s/m), damp per velocity
	gmp_ctrl_param_t friction;			// SI (N m), bias of damp

	gmp_motor_duty_t duty_cycles;		// duty type of motor
}pmsm_intrin_gt;

// general type for motor ADC input
typedef struct _tag_motor_adc_atleast_t
{
	// ADC result after transform variables
	gmp_math_t I_abc[3];				// In p.u.
	gmp_math_t I_bus;					// In p.u.

	// auxiliary scale factor
	gmp_math_t I_adc2abc_sf[3];			// transfer ADC current into Iabc p.u. value
	gmp_math_t I_adc2bus_sf;			// transfer ADC current into Ibus p.u. value
	gmp_math_t I_adc2abc_bias[3];		// transfer ADC current into AC value
	gmp_math_t I_adc2bus_bias;			// transfer ADC current into bus value

	// coordinate transform result
	gmp_math_t Idq[2];					// In p.u.

}motor_adc_atleast_gt;

// general type for motor udc input
typedef struct _tag_motor_adc_udc_t
{
	motor_adc_atleast_gt current;
	gmp_math_t V_bus;					// Vn p.u.
	gmp_math_t V_adc2bus_sf;			// transfer ADC voltage into Ubus p.u. value
}tag_motor_adc_udc_gt;

// general type for motor ADC input
typedef struct _tag_motor_adc_t
{
	// ADC result after transform variables
	gmp_math_t I_abc[3];				// In p.u.
	gmp_math_t I_bus;					// In p.u.

	// auxiliary scale factor
	gmp_math_t I_adc2abc_sf[3];			// transfer ADC current into Iabc p.u. value
	gmp_math_t I_adc2bus_sf;			// transfer ADC current into Ibus p.u. value
	gmp_math_t I_adc2abc_bias[3];		// transfer ADC current into AC value
	gmp_math_t I_adc2bus_bias;			// transfer ADC current into bus value

	gmp_math_t V_abc[3];				// Vn p.u.
	gmp_math_t V_bus;					// Vn p.u.

	gmp_math_t V_adc2abc_sf[3];			// transfer ADC voltage into Uabc p.u. value
	gmp_math_t V_adc2bus_sf;			// transfer ADC voltage into Ubus p.u. value

	// coordinate transform result
	gmp_math_t Idq[2];					// In p.u.
	gmp_math_t Vdq[2];					// Vn p.u.

}motor_adc_gt;

// general type for motor controller interface
typedef struct _tag_motor_ctrl_interface_t
{
	// intermediate variables

	// Speed reference, (krpm, Revolution Per Minute)
	gmp_math_t speed_set_krpm;
	// current set, p.u.
	// in speed mode and position mode, 
	// this value shows the maximum output of the current
	gmp_math_t Idq_set[2];
	// voltage set, p.u.
	// in current mode and speed mode and position mode, 
	// this value is means that the maximum output of Voltage.
	gmp_math_t Vdq_set[2];
}motor_ctrl_if_gt;

// general type for servo motor controller interface
typedef struct _tag_servo_ctrl_interface_t
{
	// intermediate variables
	
	// revolution of turns
	int32_t    rot_set;				
	// marking per circle
	gmp_math_t ang_set;	
	// Speed reference, (krpm, Revolution Per Minute)
	// in position mode, this value is to show 
	// the velocity expected the time reaching the target.	
	gmp_math_t speed_set_krpm;	
	// current set, p.u.
	// in speed mode and position mode, 
	// this value shows the maximum output of the current
	gmp_math_t Idq_set[2];			
	// voltage set, p.u.
	// in current mode and speed mode and position mode, 
	// this value is means that the maximum output of Voltage.
	gmp_math_t Vdq_set[2];				
}servo_ctrl_if_gt;

// general type for motor extension observer
typedef struct _tag_pmsm_monitor_t
{
	// torque
	gmp_math_t torque_Nm;				// Output Torque, unit Nm

	// stator current
	gmp_math_t Is;						// stator current, unit In p.u.

	// magnetizing current
	gmp_math_t Im;						// magnetizing current, unit In p.u.

	// zero-sequence current
	gmp_math_t I0;						// zero-sequence current, unit In p.u.

	// temperature
	int16_t motor_temp_centideg;		// motor temperature sensor, unit 0.01 Celsius degree
	int16_t ctrl_temp_centideg;			// inverter temperature sensor, unit 0.01 Celsius degree

	// flux
	gmp_math_t flux;					// flux observed, unit pmsm_intrinsic_gt::flux p.u.

	// Continuous working time
	uint32_t cw_cnt;					// describe the motor continuous working time, unit Sec
}pmsm_monitor_gt;

typedef struct _tag_pmsm_plant
{
	// input variables
	motor_adc_gt* adc;					// current and voltage analog sensor
	gmp_enc_t  *enc;					// encoder for the motor

	// output variables
	gmp_math_t T_abc[3];

	// intermediate variables
	pmsm_ctrl_if_gt* target;			// input control target

	// intrinsic variables
	pmsm_inv_intrin_gt* inv;
	pmsm_intrin_gt* motor;

	// controllers



	// properties variables
	uint16_t close_loop_level;

	// state machine


	// protection & limitation


	// monitor port
	pmsm_monitor_gt* monitor;

}pmsm_plant_t;

inline void_motor_std_ctrl(
	pmsm_plant_t pmsm,
	pid_regular_t spd,
	pid_regular_t cur
)
{

}

#endif // _FILE_PMSM_H_
