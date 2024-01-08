// general AC motor controller

#include <assert.h>

#include <ctrl/component/motor/encoder.h>
#include <ctrl/component/commmon/pid.hpp>
#include <ctrl/component/common/trajectory.hpp>
#include <ctrl/motor/coord_trans.hpp>
#include <ctrl/motor/svpwm.h>
#include <ctrl/component/common/track.hpp>
#include <ctrl/component/common/divider.hpp>

#ifndef _FILE_MOTOR_GENERAL_H_
#define _FILE_MOTOR_GENERAL_H_

//////////////////////////////////////////////////////////////////////////
// general type of encoder
class encoder_gt
{
public:
	// output variables
	gmp_math_t mech_ang; // p.u.
	int32_t revolution;  // turns
	gmp_math_t elec_ang; // p.u.

	// derived variables
	gmp_math_t speed;    // unit: krpm

	// intrinsic variables
	uint16_t pole_pairs;
	gmp_math_t mech_ang_offset; // offset of mechanical angle, p.u.

	// sin/cos, these two value is for transformation
	gmp_math_t elec_ang_sin;
	gmp_math_t elec_ang_cos;

public:
	// encoder callback
	void angle_calc()
	{
		// This function should get the Field position

	}
};

//////////////////////////////////////////////////////////////////////////
// general type of inverter intrinsic
class inv_intrinsic_gt
{
public:
	gmp_ctrl_param_t pwm_freq_kHz;		// PWM frequency, (kHz)
	gmp_ctrl_param_t Udc_V;				// DC bus voltage, SI (V)

	gmp_ctrl_param_t Rds_on_ef;			// equivalent Rds(on)
};

//////////////////////////////////////////////////////////////////////////
// general type of motor intrinsic

// motor intrinsic variables from norm plate 
class motor_intrinsic_plate_gt
{
public:
	uint16_t pairs;						// Number of motor poles

	gmp_ctrl_param_t ctrl_freq_kHz;		// controller frequency, (kHz)
	gmp_ctrl_param_t Pn_kW;				// rated power, SI (kW)
	gmp_ctrl_param_t Un_V;				// rated voltage, SI (V)
	gmp_ctrl_param_t In_A;				// rated current, SI (A)
	gmp_ctrl_param_t fn_Hz;				// rated frequency, SI (Hz)
	gmp_ctrl_param_t speed_rated_krpm;	// rated speed, SI (krpm)

	gmp_motor_duty_t motor_duty;		// duty type of motor
};

// If you would control a hardly unknown motor
// this additional two variables you may get it by RLC meter
class motor_intrinsic_visible_gt
	:public motor_intrinsic_plate_gt
{
public:
	gmp_ctrl_param_t Rs;				// SI (Ohm)
	gmp_ctrl_param_t Ls;				// SI (mH, Henry)
};

// If you are the designer of the motor you will now the details
// parameters of motor
class motor_intrinsic_inner_gt
	:public motor_intrinsic_visible_gt
{
public:
	gmp_ctrl_param_t Rd;				// SI (Ohm), 
	gmp_ctrl_param_t Ld;				// SI (mH, Henry), 
	gmp_ctrl_param_t Rq;				// SI (Ohm), 
	gmp_ctrl_param_t Lq;				// SI (mH, Henry), 
	gmp_ctrl_param_t flux;				// SI (Wb), 
	gmp_ctrl_param_t inertia;			// SI (kg m2), machinery energy capacity
	gmp_ctrl_param_t damp;				// SI (N s/m), damp per velocity
	gmp_ctrl_param_t friction;			// SI (N m), bias of damp
};

// adc sampling interface type
class motor_adc_atleast_gt
{
public:
	// ADC result after transform variables
	gmp_math_t I_abc[3];				// In p.u.
	gmp_math_t I_bus;					// In p.u.

	// auxiliary scale factor
	// The following two factors would be calculated by intrinsic variables
	gmp_math_t I_adc2abc_sf[3];			// transfer ADC current into Iabc p.u. value
	gmp_math_t I_adc2bus_sf;			// transfer ADC current into Ibus p.u. value

	// The following two factors would be calculated by intrinsic variables of sampling circuit 
	gmp_math_t I_adc2abc_bias[3];		// transfer ADC current into AC value
	gmp_math_t I_adc2bus_bias;			// transfer ADC current into bus value

