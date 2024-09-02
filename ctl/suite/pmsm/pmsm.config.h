

// common modules
#include <ctl/component/common/track_pid.h>


// interfaces



#ifndef _PMSM_CONFIG_H_
#define _PMSM_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//////////////////////////////////////////////////////////////////////////
	// Here's the default PMSM motor parameters
	// These parameters are given by macros
#define ADC_CURRENT_GAIN 20
#define ADC_CURRENT_BIAS 10
#define ADC_VOLTAGE_GAIN 20
#define ADC_VOLTAGE_BIAS 10
#define ADC_RESOLUTION 16

#define PWM_MODULATION_HALF_CYCLE ((10000))



//////////////////////////////////////////////////////////////////////////
// Here's the PMSM controller template








#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _PMSM_CONFIG_H_

