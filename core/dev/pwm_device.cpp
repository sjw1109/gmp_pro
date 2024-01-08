// This file provide a set of PWM write function prototype

#include <core/gmp_core.hpp>
#include <ctrl/ctrl_config.h>

#include <core/dev/pwm_device.h>

//void init_pwm_device(pwm_dev_t* dev, pwm_dev_handle_t handle, uint32_t peripheral_freq, uint32_t pwm_freq)
//{
//	// check parameter
//	assert(dev);
//	assert(handle);

//	dev->handle = handle;

//	disable_pwm(dev->handle);

//	dev->peripheral_freq = peripheral_freq;
//	dev->pwm_full_scale = (uint32_t)(peripheral_freq / pwm_freq);
//	gmp_write_pwm_div(dev->handle, dev->pwm_full_scale);
//}
