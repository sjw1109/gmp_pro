// Two types of PWM device is defined in this file.
#include <stdint.h>


#ifndef _FILE_PWM_DEVICE_H_
#define _FILE_PWM_DEVICE_H_

// PWM channel default flag
#ifndef GMP_PWM_CHN_1
#define GMP_PWM_CHN_1 TIM_CHANNEL_1
#endif // GMP_PWM_CHN_1

#ifndef GMP_PWM_CHN_2
#define GMP_PWM_CHN_2 TIM_CHANNEL_2
#endif // GMP_PWM_CHN_2

#ifndef GMP_PWM_CHN_3
#define GMP_PWM_CHN_3 TIM_CHANNEL_3
#endif // GMP_PWM_CHN_3

#ifndef GMP_PWM_CHN_4
#define GMP_PWM_CHN_4 TIM_CHANNEL_4
#endif // GMP_PWM_CHN_4

#ifndef GMP_PWM_CHN_5
#define GMP_PWM_CHN_5 TIM_CHANNEL_5
#endif // GMP_PWM_CHN_5

#ifndef GMP_PWM_CHN_6
#define GMP_PWM_CHN_6 TIM_CHANNEL_6
#endif // GMP_PWM_CHN_6

// function redirection
	// calculate PWM compare value based on division 
#ifndef GMP_CALC_PWM_CMP_FUNC 
#define GMP_CALC_PWM_CMP_FUNC(PWM_OBJ, TARGET_SCL) \
	 gmp_get_pwm_cmp((PWM_OBJ), (TARGET_SCL))
#endif // GMP_CALC_PWM_CMP_FUNC

	// calculate PWM division coefficient based on peripheral frequency
#ifndef GMP_CALC_PWM_DIV_FUNC
#define GMP_CALC_PWM_DIF_FUNC(PWM_OBJ, PERIPHERAL_FREQ) \
	gmp_get_pwm_div((PWM_OBJ),(PERIPHERAL_FREQ))
#endif // GMP_CALC_PWM_DIV_FUNC


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	// handle of PWM peripheral
#ifndef GMP_PWM_PERIPHERAL_HANDLE_TYPE
	typedef void* pwm_dev_handle_t;
#else
	typedef GMP_PWM_PERIPHERAL_HANDLE_TYPE pwm_dev_handle_t;
#endif // GMP_PWM_PERIPHERAL_HANDLE_TYPE

	typedef struct _tag_pwm_dev
	{
		pwm_dev_handle_t handle;		// Handle for the PWM device

		uint32_t peripheral_freq;	    // Unit Hz, the kernel frequency for chip
		uint32_t pwm_full_scale;		// tick, how many ticks does one overflow event need

		uint32_t dead_time;				// Unit ns, deadtime for switching

	}pwm_dev_t;

	// trans_init a PWM device
	void init_pwm_device(pwm_dev_t* dev, pwm_dev_handle_t handle, uint32_t peripheral_freq, uint32_t pwm_freq);

	// Auxiliary functions
	// calculate compare value based on timer scaler
	inline uint32_t gmp_get_pwm_cmp(pwm_dev_t* pwm, gmp_math_t target_cmp)
	{
		// This function should be re- implemented by specified chip
		return (uint32_t)(target_cmp * pwm->pwm_full_scale);
	}

	// calculate division value based on timer peripheral frequency
	inline uint32_t gmp_get_pwm_div(pwm_dev_t* pwm, gmp_math_t target_freq)
	{
		return (uint32_t)(pwm->peripheral_freq / target_freq);
	}

	// calculate deadtime tick based on PWM device,
	// target deadtime has unit ns.
	inline uint32_t gmp_get_dead_time(pwm_dev_t* pwm, uint32_t target_deadtime)
	{
		pwm->dead_time = target_deadtime;
		return (target_deadtime / 100) * (pwm->peripheral_freq / 10000000UL);
	}
	

	//////////////////////////////////////////////////////////////////////////
	// The following function should be implemented as high efficient and fast as possible.

	// write PWM compare coefficient
//#ifndef gmp_write_pwm_cmp
//	// This function should be implement by user
//#define gmp_write_pwm_cmp(_dev_, _channel_, _cmp_) 
//#endif
//
//#ifndef gmp_write_pwm_div
//	// This function should be implement by user
//#define gmp_write_pwm_div(_dev_, _div_)
//#endif

	// enable PWM output
	void enable_pwm(pwm_dev_handle_t dev);

	// disable PWM output
	void disable_pwm(pwm_dev_handle_t dev);






#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PWM_DEVICE_H_