	// coordinate transform result
	gmp_math_t Idq[2];					// In p.u.

public:
	// sampling 
	void sampling()
	{
		// Call ADC process and get the sampling results

		// auto scale factor

	}
};

class motor_adc_all_gt
	:public motor_adc_atleast_gt
{
public:
	// ADC result after transform
	gmp_math_t V_abc[3];				// Vn p.u.
	gmp_math_t V_bus;					// Vn p.u.

	// auxiliary scale factor
	gmp_math_t V_adc2abc_sf[3];			// transfer ADC voltage into Uabc p.u. value
	gmp_math_t V_adc2bus_sf;			// transfer ADC voltage into Ubus p.u. value
	gmp_math_t V_adc2abc_bias[3];		// transfer ADC current into AC value
	gmp_math_t V_adc2bus_bias;			// transfer ADC current into bus value

	// coordinate transform result
	gmp_math_t Vdq[2];					// Vn p.u.
};

//////////////////////////////////////////////////////////////////////////
// general type of motor interface
class motor_interface_gt
{
public:
	// Speed reference, (krpm, Revolution Per Minute)
	gmp_math_t speed_set_krpm;
	// current set, p.u.
	// in speed mode and position mode, 
	// this value shows the maximum output of the current
	gmp_math_t Idq_set[2];
	// voltage set, p.u.
	// in current mode and speed mode and current mode, 
	// this value is means that the maximum output of Voltage.
	gmp_math_t Vdq_set[2];
};

//////////////////////////////////////////////////////////////////////////
// general type of motor monitor variables
class motor_monitor_gt
{
public:
	// torque
	gmp_math_t torque_Nm;				// Output Torque, unit Nm

	// stator current
	gmp_math_t Is;						// stator current, unit In p.u.

	// magnetizing current
	gmp_math_t Im;						// magnetizing current, unit In p.u.

	// zero-sequence current
	gmp_math_t I0;						// zero-sequence current, unit In p.u.

	// temperature
	int16_t motor_temp_centideg;		// motor temperature sensor, unit 0.01 Celsius degree
	int16_t ctrl_temp_centideg;			// inverter temperature sensor, unit 0.01 Celsius degree

	// acceleration
	gmp_ctrl_param_t acc;				// acceleration, SI unit rad/s2

	// flux
	gmp_math_t flux[2];					// flux observed, unit pmsm_intrinsic_gt::flux p.u.

	// Continuous working time
	uint32_t cw_cnt;					// describe the motor continuous working time, unit Sec

};

//////////////////////////////////////////////////////////////////////////
// controller

// template <typename reg_type, typename traj_type>
// class traj_tracking_ctrl_gt
// {
// public:
// 	// output
// 	gmp_math_t out;
// 
// public:
// 	// feedback controller
// 	reg_type regulator;
// 
// public:
// 	// ramping setting
// 	traj_type ramp;
// 
// public:
// 	// counter divider
// 	uint16_t ctrl_cnt;
// 	uint16_t ctrl_cnt_divider;
// 
// 
// 
// public:
// 	// default controller function 
// 	inline void exec(gmp_math_t target, gmp_math_t feedback)
// 	{
// 
// 	}
// };

typedef tracking_ctrl_gt<pid_reg_t> motor_asr_gt;

class motor_asr_ff_gt
	:public motor_asr_gt
{
public:
	// feed-forward: fraction, acceleration, torque output
	gmp_math_t fraction;
	gmp_math_t acc;
	gmp_math_t toruqe;

public:
	// bounds
	gmp_math_t out_min;
	gmp_math_t out_max;

public:
	inline void exec(gmp_math_t spd_target, gmp_math_t spd_feedback)
	{


		// fraction torque
		if (spd_target > 0)
		{
			out = fraction;
		}
		else if (spd_target < 0)
		{
			out = -fraction;
		}
		else
		{
			out = 0;
		}

		// accelerate torque
		out += acc;

		// output torque 
		out += torque;

		// feedback process
		regulator.exec(spd_target - spd_feedback);
		out += regulator.out;

		// saturation
		out = gmp_math_sat(out, out_min, out_max);
	}

	inline void set_output_bound(gmp_math_t out_min, gmp_math_t out_max)
	{
		this->out_min = out_min;
		this->out_max = out_max;
		regulator.out_min = out_min;
		regulator.out_max = out_max;
	}

};

