
#ifndef _FILE_ADC_CHANNEL_H_
#define _FILE_ADC_CHANNEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef struct _tag_adc_channel_t
	{
		// INPUT raw data from adc
		adc_gt raw;

		// resolution of ADC bits
		fast_gt resolution;
		// The result of ADC is IQn number
		fast_gt iqn;

		// The bias of the ADC data
		ctrl_gt bias;
		// The gain of the ADC data
		ctrl_gt gain;

		// OUTPUT the ADC data in ctrl_gt type
		// Formal expression: value = gain * (raw - gain);
		ctrl_gt value;
	}adc_channel_t;


	// init object function 
	void ctl_init_adc_channel(adc_channel_t* adc_obj);

	void ctl_setup_adc_channel(adc_channel_t* adc_obj,
		ctrl_gt gain,
		ctrl_gt bias,
		fast_gt resolution,
		fast_gt iqn
	);

	// transfer raw data to final value
	GMP_STATIC_INLINE
		void ctl_step_adc_channel(adc_channel_t* adc_obj)
	{
		// transfer ADC data to _IQ24
		ctrl_gt raw_data = adc_obj->raw << (GLOBAL_Q - adc_obj->iqn);

		// remove bias
		ctrl_gt raw_without_bias = raw_data - adc_obj->bias;

		// Gain
		adc_obj->value = ctrl_mpy(raw_without_bias, adc_obj->gain);

		return;
	}

	// input process
	GMP_STATIC_INLINE
		void ctl_input_adc_source_data(adc_channel_t* adc_obj, adc_gt raw)
	{
		adc_obj->raw = raw;
	}

	// get result
	GMP_STATIC_INLINE
		ctrl_gt ctl_get_adc_data(adc_channel_t* adc)
	{
		return adc->value;
	}

	//////////////////////////////////////////////////
	// Dual channel ADC

	// This is a dual channel ADC peripheral interface
	// generally, user may use it to sample two channel analog input.
	// or use it as a differential pair.
	typedef struct _tag_adc_dual_channel_t
	{
		// INPUT raw data from adc
		adc_gt raw[2];

		// resolution of ADC bits
		fast_gt resolution;
		// The result of ADC is IQn number
		fast_gt iqn;

		// The bias of the ADC data
		ctrl_gt bias[2];
		// The gain of the ADC data
		ctrl_gt gain[2];

		// OUTPUT the ADC data in ctrl_gt type
		// Formal expression: value = gain * (raw - gain);
		ctrl_gt value[2];
	}adc_dual_channel_t;

	void ctl_init_adc_dual_channel(adc_dual_channel_t* adc);

	void ctl_setup_adc_dual_channel(adc_dual_channel_t* adc,
		ctrl_gt gain,
		ctrl_gt bias,
		fast_gt resolution,
		fast_gt iqn
	);

	GMP_STATIC_INLINE
		void ctl_input_adc_dual_channel_raw_data(adc_dual_channel_t* adc_obj, adc_gt raw1, adc_gt raw2)
	{
		adc_obj->raw[0] = raw1;
		adc_obj->raw[1] = raw2;
	}

	GMP_STATIC_INLINE
		void ctl_step_adc_dual_channel(adc_dual_channel_t* adc)
	{
		ctrl_gt raw_data;
		ctrl_gt raw_without_bias;
		int i = 0;
		
		for (i = 0; i < 2; ++i)
		{
			// transfer ADC data to _IQ24
			raw_data = adc->raw[i] << (GLOBAL_Q - adc->iqn);

			// remove bias
			raw_without_bias = raw_data - adc->bias;

			// Gain
			adc_obj->value[i] = ctrl_mpy(raw_without_bias, adc->gain);
		}
		return;

	}

	GMP_STATIC_INLINE
		void ctl_get_adc_dual_channel(adc_dual_channel_t* adc,
			ctrl_gt* dat1, ctrl_gt dat2)
	{
		*dat1 = adc->value[0];
		*dat2 = adc->value[1];
	}

	GMP_STATIC_INLINE
		ctrl_gt ctl_get_adc_dual_channel_ch1(adc_dual_channel_t* adc)
	{
		return adc->value[0];
	}

	GMP_STATIC_INLINE
		ctrl_gt ctl_get_adc_dual_channel_ch2(adc_dual_channel_t* adc)
	{
		return adc->value[1];
	}

	// positive - negative
	GMP_STATIC_INLINE
		ctrl_gt ctl_get_adc_dual_channel_diff(adc_dual_channel_t* adc)
	{
		return adc->value[0] - adc->value[1];
	}

	GMP_STATIC_INLINE
		ctrl_gt ctl_get_adc_dual_channel_comm(adc_dual_channel_t* adc)
	{
		return ctrl_div2(adc->value[0] + adc->value[1]);
	}

	GMP_STATIC_INLINE
		void ctl_get_adc_dual_channel_via_vector2(adc_dual_channel_t* adc,
			ctl_vector2_t *vector)
	{
		vector->dat[0] = adc->value[0];
		vector->dat[1] = adc->value[1];
	}

	//////////////////////////////////////////////////////////////////////////
	// ADC tri channel

	// This module is for three-phase system
	// Such as motor controller and power grid applicaion
	typedef struct _tag_adc_tri_channel_t
	{
		// INPUT raw data from adc
		adc_gt raw[3];

		// resolution of ADC bits
		fast_gt resolution;
		// The result of ADC is IQn number
		fast_gt iqn;

		// The bias of the ADC data
		ctrl_gt bias[3];
		// The gain of the ADC data
		ctrl_gt gain[3];

		// OUTPUT the ADC data in ctrl_gt type
		// Formal expression: value = gain * (raw - gain);
		ctrl_gt value[3];
	}adc_tri_channel_t;

	void ctl_init_adc_tri_channel(adc_tri_channel_t* adc);

	void ctl_setup_adc_tri_channel(adc_tri_channel_t* adc,
		ctrl_gt gain,
		ctrl_gt bias,
		fast_gt resolution,
		fast_gt iqn
	);

	GMP_STATIC_INLINE
		void ctl_input_adc_tri_channel_raw_data(adc_tri_channel_t* adc_obj,
			adc_gt raw1, adc_gt raw2, adc_gt raw3)
	{
		adc_obj->raw[0] = raw1;
		adc_obj->raw[1] = raw2;
		adc_obj->raw[2] = raw3;
	}

	GMP_STATIC_INLINE
		void ctl_step_adc_tri_channel(adc_tri_channel_t* adc)
	{
		ctrl_gt raw_data;
		ctrl_gt raw_without_bias;
		int i = 0;

		for (i = 0; i < 3; ++i)
		{
			// transfer ADC data to _IQ24
			raw_data = adc->raw[i] << (GLOBAL_Q - adc->iqn);

			// remove bias
			raw_without_bias = raw_data - adc->bias;

			// Gain
			adc_obj->value[i] = ctrl_mpy(raw_without_bias, adc->gain);
		}
		return;

	}

	GMP_STATIC_INLINE
		void ctl_get_adc_tri_channel_via_vector3(adc_tri_channel_t* adc,
			ctl_vector3_t* vec)
	{
		vec->dat[0] = adc->value[0];
		vec->dat[1] = adc->value[1];
		vec->dat[2] = adc->value[2];
	}


	//////////////////////////////////////////////////////////////////////////
	// ADC bias calibrator

