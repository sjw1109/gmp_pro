/**
 * @file dac_channel.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
#ifndef _FILE_DAC_CHANNEL_H_
#define _FILE_DAC_CHANNEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


	typedef struct _tag_dac_channel_t
	{
		// input DAC output target value
		ctrl_gt raw;

		// resolution of DAC
		fast_gt resolution;

		// gain of DAC output
		ctrl_gt gain;

		// bias of DAC output
		ctrl_gt bias;

		// DAC output value
		// value = raw * gain + bias;
		dac_gt value;

	}dac_channel_t;


#ifdef __cplusplus
}
#endif

#endif // _FILE_DAC_CHANNEL_H_
