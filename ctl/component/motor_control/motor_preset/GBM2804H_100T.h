

#include <ctl/component/motor_control/basic/motor_unit_calculator.h>


#ifndef _FILE_MOTOR_PARAM_GBM2804H_100T_H_
#define _FILE_MOTOR_PARAM_GBM2804H_100T_H_


//
// Information Source:
// https://zhuanlan.zhihu.com/p/545688192
//

//
// Standard Encoder Interface
// SPI Encoder
// CS black
// clk blue
// MISO green
// MOSI yellow
// GND white
// +5V red
//


//////////////////////////////////////////////////////////////////////////
// Basic parameters
//
// Number of pole pairs
#define MOTOR_PARAM_POLE_PAIRS ((7))

// Stator Resistance, unit Ohm
#define MOTOR_PARAM_RS ((4.7))

// Stator Inductance, unit H
#define MOTOR_PARAM_LS ((0.96e-3))

// Inertia, unit nMms2
#define MOTOR_PARAM_INERTIA ((497.0))

// Friction unit nNms
#define MOTOR_PARAM_FRICTION ((755.0))

// constant velocity of a motor, unit rpm/V
#define MOTOR_PARAM_KV ((206.2))

// Flux linkage, unit Wb
#define MOTOR_PARAM_FLUX ((MOTOR_PARAM_CALCULATE_FLUX_BY_KV(MOTOR_PARAM_KV)))

// Back EMF Voltage, unit V/krpm
#define MOTOR_PARAM_EMF ((4.85))

//////////////////////////////////////////////////////////////////////////
// Rated parameters

// Rated voltage, unit V
#define MOTOR_PARAM_RATED_VOLTAGE ((10))

// no load current, unit A
#define MOTOR_PARAM_NO_LOAD_CURRENT ((0.01))

//////////////////////////////////////////////////////////////////////////
// Maximum parameters

// Maximum Torque, unit Nm
#define MOTOR_PARAM_MAX_TORQUE ((0.981))

// Maximum Speed, unit rpm
#define MOTOR_PARAM_MAX_SPEED ((2180))
//#define MOTOR_PARAM_MAX_SPEED ((1000))

// Maximum DC bus voltage, unit V
#define MOTOR_PARAM_MAX_DC_VOLTAGE ((14.2))

// Maximum Phase Current RMS, unit A
#define MOTOR_PARAM_MAX_PH_CURRENT ((5.0))

#endif // _FILE_MOTOR_PARAM_GBM2804H_100T_H_

