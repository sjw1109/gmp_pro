
#include <ctrl/component/common/pi.h>

// Buck Model
// 

#ifndef _FILE_BUCK_H_
#define _FILE_BUCK_H_

typedef enum gmp_buck_io_enum_t
{
	DPC_INPUT_PORT = 0,
	DPC_OUTPUT_PORT = 1
};

typedef enum gmp_buck_closeloop_t
{
	// DPC disable
	DPC_DISDABLE = 0,

	
	DPC_OPENLOOP = 16,

	// DPC current close loop
	DPC_CURRENT_LOOP = 64,

	// DPC voltage close loop
	DPC_VOLTAGE_LOOP = 128
}buck_closeloop_t;

// controlled plant
typedef struct _tag_buck_plant
{
	// input variables
	gmp_math_t V_io[2];					// Vn p.u.
	gmp_math_t I_io[2];					// In p.u.
	gmp_math_t IL;						// In p.u.

	// output variables
	gmp_math_t T_duty;					// output buck switch duty

	// intermediate variables
	gmp_math_t io_set;					// support constant current
	gmp_math_t Vo_set;					// support constant voltage
	gmp_math_t Po_set;					// support constant power

	// intrinsic variables
	gmp_ctrl_param_t rated_P;			// P norm, unit kW
	gmp_ctrl_param_t rated_U;			// U norm, unit V
	gmp_ctrl_param_t rated_I;			// I norm, unit A

	gmp_ctrl_param_t pwm_freq_kHz;		// PWM frequency, unit kHz
	gmp_ctrl_param_t ctrl_freq_kHz;		// controller frequency, unit kHz
	gmp_ctrl_param_t L;					// Inductor, unit uH
	gmp_ctrl_param_t Lesr;				// ESR(Equivalent Series Resistance) of Inductor, unit mOhm
	gmp_ctrl_param_t C;					// Capacitor, unit uF
	gmp_ctrl_param_t Cesr;				// ESR of capacitor, unit mOhm
	gmp_ctrl_param_t R;					// Equivalent load R, unit Ohm


	// properties variables
	uint16_t close_loop_level;

	// state machine

	// protection & limitation

	// monitor port

	// controller entity
	
}buck_plant;

// control function
inline void buck_ctrl_VI_pid(
	buck_plant* buck,
	pid_regular_t* voltage_regular,
	pid_regular_t* current_regular
)
{
	// Check controller parameter
	assert(buck);
	assert(voltage_regular);
	assert(current_regular);

	// Check and Run Voltage loop
	// Step1 判定是否需要执行电压环
	// Step2 电压环控制频率分频
	// Step3 执行电压环的动作

	if (buck->close_loop_level >= DPC_VOLTAGE_LOOP)
	{

	}



}


#endif // _FILE_BUCK_H_