#include <ctl/component/common/filter.h>

	typedef struct _tag_adc_bias_calibrator_t
	{
		// OUTPUT ADC bias data
		ctrl_gt bias_output;

		// output flag
		fast_gt flag_output_valid;

		// INPUT ADC RAW data
		ctrl_gt raw;

		// tools filter for RAW data.
		filter_IIR2_t filter;

		// total observation period
		uint32_t total_period;

		// current period
		uint32_t current_period;

		// start tick
		// start period == 0 means need a brand new beginning.
		uint32_t start_period;

		// filter tick
		uint32_t filter_tick;

	}adc_bias_calibrator_t;

	void ctl_init_adc_bias_calibrator(adc_bias_calibrator_t* obj);

	void ctl_setup_adc_bias_calibrator(adc_bias_calibrator_t* obj,
		filter_IIR2_setup_t* filter_parameter);

	void ctl_restart_adc_bias_calibrator(adc_bias_calibrator_t* obj);

	// return value means if the calibration output is valid
	fast_gt ctl_step_adc_bias_calibrator(
		adc_bias_calibrator_t* obj,
		uint32_t main_isr_tick,
		ctrl_gt adc_value);



#ifdef __cplusplus
}
#endif //__cplusplus


#endif // _FILE_ADC_CHANNEL_H_
