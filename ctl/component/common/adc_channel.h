
#ifndef _FILE_ADC_CHANNEL_H_
#define _FILE_ADC_CHANNEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_adc_channel
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
void init_adc_channel(adc_channel_t* adc_obj);

void setup_adc_channel(adc_channel_t* adc_obj,
	ctrl_gt gain,
	ctrl_gt bias,
	fast_gt resolution,
	fast_gt iqn
);

// transfer raw data to final value
void calc_adc_channel(adc_channel_t* adc_obj);

#ifdef __cplusplus
}
#endif //__cplusplus


#endif // _FILE_ADC_CHANNEL_H_
