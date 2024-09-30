/**
 * @file Buck_constant_param.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
#include "Buck_constant_param.h"

// parameters

// ......................................... //
// Power source
const float E = 48;     // V

// Power Inductance
const float L = 1072e-6;  // H
const float esrL = 0.058; // Ohm

// Power Capacity
const float C = 100e-6; // F
const float esrC = 0.5; // Ohm

// User Load
const float R = 10;	// Ohm

// time step
const float step = 5e-7;

// ......................................... //
// Per unit base value
const float E_base = 48; // V
const float R_base = 10;  // Ohm
const float I_base = E_base / R_base; // A

// ......................................... //
// per unit value
const float E_pu = E / E_base;

const float L_pu = L/R_base;
const float esrL_pu = esrL/R_base;

const float C_pu = C*R_base;
const float esrC_pu = esrC/R_base;

const float Rload_pu = R / R_base;

//// ......................................... //
//// mid variables
//
//// transfer matrix
//float a11_f = - (2*Rload_pu*esrC_pu + esrC_pu*esrL_pu) / (Rload_pu + esrC_pu) / (L_pu / step);
//fpga_math_fix_t a11 = a11_f;
//float a12_f = - (Rload_pu)/(Rload_pu + esrC_pu)/ (L_pu / step);
//fpga_math_fix_t a12 = a12_f;
//float a21_f = Rload_pu / (Rload_pu + esrC_pu) / (C_pu / step);
//fpga_math_fix_t a21 = a21_f;
//float a22_f = -1 / (Rload_pu+esrC_pu) / (C_pu / step);
//fpga_math_fix_t a22 = a22_f;
//
//// control matrix
//float b1_f = E_pu / (L_pu / step);
//fpga_math_fix_t b1 = b1_f;
//
//// output matrix
//float c11_f = esrC_pu / (Rload_pu + esrC_pu);
//fpga_math_fix_t c11 = c11_f;
//float c12_f =  1 / (Rload_pu + esrC_pu);
//fpga_math_fix_t c12 = c12_f;
//float c21_f = Rload_pu*esrC_pu / (Rload_pu + esrC_pu);
//fpga_math_fix_t c21 = c21_f;
//float c22_f = Rload_pu / (Rload_pu + esrC_pu);
//fpga_math_fix_t c22 = c22_f;


// ......................................... //
// mid variables

// transfer matrix

const fpga_math_fix_t a11 = - (2*Rload_pu*esrC_pu + esrC_pu*esrL_pu) / (Rload_pu + esrC_pu) / (L_pu / step);
const fpga_math_fix_t a12 = - (Rload_pu)/(Rload_pu + esrC_pu)/ (L_pu / step);
const fpga_math_fix_t a21 = Rload_pu / (Rload_pu + esrC_pu) / (C_pu / step);
const fpga_math_fix_t a22 = -1 / (Rload_pu+esrC_pu) / (C_pu / step);

// control matrix
const fpga_math_fix_t b1 = E_pu / (L_pu / step);

// output matrix
const fpga_math_fix_t c11 = esrC_pu / (Rload_pu + esrC_pu);
const fpga_math_fix_t c12 = 1 / (Rload_pu + esrC_pu);
const fpga_math_fix_t c21 =  Rload_pu*esrC_pu / (Rload_pu + esrC_pu);
const fpga_math_fix_t c22 = Rload_pu / (Rload_pu + esrC_pu);


void buck_trans(
	fpga_math_fix_t i_L_old,	// initial state of state variables
	fpga_math_fix_t u_C_old,
	fpga_ctrl_bit sw,			// switch state
	fpga_math_fix_t *i_L,		// state variables current L
	fpga_math_fix_t *u_C, 		// state variables voltage C
	fpga_math_fix_t *i_R,		// output variables iR
	fpga_math_fix_t * u_R		// output variables uR
)
{
	fpga_math_fix_t delta_i_L;
	fpga_math_fix_t delta_u_C;

	// step
	delta_i_L = a11*i_L_old + a12*u_C_old;
	delta_u_C = a21*i_L_old + a22*u_C_old;

	// control
	if(sw == 1)
		delta_i_L += b1;
	else
		delta_i_L += 0;

	// update
	(*i_L) = i_L_old + delta_i_L;
	(*u_C) = u_C_old + delta_u_C;

	// output
	(*i_R) = c11*(*i_L) + c12*(*u_C);
	(*u_R) = c21*(*i_L) + c22*(*u_C);

}
