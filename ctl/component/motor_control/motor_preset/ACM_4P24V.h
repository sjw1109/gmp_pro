
#include <ctl/component/motor_control/basic/motor_unit_calculator.h>

#ifndef _FILE_ACM_4P24V_H_
#define _FILE_ACM_4P24V_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// ACM Motor Type
#define MOTOR_TYPE INDUCTION_MOTOR

// Encoder Type
#define ENCODER_TYPE QEP_INC_ENCODER

//////////////////////////////////////////////////////////////////////////
// Basic parameters
//
// Number of pole pairs
#define MOTOR_PARAM_POLE_PAIRS ((2))

// Stator Resistance, unit Ohm
#define MOTOR_PARAM_RS ((0.329))

// Stator Inductance, unit H
#define MOTOR_PARAM_L1S ((0.0003))

// Rotor Resistance, unit Ohm
#define MOTOR_PARAM_RR ((0.44))

// Rotor Inductance, unit H
#define MOTOR_PARAM_L1R ((0.0005))

// Main inductance, unit H
#define MOTOR_PARAM_LM ((0.0012))
//#define MOTOR_PARAM_LM ((0.0005))

// Inertia, unit nMms2
#define MOTOR_PARAM_INERTIA ((0.001))

// Friction unit nNms
#define MOTOR_PARAM_FRICTION ((0.00001))

//////////////////////////////////////////////////////////////////////////
// Rated parameters

// Rated voltage, unit V
#define MOTOR_PARAM_RATED_VOLTAGE ((24.0))

// Rated Current, unit A
#define MOTOR_PARAM_RATED_CURRENT ((8.5))

// no load current, unit A
#define MOTOR_PARAM_NO_LOAD_CURRENT ((0.5))

// Rated Frequency, unit Hz
#define MOTOR_PARAM_RATED_FREQUENCY ((50.0))

//////////////////////////////////////////////////////////////////////////
// Maximum parameters

// Maximum Torque, unit Nm
#define MOTOR_PARAM_MAX_TORQUE ((0.981))

// Maximum Speed, unit rpm
#define MOTOR_PARAM_MAX_SPEED ((1450))

// Maximum DC bus voltage, unit V
#define MOTOR_PARAM_MAX_DC_VOLTAGE ((36))

// Maximum Phase Current RMS, unit A
#define MOTOR_PARAM_MAX_PH_CURRENT ((10.0))


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_ACM_4P24V_H_
