#ifndef _FILE_ENCODER_H_
#define _FILE_ENCODER_H_

// Two parts
// Speed Encoder
// Position Encoder

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

}ctl_pos_encoder_t;


// input functions should be called in input stage
GMP_STATIC_INLINE
void ctl_input_pos_encoder(ctl_pos_encoder_t* pos_encoder)
{

}

GMP_STATIC_INLINE
ctrl_gt ctl_get_elec_angle_via_pos_encoder(ctl_pos_encoder_t* pos_encoder)
{
	ctrl_gt elec_pos = pos_encoder->poles * (pos_encoder->position + pos_encoder->offset);
	
	return elec_pos;
}

void ctl_init_pos_encoder(ctl_pos_encoder_t* pos_encoder);

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


typedef struct _tag_speed_calculator_t
{
	// position encoder as data source
	ctl_pos_encoder_t* pos_encoder;

	// speed encoder as data output
	ctl_spd_encoder_t* spd_encoder;

	// transfer constants

}ctl_spd_calculator_t;

void ctl_init_spd_calculator(ctl_spd_calculator_t* sc)
{
	
}

void ctl_setup_spd_calculator(ctl_spd_calculator_t* sc,
	ctl_pos_encoder_t* pos_encoder, ctl_spd_encoder_t* spd_encoder)
{
	sc->pos_encoder = pos_encoder;
	sc->spd_encoder = spd_encoder;
}


GMP_STATIC_INLINE
void ctl_calc_spd_via_pos_encoder(spd_calculator_t *sc)
{

}


// input variables
// absolute position encoder
// QEP encoder
// Hall encoder
// Observer
// sin/cos encoder

// general header for encoder
typedef struct _tag_gmp_encoder_t
{
	// output variables
	gmp_math_t mech_ang; // p.u.
	int32_t revolution;  // turns
	gmp_math_t elec_ang; // p.u.

	// derived variables
	gmp_math_t speed;    // unit: krpm
	
	// intrinsic variables
	uint16_t pole_pairs;
	gmp_math_t mech_ang_offset; // offset of mechanical angle, p.u.

}gmp_enc_t;

// QEP encoder
typedef struct _tag_gmp_qep_encoder_t
{
	// general header
	gmp_enc_t header;

	// input variables
//	uint32_t cnt;

	// intermediate variables
	uint32_t cnt_last;
	
	// intrinsic variables
	uint32_t lines;        // encoder lines
	uint32_t division;     // division factor for control frequency

}gmp_qep_encoder_t;

// Tamagawa TS5700N8401 series absolute position encoder 
typedef struct _tag_gmp_tamagawa_encoder_t
{
	// general header
	gmp_enc_t header;

	// input variables


}gmp_tamagawa_encoder_t;

#endif // _FILE_ENCODER_H_
