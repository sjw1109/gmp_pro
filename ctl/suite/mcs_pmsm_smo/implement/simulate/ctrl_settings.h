
#ifndef _FILE_CTRL_SETTINGS_H_
#define _FILE_CTRL_SETTINGS_H_

// invoke motor parameters
#include <ctl/component/motor_control/motor_preset/GBM2804H_100T.h>

// Given 3.3V voltage reference
#define ADC_REFERENCE ((3.3))

// Controller Frequency
#define CONTROLLER_FREQUENCY (10000)

// PWM depth
#define CONTROLLER_PWM_CMP_MAX (6000)

// Speed controller Division
#define SPD_CONTROLLER_PWM_DIVISION (5)

// Controller Base Voltage and Base Current
#define MTR_CTRL_VOLTAGE_BASE ((MOTOR_PARAM_MAX_DC_VOLTAGE))
#define MTR_CTRL_CURRENT_BASE ((MOTOR_PARAM_RATED_CURRENT))

// Current sensor
#define MTR_CTRL_CURRENT_SAMPLE_RANGE (16.5)

// Current ADC module default per unit parameter
#define MTR_CTRL_CURRENT_GAIN         (MTR_CTRL_CURRENT_SAMPLE_RANGE * 2 / MTR_CTRL_CURRENT_BASE)
#define MTR_CTRL_CURRENT_BIAS         (1.65 / ADC_REFERENCE)

// Voltage sensor
#define MTR_CTRL_VOLTAGE_SAMPLE_RANGE ((80))

// Voltage ADC module default per unit parameter
#define MTR_CTRL_VOLTAGE_GAIN         ((MTR_CTRL_VOLTAGE_SAMPLE_RANGE / MTR_CTRL_VOLTAGE_BASE))
#define MTR_CTRL_VOLTAGE_BIAS         ((0.0))

// Current Bandwidth
#define MTR_CTRL_CURRENT_LOOP_BW ((100))

// Speed Bandwidth
#define MTR_CTRL_SPEED_LOOP_BW ((20))

// BUILD_LEVEL 1: Voltage Open loop
// BUILD_LEVEL 2: Current Open loop
// BUILD_LEVEL 3: SMO with speed loop
// BUILD_LEVEL 4: Speed Close loop
#define BUILD_LEVEL (3)

#endif // _FILE_CTRL_SETTINGS_H_
