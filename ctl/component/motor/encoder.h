

// based on filter module
#include <ctl/component/common/filter.h>


#ifndef _FILE_ENCODER_H_
#define _FILE_ENCODER_H_

// Two parts
// Speed Encoder
// Position Encoder

//////////////////////////////////////////////////////////////////////////
// position encoder
typedef struct _tag_ctl_pos_encoder_t
{
	// angle position, rad 0-2*pi
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

	// angle position, rad 0-2*pi
	ctrl_gt elec_angle;

}ctl_pos_encoder_t;


void ctl_init_pos_encoder(ctl_pos_encoder_t* pos_encoder);

void ctl_setup_pos_encoder(ctl_pos_encoder_t* enc, uint16_t poles, uint32_t position_base);


// input functions should be called in input stage
GMP_STATIC_INLINE
void ctl_input_pos_encoder(ctl_pos_encoder_t* pos_encoder, uint32_t raw)
{
	pos_encoder->position = ctrl_div(raw, pos_encoder->position_base);
}

GMP_STATIC_INLINE
void ctl_input_pos_encoder_shift(ctl_pos_encoder_t* pos_encoder,
	uint32_t raw, fast_gt shift)
{
	pos_encoder->position = raw << shift;
}


// GET ANGLE, unit rad
GMP_STATIC_INLINE
ctrl_gt ctl_get_elec_angle_via_pos_encoder(ctl_pos_encoder_t* pos_encoder)
{
	ctrl_gt elec_pos = pos_encoder->poles * (pos_encoder->position + GMP_CONST_1 - pos_encoder->offset);

	elec_pos = ctrl_mod_1(elec_pos);

	ctrl_gt angle = ctrl_mpy(elec_pos, GMP_CONST_2_PI);

	pos_encoder->elec_angle = angle;

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
	// lap counter
	int32_t turns;

	// angle position, rad 0-2*pi
	// Mechanical position
	ctrl_gt position;

	// offset,
	// position + offset means the true position
	ctrl_gt offset;

	// pole_pairs
	// poles*(position + offset) = Electrical position
	uint16_t poles;

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
		if (delta < -CTRL_PI)
		{
			delta += CTRL_2PI;
		}
		else if(delta > CTRL_PI)
		{
			delta -= CTRL_2PI;
		}

		ctrl_gt new_spd = ctrl_mpy((delta), sc->scale_factor);
				
		sc->speed = ctl_step_lowpass_filter(&sc->spd_filter, new_spd);
		// sc->speed = sc->spd_filter.out;
		
	}
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_spd_via_pos_encoder(ctl_spd_calculator_t* sc)
{
	return sc->speed;
}


GMP_STATIC_INLINE
void ctl_enable_spd_calc(ctl_spd_calculator_t* sc)
{
	sc->div.flag_bypass = 0;
}

GMP_STATIC_INLINE
void ctl_disable_spd_calc(ctl_spd_calculator_t* sc)
{
	sc->div.flag_bypass = 1;
}

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

#endif // _FILE_ENCODER_H_
