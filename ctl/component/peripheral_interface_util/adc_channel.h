
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
