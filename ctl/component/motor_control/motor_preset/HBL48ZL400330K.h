
#ifndef _FILE_HBL48ZL40030K_H_
#define _FILE_HBL48ZL40030K_H_

// PMSM Motor Type
#define MOTOR_TYPE PMSM_MOTOR

//////////////////////////////////////////////////////////////////////////
// Basic parameters
//
// Number of pole pairs
#define MOTOR_PARAM_POLE_PAIRS ((8))

// Stator Resistance, unit Ohm
#define MOTOR_PARAM_RS ((0.4 / 2))

// Stator Inductance, unit H
#define MOTOR_PARAM_LS ((0.6e-3 / 2))

// Inertia, unit kg cm m2
#define MOTOR_PARAM_INERTIA ((0.45))

// Friction unit nNms
#define MOTOR_PARAM_FRICTION ((0.55))

// constant velocity of a motor, unit rpm/V
// #define MOTOR_PARAM_KV ((206.2))

// Flux linkage, unit Wb
// #define MOTOR_PARAM_FLUX ((MOTOR_PARAM_CALCULATE_FLUX_BY_KV(MOTOR_PARAM_KV)))

// Back EMF Voltage, unit V/krpm
#define MOTOR_PARAM_EMF           ((10.5))

#define MOTOR_ENCODER_TYPE        ((QEP_ENCODER))

#define MOTOR_ENCODER_LINE_NUMBER ((2500))

//////////////////////////////////////////////////////////////////////////
// Rated parameters

// Rated voltage, unit V
#define MOTOR_PARAM_RATED_VOLTAGE ((48))

// no load current, unit A
#define MOTOR_PARAM_NO_LOAD_CURRENT ((0.25))

// Rated speed rpm
#define MOTOR_PARAM_RATED_SPEED ((3000))

// Rated current
#define MOTOR_PARAM_RATED_CURRENT ((11.0))

// Rated torque, unit Nm
#define MOTOR_PARAM_RATED_TORQUE ((1.27))

//////////////////////////////////////////////////////////////////////////
// Maximum parameters

// Maximum Torque, unit Nm
#define MOTOR_PARAM_MAX_TORQUE ((3.81))

// Maximum Speed, unit rpm
#define MOTOR_PARAM_MAX_SPEED ((3500))
// #define MOTOR_PARAM_MAX_SPEED ((1000))

// Maximum DC bus voltage, unit V
#define MOTOR_PARAM_MAX_DC_VOLTAGE ((54))

// Maximum Phase Current RMS, unit A
#define MOTOR_PARAM_MAX_PH_CURRENT ((33.0))

#endif // _FILE_HBL48ZL40030K_H_
