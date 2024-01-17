
// base classes headers
#include <core/dev/peripheral_driver.hpp>


#ifndef _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_
#define _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_

#if defined HAL_GPIO_MODULE_ENABLED

//#define GMP_PORT_HGPIO_T GPIO_HandleTypeDef*

class gmp_gpio_stm32_impl_t
	: public gmp_gpio_entity
{
public:
	typedef gmp_data_t data_type;

public:
	// ctor & dtor
	gmp_gpio_stm32_impl_t(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
		:gpio_group(GPIOx), pin_index(GPIO_Pin)
	{}

public:
	// utility function
	void set() override;
	void clear() override;
	void toggle() override;

	// Write and Read function
	gmp_size_t write(data_type data) override;
	data_type read() override;

	// The following function is determined by chips, not implement
	void set_mode(uint8_t mode)override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}


public:
	GPIO_TypeDef* gpio_group;
	uint16_t pin_index;
};

#endif // HAL_GPIO_MODULE_ENABLED

#if defined HAL_UART_MODULE_ENABLED

// Specify the default UART type
#define GMP_PORT_HUART_T UART_HandleTypeDef*

// implement the uart entity
class gmp_uart_stm32_impl_t
	:public gmp_uart_entity
{
public:
	typedef gmp_addr_t addr_type;
	typedef gmp_data_t data_type;

public:
	// ctor & dtor
	gmp_uart_stm32_impl_t(UART_HandleTypeDef* handle)
		:handle(handle)
	{}

public:
	void set_baudrate(uint32_t BaudRate) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_stop_bit(uint8_t stopbit) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_data_bit(uint8_t databit) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_parity(uint8_t parity_check) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

public:
	// Basic IO functions is defined here.
	gmp_size_t read(gmp_data_t* data, gmp_size_t length) override;

	gmp_size_t write(data_type* data, gmp_size_t length) override;

	gmp_size_t write(const gmp_data_t* data, gmp_size_t length) override;

	gmp_data_t read() override;

	gmp_size_t write(gmp_data_t data) override;

protected:
	UART_HandleTypeDef* handle;

};

#endif // HAL_UART_MODULE_ENABLED

#if defined HAL_I2C_MODULE_ENABLED

// IIC handle
#define GMP_PORT_HIIC_T I2C_HandleTypeDef*

class gmp_iic_stm32_impl_t
	:public gmp_iic_entity
{
public:
	typedef gmp_addr_t addr_type;
	typedef gmp_data_t data_type;

public:
	// ctor & dtor
	gmp_iic_stm32_impl_t(I2C_HandleTypeDef* handle)
		:handle(handle)
	{}

public:
	// utilities
	void set_speed(uint32_t speed) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_device_addr_length(uint32_t addr_length) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

public:
	// read or write function implement

	gmp_size_t read(addr_type device_addr, data_type* data, gmp_size_t length) override;

	gmp_size_t write(addr_type device_addr, data_type* data, gmp_size_t length) override;

	gmp_size_t read(addr_type device_addr, addr_type reg_addr, data_type* data, gmp_size_t length) override;

	gmp_size_t write(addr_type device_addr, addr_type reg_addr, data_type* data, gmp_size_t length) override;

protected:
	I2C_HandleTypeDef* handle;
};


#endif // HAL_I2C_MODULE_ENABLED

#if defined HAL_SPI_MODULE_ENABLED

// SPI device handle
#define GMP_PORT_HSPI_T SPI_HandleTypeDef*

// TODO the SPI impl should be tested
class gmp_spi_stm32_impl_t
	: public gmp_spi_entity
{
public:
	// typedef
	typedef gmp_addr_t addr_type;
	typedef uint16_t data_type;

public:
	// ctor & dtor
	gmp_spi_stm32_impl_t(SPI_HandleTypeDef* handle)
		:handle(handle)
	{}

public:
	// utilities
	void set_clk_div(uint32_t div) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_mode(uint8_t polarity) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_bit_order(uint8_t order) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_frame_size(uint8_t size) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

public:
	// Basic IO functions defined here
	data_type read() override;

	gmp_size_t write(data_type data) override;

	data_type readwrite(data_type data) override;

	gmp_size_t read(data_type* data, gmp_size_t length) override;

	gmp_size_t write(data_type* data, gmp_size_t length) override;

	gmp_size_t readwrite(data_type* data_in, data_type* data_out, gmp_size_t length) override;

protected:
	SPI_HandleTypeDef* handle;
};

#endif // HAL_SPI_MODULE_ENABLED

#if defined HAL_TIM_MODULE_ENABLED

// This is the clock base frequency
#ifndef TIM_CLOCK_BASE_FREQ
#define TIM_CLOCK_BASE_FREQ ((36000000U))
#endif

// Implement of modulator entity
class gmp_timer_stm32_impl_t
	:public gmp_modulator_entity_1ch
{
public:
	// ctor & dtor
	gmp_timer_stm32_impl_t(
		TIM_HandleTypeDef* handle,
		uint32_t channel,
		gmp_timer_cmp_t prescaler = 0
	) :handle(handle), channel(channel),
		prescaler(prescaler), enable_complement(0)
	{}

public:
	// utilities function 

	// Set modulator compare value
	virtual void set_modulator_cmp(gmp_timer_cmp_t cmp_value) override
	{
		__HAL_TIM_SET_COMPARE(handle, channel, cmp_value);
	}

	// Set frequency of timer
	virtual void set_modulator_freq(gmp_timer_cmp_t freq_value) override
	{
		// ensure parameter
		assert(handle);

		__HAL_TIM_SET_COUNTER(handle, freq_value);
	}

	// Enable modulator output
	virtual void enable() override
	{
		// ensure parameters
		assert(handle);

		HAL_TIM_Base_Stop(handle);
		HAL_TIM_PWM_Start(handle, channel);
		if (enable_complement)
			HAL_TIMEx_OCN_Start(handle, channel);
	}

	// Disable modulator output
	virtual void disable() override
	{
		// ensure parameters
		assert(handle);

		HAL_TIM_PWM_Stop(handle, channel);
		if (enable_complement)
			HAL_TIMEx_OCN_Stop(handle, channel);
		HAL_TIM_Base_Start(handle);
	}

	// Set timer pre-scaler, 
	// In STM32 is a 16-bit value
	void set_prescaler(gmp_timer_cmp_t prescaler)
	{
		// ensure parameter 
		assert(handle);

		prescaler = prescaler;
		__HAL_TIM_SET_PRESCALER(handle, prescaler);
	}

	// Get prescaler register
	gmp_timer_cmp_t get_prescaler()
	{
		// check parameter
		assert(handle);

		prescaler = handle->Instance->PSC;
		return prescaler;
	}

	// Enable complementary output
	// Enable function will not start the channel
	void enable_pair_output()
	{
		enable_complement = 1;
	}

	// Disable complementary output
	void disable_pair_output()
	{
		// Close the complement channel anyway
		HAL_TIMEx_OCN_Stop(handle, channel);

		enable_complement = 0;
	}
public:
	TIM_HandleTypeDef* handle;
	uint32_t channel;

	//	uint32_t timer_freq_Hz;

	gmp_timer_cmp_t prescaler;

private:
	uint8_t enable_complement;

};

#endif // HAL_TIM_MODULE_ENABLED

#endif // _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_
