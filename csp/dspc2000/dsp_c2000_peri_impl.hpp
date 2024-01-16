
// base classes headers
#include <core/dev/peripheral_driver.hpp>


// User must config the compiler to satisfy the C11 standard
// Properties for the project, you should follow the prompt
// - Build
// - - C2000 Compiler
// - - - Advanced Options
// - - - - Language Options
// And you should pay attention to 
// - C Dialect   : Compile program in C99 mode (--c99)
// - C++ Dialect : Compile program in C++03 mode (--c++03)


#ifndef _FILE_C2000_PERIPHERAL_IMPLEMENT_HPP_
#define _FILE_C2000_PERIPHERAL_IMPLEMENT_HPP_

typedef struct _tag_dsp_c2000_gpio_dat_t
{
	uint32_t data;
	uint32_t set;
	uint32_t clear;
	uint32_t toggle;
}gpio_dat_t;

#ifndef GPIO_MAX_NUMBER
#define GPIO_MAX_NUMBER 32
#endif // GPIO_MAX_NUMBER

// This is the only entry point of GPIO Data Register pointer.
gpio_dat_t* gpio_dat_handle = &GpioDataRegs;

// This is the GPIO implementation of C2000 series DSP
class gmp_gpio_dspc2000_impl_t
	:public gmp_gpio_entity
{
public:
	typedef gmp_data_t data_type;

public:
	// ctor & dtor
	gmp_gpio_dspc2000_impl_t()
		:gpio_mux(0), gpio_pin(0)
	{
	}

	gmp_gpio_dspc2000_impl_t(uint16_t gpio_index)
	{
		assert(gpio_index < GPIO_MAX_NUMBER);

		gpio_mux = gpio_pin / GPIO_MAX_NUMBER;
		gpio_pin = gpio_pin % GPIO_MAX_NUMBER;
	}

public:
	void set_gpio_pin(uint16_t gpio_index)
	{
		assert(gpio_index < GPIO_MAX_NUMBER);

		gpio_mux = gpio_pin / GPIO_MAX_NUMBER;
		gpio_pin = gpio_pin % GPIO_MAX_NUMBER;
	}

	// utility function
	void set() override
	{
		ENABLE_PROTECTED_REGISTER_WRITE_MODE;
		
		gpio_dat_handle[gpio_mux].set = (uint32_t)1 << gpio_pin;
		
		DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	}
	void clear() override
	{
		ENABLE_PROTECTED_REGISTER_WRITE_MODE;

		gpio_dat_handle[gpio_mux].clear = (uint32_t)1 << gpio_pin;

		DISABLE_PROTECTED_REGISTER_WRITE_MODE;

	}
	void toggle() override
	{
		ENABLE_PROTECTED_REGISTER_WRITE_MODE;

		gpio_dat_handle[gpio_mux].toggle = (uint32_t)1 << gpio_pin;

		DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	}

	// Write and Read function
	gmp_size_t write(const data_type data) override
	{
		ENABLE_PROTECTED_REGISTER_WRITE_MODE;

		gpio_dat_handle[gpio_mux].toggle = (uint32_t)1 << gpio_pin;

		DISABLE_PROTECTED_REGISTER_WRITE_MODE;
	}

	data_type read() override
	{
		data_type gpio_status = (*gpio_port) & gpio_mask;
		if (gpio_status == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}


	// The following function is determined by chips, not implement
	void set_mode(uint8_t mode)override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}


public:
	uint16_t gpio_mux;
	uint16_t gpio_pin;

};


#endif // _FILE_C2000_PERIPHERAL_IMPLEMENT_HPP_

