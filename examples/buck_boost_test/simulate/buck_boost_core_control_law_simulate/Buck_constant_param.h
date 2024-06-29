
#ifndef _FILE_BUCK_CONSTANT_PARAM_H_
#define _FILE_BUCK_CONSTANT_PARAM_H_


#include <core/gmp_core.h>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

typedef float fpga_math_fix_t;


typedef uint8_t fpga_ctrl_bit;

// Function prototype
void buck_trans(
	fpga_math_fix_t i_L_old,	// initial state of state variables
	fpga_math_fix_t u_C_old,
	fpga_ctrl_bit sw,			// switch state
	fpga_math_fix_t *i_L,		// state variables current L
	fpga_math_fix_t *u_C, 		// state variables voltage C
	fpga_math_fix_t *i_R,		// output variables iR
	fpga_math_fix_t * u_R		// output variables uR
);


extern const float E;     // V
extern const float L;  // H
extern const float esrL; // Ohm

extern const float C; // F
extern const float esrC; // Ohm

// User Load
extern const float R;	// Ohm

// time step
extern const float step;

extern const float E_base; // V
extern const float R_base;  // Ohm
extern const float I_base; // A


#endif //_FILE_BUCK_CONSTANT_PARAM_H_
