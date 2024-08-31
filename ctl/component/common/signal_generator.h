

#ifndef _FILE_SIGNAL_GENERATOR_H_
#define _FILE_SIGNAL_GENERATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//////////////////////////////////////////////////////////////////////////
	// Sine & Cosine Wave generator
	typedef struct _tag_sine_generator_t
	{
		// frequency
		ctrl_gt ph_sin_delta;
		ctrl_gt ph_cos_delta;

		// current phasor
		ctrl_gt ph_sin;
		ctrl_gt ph_cos;

	}ctl_src_sg_t;

	void init_sincos_gen(ctl_src_sg_t* sg);

	void setup_sincos_gen(ctl_src_sg_t* sg,
		ctrl_gt init_angle, // rad
		ctrl_gt step_angle); // rad


	GMP_STATIC_INLINE
		void sincos_step(ctl_src_sg_t* sg)
	{
		ctrl_gt sin_new = sg->ph_sin * sg->ph_cos_delta + sg->ph_cos * sg->ph_sin_delta;
		ctrl_gt cos_new = sg->ph_cos * sg->ph_cos_delta - sg->ph_sin * sg->ph_sin_delta;
	}


	//////////////////////////////////////////////////////////////////////////
	// Ramp wave generator
	typedef struct _tag_ramp_generator_t
	{
		// output range
		ctrl_gt minimum;
		ctrl_gt maximum;

		// current output
		ctrl_gt current;

		// ramp slope
		ctrl_gt slope;
	}ctl_src_rg_t;


	void init_ramp_gen(ctl_src_rg_t* rg);

	void setup_ramp_gen(ctl_src_rg_t* rg, ctrl_gt slope);
	void setup_ramp_gen_with_amp_freq(ctl_src_rg_t* rg,
		parameter_gt isr_freq, parameter_gt target_freq,
		parameter_gt amp_pos, parameter_gt amp_neg);

	GMP_STATIC_INLINE
		void ramp_step(ctl_src_rg_t* rg)
	{
		rg->current += rg->slope;
		
		if (rg->current > rg->maximum)
			rg->current = rg->minimum;
	}




#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_SIGNAL_GENERATOR_H_
