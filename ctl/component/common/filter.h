#ifndef _FILE_FILTER_H_
#define _FILE_FILTER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	// necessary constant parameters
#ifndef PI
#define PI ((3.1415926f))
#endif // PI

#ifndef EXP
#define EXP ((2.718281828f))
#endif // EXP


	// 1rd order lowpass IIR filter
	// Z transfer function:
	//tex: $R(s) = \frac{\omega_c}{s+\omega_c},R(z) = \frac{1-e^{-\omega_c T_s}}{1-e^{-\omega_c T_s}\cdot z^{-1}}$

	// where, 
	//tex: $1-e^{-\omega_c T_s}\approx \omega_c*T_s$

	// So, 
	//tex: $Y = a\cdot X(n) + (1-a) Y(n-1)$
	typedef struct _tag_low_pass_filter_t
	{
		// parameters
		ctrl_gt a;

		// as a container of last point
		ctrl_gt out;
	}ctl_low_pass_filter_t;

	void ctl_init_lp_filter(ctl_low_pass_filter_t* lpf);

	void ctl_setup_lp_filter(ctl_low_pass_filter_t* lpf,
		parameter_gt fs,  // Sample rate
		parameter_gt fc); // cut frequency


	GMP_STATIC_INLINE
		ctrl_gt ctl_step_lopass_filter(ctl_low_pass_filter_t* lpf, ctrl_gt input)
	{
		ctrl_gt new_out;

		new_out = ctrl_mpy(input, lpf->a) + ctrl_mpy(lpf->out, CTRL_T(1.0) - lpf->a);

		lpf->out = new_out;

		return new_out;
	}



	// 2rd order IIR Filter

	// Z transfer function:
	//tex:
	// $$ H(z) = \frac{b_0 + b_1z^{-1} + b_2z^{-2}}{a_0 + a_1z^{-1} + a_2z^{-2}}$$

	// Discrete expression:
	//tex:
	// $$ y(n) = \frac{b_0}{a_0} x(n) + \frac{b_1}{a_0} x(n-1) + \frac{b_2}{a_0}x(n-2) 
	// -\frac{a_1}{a_0}y(n-1) - \frac{a_2}{a_0}y(n-2)$$

	typedef struct _tag_filter_IIR2_t
	{
		// parameters
		// x[0] = x(n-1); x[1] = x(n-2)
		ctrl_gt x[2];
		// y[0] = y(n-1); y[1] = y(n-2)
		ctrl_gt y[2];

		// filter coefficient
		// a[0] = a_1/a_0; a[1] = a_2/a_0;
		ctrl_gt a[2];
		// b[0] = b_0/a_0; b[1] = b_1/a_0; b[2] = b_2/a_0;
		ctrl_gt b[3];

		// output 
		ctrl_gt out;
	}ctl_filter_IIR2_t;

	GMP_STATIC_INLINE
		void ctl_step_filter_iir2(ctl_filter_IIR2_t* obj, ctrl_gt input)
	{
		obj->out = ctrl_mpy(obj->b[0], input)
			+ ctrl_mpy(obj->b[1], obj->x[0]) + ctrl_mpy(obj->b[2], obj->x[1])
			- ctrl_mpy(obj->a[0], obj->y[0]) - ctrl_mpy(obj->a[1], obj->y[1]);

		obj->x[1] = obj->x[0];
		obj->x[0] = input;

		obj->y[1] = obj->y[0];
		obj->y[0] = obj->out;
	}

	// clear all the intermediate variables
	void ctl_clear_filter(ctl_filter_IIR2_t* obj);

	typedef enum _tag_filter_IIR2_type_t
	{
		FILTER_IIR2_TYPE_LOWPASS = 0,
		FILTER_IIR2_TYPE_HIGHPASS = 1,
		FILTER_IIR2_TYPE_BANDPASS = 2
	}filter_IIR2_type_t;

	typedef struct _tag_filter_IIR2_setup_t
	{
		// filter type
		filter_IIR2_type_t filter_type;

		// cut frequency
		parameter_gt fc;

		// sample frequency
		parameter_gt fs;

		// quality factor
		parameter_gt q;

		// gain 
		parameter_gt gain;

	}ctl_filter_IIR2_setup_t;

	void ctl_init_filter_iir2(ctl_filter_IIR2_t* obj);

	// Design a 2rd Order IIR filter based on center frequency and Q
	void ctl_setup_filter_iir2(ctl_filter_IIR2_t* obj, ctl_filter_IIR2_setup_t* setup_obj);



#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_FILTER_H_

