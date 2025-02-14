

#ifndef _FILE_MOTOR_UNIT_CALCULATOR_H_
#define _FILE_MOTOR_UNIT_CALCULATOR_H_

//////////////////////////////////////////////////////////////////////////
// Constant Section

// Constant sqrt(3)
#define MTR_CONST_SQRT_3                               ((1.732050807568877))

// Calculate Motor parameter Kv by flux
#define MOTOR_PARAM_CALCULATE_KV_BY_FLUX(flux, pole_pair) ((flux * 30 / PI / pole_pair / CONST_SQRT_3))

// Calculate Motor parameter flux by Kv
#define MOTOR_PARAM_CALCULATE_FLUX_BY_KV(Kv, pole_pair) ((Kv * PI * pole_pair / 10.0 / CONST_SQRT_3))


#endif // _FILE_MOTOR_UNIT_CALCULATOR_H_
