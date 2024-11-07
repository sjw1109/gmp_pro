/**
 * @file encoder.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// based on filter module
#include "core/std/gmp_cport.h"
#include <ctl/component/intrinsic/discrete/filter.h>


#ifndef _FILE_ENCODER_H_
#define _FILE_ENCODER_H_

#ifdef __cplusplus
extern "C"
{
	#endif // __cplusplus


// Two parts
// Speed Encoder
// Position Encoder

typedef struct _tag_rotation_encoder_t
{
	// All the rotation encoder should start with the following one items

	// position of the encoder 
	// unit, p.u.
	ctrl_gt position;

}ctl_rotation_enc_t;

// Get encoder position
GMP_STATIC_INLINE
ctrl_gt ctl_get_encoder_position(ctl_rotation_enc_t* enc)
{
	return enc->position;
}


//////////////////////////////////////////////////////////////////////////
// position encoder
typedef struct _tag_ctl_pos_encoder_t
{
	// angle position, p.u.
	// Mechanical position
	ctrl_gt position;

	// offset,
	// position + offset means the true position
	ctrl_gt offset;

	// pole_pairs
	// poles*(position + offset) = Electrical position
	uint16_t poles;

	// uint32_t p.u. base value
	uint32_t position_base;

	// angle position, p.u.
	ctrl_gt elec_angle;

}ctl_pos_encoder_t;


void ctl_init_pos_encoder(ctl_pos_encoder_t* pos_encoder);

void ctl_setup_pos_encoder(ctl_pos_encoder_t* enc, uint16_t poles, uint32_t position_base);


// input functions should be called in input stage
GMP_STATIC_INLINE
void ctl_input_pos_encoder(ctl_pos_encoder_t* pos_encoder, uint32_t raw)
{
	pos_encoder->position = ctl_div(raw, pos_encoder->position_base);
}

//GMP_STATIC_INLINE
//void ctl_input_pos_encoder_shift(ctl_pos_encoder_t* pos_encoder,
//	uint32_t raw, fast_gt shift)
//{
//	pos_encoder->position = raw << shift;
//}


// GET ANGLE, unit rad
GMP_STATIC_INLINE
ctrl_gt ctl_get_elec_angle_via_pos_encoder(ctl_pos_encoder_t* pos_encoder)
{
	ctrl_gt elec_pos = pos_encoder->poles * (pos_encoder->position + GMP_CONST_1 - pos_encoder->offset);

	ctrl_gt elec_pos_pu = ctrl_mod_1(elec_pos);

//	ctrl_gt angle = ctl_mul(elec_pos_pu, GMP_CONST_2_PI);

	pos_encoder->elec_angle = elec_pos_pu;

	return angle;
}


GMP_STATIC_INLINE
void ctl_set_pos_encoder_offset(ctl_pos_encoder_t* enc, ctrl_gt offset)
{
	enc->offset = offset;
}


//////////////////////////////////////////////////////////////////////////
// Multi-turns encoder

typedef struct _tag_ctl_pos_multiturn_encoder_t
{

	// angle position, p.u.
	// Mechanical position
	ctrl_gt position;

	// offset,
	// position + offset means the true position
	ctrl_gt offset;

	// pole_pairs
	// poles*(position + offset) = Electrical position
	uint16_t poles;

	// angle position, position p.u.
	ctrl_gt elec_angle;

	// lap counter
	int32_t turns;


}ctl_pos_multiturn_encoder_t;


//////////////////////////////////////////////////////////////////////////
// speed encoder
typedef struct _tag_ctl_volocity_encoder_t
{
	// Speed p.u. base, unit rad/s or rpm
	parameter_gt speed_base;

	// Speed p.u. value
	ctrl_gt speed;

	// Speed krpm value
	ctrl_gt speed_krpm;
}ctl_spd_encoder_t;

void ctl_init_spd_encoder(ctl_spd_encoder_t* spd_encoder);

void ctl_setup_spd_encoder(ctl_spd_encoder_t* enc, parameter_gt speed_base);


//////////////////////////////////////////////////////////////////////////
// Speed calculator based on position

typedef struct _tag_speed_calculator_t
{
	// position current data 
	// elec angle
	ctrl_gt position;

	// position old data
	ctrl_gt old_position;

	// speed scale factor
	// speed = delta_ticks * scale_factor
	ctrl_gt scale_factor;

	// Speed output
	ctrl_gt speed;

	// speed filter
	ctl_low_pass_filter_t spd_filter;

	// speed division 
	ctl_divider_t div;

}ctl_spd_calculator_t;

void ctl_init_spd_calculator(ctl_spd_calculator_t* sc);

// (pos - old_pos) * speed_calc_freq -> rad/s
// 60/(2*pi)*(rad/s) -> rpm
// rpm / (pole_pairs * rate_speed_rpm) -> Mech p.u.


// User must ensure fcalc > n_max_rpm / 30
void ctl_setup_spd_calculator(ctl_spd_calculator_t* sc,
	parameter_gt control_law_freq,  // control law frequency, unit Hz
	uint16_t speed_calc_div,  // division of control law frequency, unit ticks
	parameter_gt rated_speed_rpm,  // Speed per unit base value, unit rpm
	uint16_t pole_pairs,		   // pole pairs, if you pass a elec angle,
	// just set this value to 1.
	parameter_gt speed_filter_fc);  // generally, speed_filter_fc approx to speed_calc freq divided by 5 



GMP_STATIC_INLINE
void ctl_step_spd_calc(ctl_spd_calculator_t* sc, ctrl_gt position_rad)
{
	ctrl_gt CTRL_PI = GMP_CONST_PI;
	ctrl_gt CTRL_2PI = GMP_CONST_2_PI;

	if (ctl_step_divider(&sc->div))
	{

		sc->old_position = sc->position;
		sc->position = position_rad;

		ctrl_gt delta = sc->position - sc->old_position;
		// direction correction
		if (delta < -GMP_CONST_1_OVER_2)
		{
			delta += GMP_CONST_1;
		}
		else if(delta > GMP_CONST_1_OVER_2)
		{
			delta -= GMP_CONST_1;
		}

		ctrl_gt new_spd = ctl_mul((delta), sc->scale_factor);
				
		sc->speed = ctl_step_lowpass_filter(&sc->spd_filter, new_spd);
		// sc->speed = sc->spd_filter.out;
		
	}
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_spd_via_pos_encoder(ctl_spd_calculator_t* sc)
{
	return sc->speed;
}


//GMP_STATIC_INLINE
//void ctl_enable_spd_calc(ctl_spd_calculator_t* sc)
//{
//	sc->div.flag_bypass = 0;
//}
//
//GMP_STATIC_INLINE
//void ctl_disable_spd_calc(ctl_spd_calculator_t* sc)
//{
//	sc->div.flag_bypass = 1;
//}

// input variables
// absolute position encoder
// QEP encoder
// Hall encoder
// Observer
// sin/cos encoder

// general header for encoder
//typedef struct _tag_gmp_encoder_t
//{
//	// output variables
//	gmp_math_t mech_ang; // p.u.
//	int32_t revolution;  // turns
//	gmp_math_t elec_ang; // p.u.
//
//	// derived variables
//	gmp_math_t speed;    // unit: krpm
//	
//	// intrinsic variables
//	uint16_t pole_pairs;
//	gmp_math_t mech_ang_offset; // offset of mechanical angle, p.u.
//
//}gmp_enc_t;
//
//// QEP encoder
//typedef struct _tag_gmp_qep_encoder_t
//{
//	// general header
//	gmp_enc_t header;
//
//	// input variables
////	uint32_t cnt;
//
//	// intermediate variables
//	uint32_t cnt_last;
//	
//	// intrinsic variables
//	uint32_t lines;        // encoder lines
//	uint32_t division;     // division factor for control frequency
//
//}gmp_qep_encoder_t;
//
//// Tamagawa TS5700N8401 series absolute position encoder 
//typedef struct _tag_gmp_tamagawa_encoder_t
//{
//	// general header
//	gmp_enc_t header;
//
//	// input variables
//
//
//}gmp_tamagawa_encoder_t;

//////////////////////////////////////////////////////////////////////////
// Position Encoder calibrate

typedef struct _tag_position_enc_calibrate
{
	// target motor current controller
	ctl_motor_current_ctrl_t *mc;

	// target motor encoder
	ctl_rotation_encoder_t *encoder;

	// parameters:

	// when running position calibrate task,
	// this current may treated as the target
	ctrl_gt current_target;

	// when current exceed the limit current
	// the controller output would disable
	ctrl_gt current_limit;

	// target delta
	ctrl_gt position_delta_target;

	// old motor position
	ctrl_gt old_position;

	fast_gt flag_position_convergence;

	flag_gt flag_stage1;

	time_gt switch_time;

	ctrl_gt offset;
}ctl_position_enc_calibrate_t;

ec_gt ctl_task_position_encoder_offset_calibrate(
	ctl_position_enc_calibrate_t* obj
)
{
	// Step I: set current target & voltage target
	ctl_set_motor_current_ctrl_idq_ref(&obj->mc, obj->current_target,0);
	ctl_set_motor_current_ctrl_vdq_ff(&obj->mc, 0,0);

	// TODO: Safe routine
	// judge if current is over current limit

	// Step II: wait till encoder output is stable.
	// encoder output delta is less than position delta target (default is 0.5%), 
	// set `flag_position_convergence = 1`
	if((obj->flag_stage1 == 0))
	{
		// judge position convergency
		if(ctl_get_encoder_position(obj->encoder) - obj->old_position < obj->position_delta_target)
		{
			obj->flag_position_convergence = obj->flag_position_convergence << 1 | 0x00;
		}
		else // position still not convergency
		{
			obj->flag_position_convergence = obj->flag_position_convergence << 1 | 0x01;
		}

		// save old position
		obj->old_position = ctl_get_encoder_position(obj->encoder);

		// judge position convergency sequence is full 
		if(obj->flag_position_convergence & 0x0F == 0)
		{
			obj->switch_time = gmp_base_get_system_tick();
			obj->flag_stage1 = 1;
		}

		// Still running
		return GMP_EC_OK;
	}

	// Step III: wait to infinity
	// after output delta is satisfied the target, wait another 1 second. 
	else
	{
		if(gmp_base_get_system_tick() - obj->switch_time)
		{
			// offset
			ctrl_gt offset = obj->encoder->offset + ctl_get_encoder_position(obj->encoder);
			offset = ctl_sat_1(offset);

			// save valibrate result
			obj->encoder->offset = offset;

			// complete task
			return 1;
		}
	}

}

ec_gt ctl_clear_position_encoder_calibrator(ctl_position_enc_calibrate_t* obj)
{
	obj->old_position = 0;
	obj->flag_position_convergence = ~ 0;
	obj->flag_stage1 = 0;
	
	ctl_set_motor_current_ctrl_idq_ref(&obj->mc, 0,0);
	ctl_set_motor_current_ctrl_vdq_ff(&obj->mc, 0,0);
}


#endif // _FILE_ENCODER_H_

#ifdef __cplusplus
}
#endif // __cplusplus
