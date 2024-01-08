#ifndef _FILE_ENCODER_H_
#define _FILE_ENCODER_H_

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