typedef tracking_ctrl_gt<pid_reg_t> motor_acr_iq_gt;

class motor_acr_iq_ff_gt
	:public motor_acr_iq_gt
{
public:
	// feed-forward: speed
	gmp_math_t Kv;

public:
	inline void exec(gmp_math_t iq_target, gmp_math_t iq_feedback,
		gmp_math_t spd)
	{
		if (!ctrl_cnt)
		{
			// speed feed-forward
			out = speed * Kv;

			// feedback process
			regulator.exec(iq_target - iq_feedback);
			out += regulator.out;

			// saturation
			out = gmp_math_sat(out, out_min, out_max);
		}

		if (ctrl_cnt++ >= ctrl_cnt_divider)
			ctrl_cnt = 0;
	}

	inline void set_output_bound(gmp_math_t out_min, gmp_math_t out_max)
	{
		this->out_min = out_min;
		this->out_max = out_max;
		regulator.out_min = out_min;
		regulator.out_max = out_max;
	}

};

typedef tracking_ctrl_gt<pid_reg_t> motor_acr_id_gt;


//////////////////////////////////////////////////////////////////////////
// Default general state machine
class motor_default_sm_gt
{
public:
	// This function would be called multi-times, when state machine keep init process
	void inited()
	{

	}

	// This function would be called when state machine is power-off state
	void poweroff()
	{

	}

	// This function would be called when state machine is power-on state
	void poweron()
	{

	}

	// This function would be called when quick stop command is received
	void quick_stop()
	{

	}

	// This function would only be called once, when motor controller init
	void init_react()
	{

	}

	// This function would be called when power-off command received
	void poweroff_react()
	{

	}

	// This function would be called when power-on command received
	void poweron_react()
	{

	}

	// This function would be called when quick stop command received
	void quick_stop_react()
	{

	}

	// This function should be called when fault occurred
	void fault_react()
	{

	}

	// This function should be called when fault barrier occurred
	void fault()
	{

	}

};

//////////////////////////////////////////////////////////////////////////
// controller pure values
class acmtr_gt_base
{
public: // controller type

	typedef enum _tag_phase_enum_t
	{
		PHASE_U = 0,
		PHASE_V = 1,
		PHASE_W = 2
	}phase_t;

	typedef enum _tag_dq_enum_t
	{
		phase_d = 0,
		phase_q = 1
	}dq_t;

	typedef enum _tag_motor_closeloop_t
	{
		// Controller is disabled,
		// Motor will not get output
		MOTOR_DISABLED = 0,
		// User may all other close loop level here.

		// Motor will running in open-loop mode
		OPENLOOP = 16,

		// Motor will running in current-loop mode
		CURRENTLOOP = 64,

		// Motor will running in speed-loop mode
		SPEEDLOOP = 128,

		// Motor will running in position-loop mode
		POSITIONLOOP = 256
	}motor_closeloop_t;

public:
	// output variables
	gmp_math_t Vab[2];    // alpha-beta axis output voltage
	gmp_math_t T_abc[3];  // SVPWM generator

	// controllers levels
	motor_closeloop_t close_loop_level;

	// controller running switch
	uint8_t flag_run;

public: // controller ticks

};

//////////////////////////////////////////////////////////////////////////
// General AC motor
template<
	typename adc_type,
	typename encoder_type,
	typename interface_type,
	typename mtr_intrinsic_type,
	typename inv_intrinsic_type,
	typename spd_ctrl_type,
	typename iq_ctrl_type,
	typename id_ctrl_type,
	// 	typename state_machine_type,
	typename monitor_type
