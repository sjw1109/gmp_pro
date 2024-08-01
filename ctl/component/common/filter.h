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


	// 2rd order IIR Filter: reference https://www.jianshu.com/p/e818028764f7

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
	}filter_IIR2_t;

	static inline void ctl_filter_iir2_calc(filter_IIR2_t* obj, ctrl_gt input)
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
	void clear_filter(filter_IIR2_t* obj);

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

	}filter_IIR2_setup_t;

	void ctl_init_filter_iir2(filter_IIR2_t* obj);

	// Design a 2rd Order IIR filter based on center frequency and Q
	void ctl_setup_filter_iir2(filter_IIR2_t* obj, filter_IIR2_setup_t* setup_obj);
	


#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_FILTER_H_

