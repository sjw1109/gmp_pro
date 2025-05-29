
#include <ctl/component/motor_control/basic/motor_unit_calculator.h>

#ifndef _FILE_MOTOR_PARAM_TYI5008_KV335_H_
#define _FILE_MOTOR_PARAM_TYI5008_KV335_H_

// PMSM Motor Type
#define MOTOR_TYPE PMSM_MOTOR

//////////////////////////////////////////////////////////////////////////
// Basic parameters
//
// Number of pole pairs
#define MOTOR_PARAM_POLE_PAIRS ((7))

// Stator Resistance, unit Ohm
#define MOTOR_PARAM_RS ((0.12))

// Stator Inductance, unit H
#define MOTOR_PARAM_LS ((50.0e-6))

// Inertia, unit nMms2
// #define MOTOR_PARAM_INERTIA ((497.0))

// Friction unit nNms
// #define MOTOR_PARAM_FRICTION ((755.0))

// Flux linkage, unit Wb
#define MOTOR_PARAM_FLUX ((0.0021))

// constant velocity of a motor, unit rpm/V
#define MOTOR_PARAM_KV ((360))

// Back EMF Voltage, unit V/krpm
#define MOTOR_PARAM_EMF((4.85))

//////////////////////////////////////////////////////////////////////////
// Rated parameters

// Rated voltage, unit V
#define MOTOR_PARAM_RATED_VOLTAGE ((24.0))

// no load current, unit A
#define MOTOR_PARAM_NO_LOAD_CURRENT ((0.5))

//////////////////////////////////////////////////////////////////////////
// Maximum parameters

// Maximum Torque, unit Nm
#define MOTOR_PARAM_MAX_TORQUE ((0.25))

// Maximum Speed, unit rpm
#define MOTOR_PARAM_MAX_SPEED ((8600))

// Maximum DC bus voltage, unit V
#define MOTOR_PARAM_MAX_DC_VOLTAGE ((25.2))

// Maximum Phase Current RMS, unit A
#define MOTOR_PARAM_MAX_PH_CURRENT((23.0))

#endif //_FILE_MOTOR_PARAM_TYI5008_KV335_H_