>
class acmtr_gt
	:public acmtr_gt_base
{
public: // input variables
	adc_type adc;			// current and voltage analog sensor
	encoder_type enc;		// encoder for the motor

private: // intermediate variables
	interface_type target;			// generate system target

public: // user interface
	interface_type user_if;			// user input interface

	// intrinsic variables
	inv_intrinsic_type inv;
	mtr_intrinsic_type motor;

	// controllers
	ctrl_divider_t spd_div;			// speed loop divider
	spd_ctrl_type spd_ctl;
	traj_ramp_t spd_traj;

	ctrl_divider_t cur_div;			// current loop divider
	iq_ctrl_type iq_ctl;
	traj_ramp_t iq_traj;

	id_ctrl_type id_ctl;
	traj_ramp_t id_traj;


	//public: // state machine
	//	state_machine_type sm;
	//
	//public: // protection & limitation
	//	boundary_set_t* boundarys;

	// monitor port
	monitor_type monitor;

	// controller main process
	// export general motor controller function
	inline void gmp_motor_ctrl_g()
	{
		// validate parameters

		if (flag_run)
		{
			// ADC sampling and transformation
			adc.sampling();

			// Encoder angle calculating
			enc.angle_calc();

			// park transform
			abc2dq(&adc.I_abc, enc.elec_ang, &adc.Idq);

			// speed control stage
			if (close_loop_level >= SPEEDLOOP
				&& spd_div.is_tick())
				// invoke spd controller 
			{
				// assign target value
				target.speed_set_krpm = user_if.speed_set_krpm;
				spd_ctl.set_min_limit(-user_if.Idq_set[phase_q]);
				spd_ctl.set_max_limit(user_if.Idq_set[phase_q]);

				// equal to input filter
				spd_ctl.exec(spd_traj.get_next_position(target.speed_set_krpm), 
					enc.speed);

				// assign inner loop target
				target.Idq_set[phase_q] = spd_ctl.out;
				target.Idq_set[phase_d] = 0;
			}
			else
				// release the sod controller
			{
				// assign inner loop target
				target.Idq_set[phase_q] = user_if.Idq_set[phase_q];
				target.Idq_set[phase_d] = user_if.Idq_set[phase_d];
			}

			// current control stage
			if (close_loop_level >= CURRENTLOOP
				&& cur_div.is_tick())
				// invoke current controller
			{
				// assign target value
				iq_ctl.set_min_limit(-user_if.Vdq_set[phase_q]);
				iq_ctl.set_max_limit(user_if.Vdq_set[phase_q]);
// 				iq_ctl.regulator.out_min = -user_if.Vdq_set[phase_q];
// 				iq_ctl.regulator.out_max = user_if.Vdq_set[phase_q];

				// iq controller
				iq_ctl.exec(iq_traj.get_next_position(target.Idq_set[phase_q]), 
					adc.Idq[phase_q]);
				target.Vdq_set[phase_q] = iq_ctl.out;

				// calculate id controller limit 
				gmp_math_t ud_limit = gmp_math_sqrt(1 - target.Vdq_set[phase_q] * target.Vdq_set[phase_q]);

				if (ud_limit > user_if.Vdq_set[phase_d])
					ud_limit = user_if.Vdq_set[phase_d];

				id_ctl.set_min_limit(-ud_limit);
				id_ctl.set_max_limit(ud_limit);
// 				id_ctl.regulator.out_min = -ud_limit;
// 				id_ctl.regulator.out_max = ud_limit;

				// id controller
				id_ctl.exec(id_traj.get_next_position(target.Idq_set[phase_d]), adc.Idq[phase_d]);
				target.Vdq_set[phase_d] = id_ctl.out;
			}
			else
				// release the current controller
			{
				target.Vdq_set[phase_q] = user_if.Vdq_set[phase_q];
				target.Vdq_set[phase_d] = user_if.Vdq_set[phase_d];
			}

			

			// voltage output stage
			if (acmtr->close_loop_level >= OPENLOOP)
				// invoke voltage output stage
			{
				// ipark trans
				dq2phasor(&target.Vdq_set, enc.elec_ang, &Vab);

				// alpha-beta axis to Tabc
				svpwm_gen(&Vab, &T_abc);

				// write duty

				// enable PWM

			}
			else
			{
				// Disable PWM output

				// Clear PWM duty

			}


		} // run_flag
	}// function gmp_motor_ctrl_g

public: // state exchange variables 

};

// export general motor control type
typedef acmtr_gt<motor_adc_atleast_gt, encoder_gt,
	motor_interface_gt,
	inv_intrinsic_gt, motor_intrinsic_plate_gt,
	motor_asr_ff_gt, motor_acr_iq_ff_gt, motor_acr_id_gt,
	motor_default_sm_gt,
	motor_monitor_gt>
	acm_commnon_ctrl_t; // output types



#endif // _FILE_MOTOR_GENERAL_H_

