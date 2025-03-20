/**
 * @file motor_driver_consultant.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */


#ifndef _FILE_MOTOR_DRIVER_CONSULTANT_H_
#define _FILE_MOTOR_DRIVER_CONSULTANT_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// Motor Driver parameters.
	// These parameters will be used to generate protect parameters


	// Motor Driver parameters, DC bus voltage
	// unit, SI V
#ifndef MOTOR_DRIVER_RATED_DC_VOLTAGE
#define MOTOR_DRIVER_RATED_DC_VOLTAGE ((300.0))
#endif // MOTOR_DRIVER_RATED_DC_VOLTAGE

	// Motor Driver parameters, Current pu base
	// unit, SI A
#ifndef MOTOR_DRIVER_RATED_DC_CURRNET
#define MOTOR_DRIVER_RATED_DC_CURRNET ((100.0))
#endif // MOTOR_DRIVER_RATED_DC_CURRNET


	// Motor Driver parameters, maximum current
	// unit, SI A, transient value
#ifndef MOTOR_DRIVER_MAX_BRIDGE_CURRENT
#define MOTOR_DRIVER_MAX_BRIDGE_CURRENT ((100.0))
#endif // MOTOR_DRIVER_MAX_BRIDGE_CURRENT

	// Motor Driver parameters, maximum current
	// unit, SI A, transient value
#ifndef MOTOR_DRIVER_MAX_DC_CURRENT
#define MOTOR_DRIVER_MAX_DC_CURRENT ((100.0))
#endif // MOTOR_DRIVER_MAX_DC_CURRENT

	// Motor Driver parameters, PWM frequency
	// unit, SI Hz
#ifndef MOTOR_DRIVER_PWM_FREQUENCY
#define MOTOR_DRIVER_PWM_FREQUENCY  ((10e3))
#endif // MOTOR_DRIVER_PWM_FREQUENCY

	// Motor Driver parameters, Control law frequency
	// unit, SI Hz
#ifndef MOTOR_DRIVER_CONTROL_FREQUENCY
#define MOTOR_DRIVER_CONTROL_FREQUENCY ((10e3))
#endif // MOTOR_DRIVER_CONTROL_FREQUENCY

	// Motor Driver parameters, PWM generator half period
#ifndef MOTOR_DRIVER_PWM_HALF_CYCLE
#define MOTOR_DRIVER_PWM_HALF_CYCLE ((10000))
#endif // MOTOR_DRIVER_PWM_HALF_CYCLE

	// Current controller loop bandwidth
	// unit, Hz
#ifndef MOTOR_DRIVER_CURRENT_BW
#define MOTOR_DRIVER_CURRENT_BW ((200))
#endif // MOTOR_DRIVER_CURRENT_BW

	// Speed Controller loop bandwidth
	// unit, Hz
#ifndef MOTOR_DRIVER_SPEED_BW
#define MOTOR_DRIVER_SPEED_BW ((50))
#endif // MOTOR_DRIVER_SPEED_BW

	// Speed controller division
	// unit, 1 (times)
#ifndef MOTOR_DRIVER_SPEED_DIV
#define MOTOR_DRIVER_SPEED_DIV ((10))
#endif // MOTOR_DRIVER_SPEED_DIV

	// Speed calculator division
	// unit 1 (times)
#ifndef MOTOR_DRIVER_SPEED_CALC_DIV
#define MOTOR_DRIVER_SPEED_CALC_DIV ((10))
#endif

	// ADC performance resolution
#ifndef MOTOR_DRIVER_ADC_RESOLUTION
#define MOTOR_DRIVER_ADC_RESOLUTION ((16))
#endif

	// ADC Voltage Full scale Range
#ifndef MOTOR_DRIVER_ADC_VOLTAGE_FULL_SCALE
#define MOTOR_DRIVER_ADC_VOLTAGE_FULL_SCALE ((750.0))
#endif // MOTOR_DRIVER_ADC_VOLTAGE_FULL_SCALE

	// ADC Voltage Bias p.u.
#ifndef MOTOR_DIRVER_ADC_VOLTAGE_BIAS_PU
#define MOTOR_DIRVER_ADC_VOLTAGE_BIAS_PU    (0.5)
#endif // MOTOR_DIRVER_ADC_VOLTAGE_BIAS_PU

	// ADC Current Full scale Range
#ifndef MOTOR_DRIVER_ADC_CURRENT_FULL_SCALE
#define MOTOR_DRIVER_ADC_CURRENT_FULL_SCALE ((300.0))
#endif // MOTOR_DRIVER_ADC_CURRENT_FULL_SCALE

	// ADC Current Bias p.u.
#ifndef MOTOR_DIRVER_ADC_CURRENT_BIAS_PU
#define MOTOR_DIRVER_ADC_CURRENT_BIAS_PU    (0.5)
#endif // MOTOR_DIRVER_ADC_CURRENT_BIAS_PU

	// ADC Voltage Full scale Range
#ifndef MOTOR_DRIVER_ADC_VOLTAGE_DC_FULL_SCALE
#define MOTOR_DRIVER_ADC_VOLTAGE_DC_FULL_SCALE ((450.0))
#endif // MOTOR_DRIVER_ADC_VOLTAGE_DC_FULL_SCALE

	// ADC Voltage Bias p.u.
#ifndef MOTOR_DIRVER_ADC_VOLTAGE_DC_BIAS_PU
#define MOTOR_DIRVER_ADC_VOLTAGE_DC_BIAS_PU    (0.15)
#endif // MOTOR_DIRVER_ADC_VOLTAGE_DC_BIAS_PU

	// ADC Current Full scale Range
#ifndef MOTOR_DRIVER_ADC_CURRENT_DC_FULL_SCALE
#define MOTOR_DRIVER_ADC_CURRENT_DC_FULL_SCALE ((30.0))
#endif // MOTOR_DRIVER_ADC_CURRENT_DC_FULL_SCALE

	// ADC Current Bias p.u.
#ifndef MOTOR_DIRVER_ADC_CURRENT_DC_BIAS_PU
#define MOTOR_DIRVER_ADC_CURRENT_DC_BIAS_PU    (0.15)
#endif // MOTOR_DIRVER_ADC_CURRENT_DC_BIAS_PU

	// Encoder Base
#ifndef MOTOR_DRIVER_ENCODER_BASE
#define MOTOR_DRIVER_ENCODER_BASE              ((1<<17))
#endif // MOTOR_DRIVER_ENCODER_BASE

	// rotor acceleration limit, unit rpm/s
#ifndef MOTOR_DRIVER_ACCLERATION
#define MOTOR_DRIVER_ACCLERATION               ((3000))
#endif // MOTOR_DRIVER_ACCLERATION

	// jerk limit, p.u./ms
#ifndef MOTOR_DRIVER_JERK
#define MOTOR_DRIVER_JERK                      ((0.02))
#endif // MOTOR_DRIVER_JERK

	// Fast init wrapper
// With this, it's no need to use init and setup function 
#define MOTOR_DRIVER_CONSULTANT_WRAPPER {\
	MOTOR_DRIVER_RATED_DC_VOLTAGE,\
	MOTOR_DRIVER_RATED_DC_CURRNET, \
	MOTOR_DRIVER_MAX_BRIDGE_CURRENT,\
	MOTOR_DRIVER_MAX_DC_CURRENT,\
	MOTOR_DRIVER_PWM_FREQUENCY,\
	MOTOR_DRIVER_CONTROL_FREQUENCY,\
	MOTOR_DRIVER_PWM_HALF_CYCLE,\
	MOTOR_DRIVER_CURRENT_BW,\
	MOTOR_DRIVER_SPEED_BW,\
	MOTOR_DRIVER_SPEED_DIV,\
	MOTOR_DRIVER_SPEED_CALC_DIV, \
	MOTOR_DRIVER_ADC_RESOLUTION,\
	MOTOR_DRIVER_ADC_VOLTAGE_FULL_SCALE,\
	MOTOR_DIRVER_ADC_VOLTAGE_BIAS_PU,\
	MOTOR_DRIVER_ADC_CURRENT_FULL_SCALE,\
	MOTOR_DIRVER_ADC_CURRENT_BIAS_PU,\
	MOTOR_DRIVER_ADC_VOLTAGE_DC_FULL_SCALE,\
	MOTOR_DIRVER_ADC_VOLTAGE_DC_BIAS_PU,\
	MOTOR_DRIVER_ADC_CURRENT_DC_FULL_SCALE,\
	MOTOR_DIRVER_ADC_CURRENT_DC_BIAS_PU, \
	MOTOR_DRIVER_ENCODER_BASE, \
	MOTOR_DRIVER_ACCLERATION, \
	MOTOR_DRIVER_JERK \
}


typedef struct _tag_motor_driver_consultant_t
{
	// Motor Driver parameters, DC bus voltage
	// unit, SI V
	// voltage p.u. base value
	parameter_gt rated_dc_voltage;

	// Motor Driver parameters, DC bus current
	// unit, SI A
	// current p.u. base value
	parameter_gt rated_dc_current;

	// Motor Driver parameters, maximum current
	// unit, SI A, transient value
	parameter_gt max_phase_current;

	// Motor Driver parameters, maximum current
	// unit, SI A, transient value
	parameter_gt max_dc_current;

	// Motor Driver parameters, PWM frequency
	// unit, SI Hz
	parameter_gt pwm_freq;

	// Motor Driver parameters, control law frequency
	// unit, SI Hz
	parameter_gt control_law_freq;

	// Motor Driver parameters, PWM modulation subdivision
	uint32_t pwm_half_cycle;

	// Motor Current Close loop bandwidth
	// unit, SI Hz
	parameter_gt current_closeloop_bw;

	// Motor Speed Close loop bandwidth
	// unit, SI Hz
	parameter_gt speed_closeloop_bw;

	// Motor Speed Controller Division
	// unit, Current Times
	uint32_t speed_div_times;

	// Motor Speed calculator division
	// unit, current times
	uint32_t speed_calc_div_times;

	// ADC performance
	uint16_t adc_resolution_bit;

	// ADC full scale voltage value (phase)
	parameter_gt adc_voltage_full_scale;

	// ADC voltage bias p.u.(phase)
	// such as 0.5
	parameter_gt adc_voltage_bias;

	// ADC full scale current value(phase)
	parameter_gt adc_current_full_scale;

	// ADC current bias p.u.(phase)
	// such as 0.5
	parameter_gt adc_current_bias;

	// ADC full scale voltage value (DC BUS)
	parameter_gt adc_dc_voltage_full_scale;

	// ADC voltage bias p.u.(DC BUS)
	// such as 0.5
	parameter_gt adc_dc_voltage_bias;

	// ADC full scale current value(DC BUS)
	parameter_gt adc_dc_current_full_scale;

	// ADC current bias p.u.(DC BUS)
	// such as 0.5
	parameter_gt adc_dc_current_bias;

	// Position Encoder Base
	uint32_t position_enc_base;

	// acceleration, unit rpm / s
	parameter_gt acceleration;

	// jerk, unit p.u./ms
	parameter_gt jerk;


}ctl_motor_driver_consultant_t;




#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_MOTOR_DRIVER_CONSULTANT_H_
