//
//// base classes headers
//#include <core/dev/peripheral_driver.hpp>
//
//
//#ifndef _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_
//#define _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_
//
//#if defined HAL_GPIO_MODULE_ENABLED
//
////#define GMP_PORT_HGPIO_T GPIO_HandleTypeDef*
//
//class gmp_gpio_stm32_impl_t
//	: public gmp_gpio_entity
//{
//public:
//	typedef data_gt data_type;
//
//public:
//	// ctor & dtor
//	gmp_gpio_stm32_impl_t(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
//		:gpio_group(GPIOx), pin_index(GPIO_Pin)
//	{}
//
//public:
//	// utility function
//	void set() override;
//	void clear() override;
//	void toggle() override;
//
//	// Write and Read function
//	size_gt write(data_type data) override;
//	data_type read() override;
//
//	// The following function is determined by chips, not implement
//	void set_mode(uint8_t mode)override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//
//public:
//	GPIO_TypeDef* gpio_group;
//	uint16_t pin_index;
//};
//
//#endif // HAL_GPIO_MODULE_ENABLED
//
//#if defined HAL_UART_MODULE_ENABLED
//
//// Specify the default UART type
//// #define GMP_PORT_HUART_T UART_HandleTypeDef*
//
//// implement the uart entity
//class gmp_uart_stm32_impl_t
//	:public gmp_uart_entity
//{
//public:
//	typedef addr_gt addr_type;
//	typedef data_gt data_type;
//
//public:
//	// ctor & dtor
//	gmp_uart_stm32_impl_t(UART_HandleTypeDef* handle)
//		:handle(handle)
//	{}
//
//public:
//	void set_baudrate(uint32_t BaudRate) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_stop_bit(uint8_t stopbit) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_data_bit(uint8_t databit) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_parity(uint8_t parity_check) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//public:
//	// Basic IO functions is defined here.
//	size_gt read(data_gt* data, size_gt length) override;
//
//	size_gt write(data_type* data, size_gt length) override;
//
//	size_gt write(const data_gt* data, size_gt length) override;
//
//	data_gt read() override;
//
//	size_gt write(data_gt data) override;
//
//protected:
//	UART_HandleTypeDef* handle;
//
//};
//
//#endif // HAL_UART_MODULE_ENABLED
//
//#if defined HAL_I2C_MODULE_ENABLED
//
//// IIC handle
//// #define GMP_PORT_HIIC_T I2C_HandleTypeDef*
//
//class gmp_iic_stm32_impl_t
//	:public gmp_iic_entity
//{
//public:
//	typedef addr_gt addr_type;
//	typedef data_gt data_type;
//
//public:
//	// ctor & dtor
//	gmp_iic_stm32_impl_t(I2C_HandleTypeDef* handle)
//		:handle(handle)
//	{}
//
//public:
//	// utilities
//	void set_speed(uint32_t speed) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_device_addr_length(uint32_t addr_length) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//public:
//	// read or write function implement
//
//	size_gt read(addr_type device_addr, data_type* data, size_gt length) override;
//
//	size_gt write(addr_type device_addr, data_type* data, size_gt length) override;
//
//	size_gt read(addr_type device_addr, addr_type reg_addr, data_type* data, size_gt length) override;
//
//	size_gt write(addr_type device_addr, addr_type reg_addr, data_type* data, size_gt length) override;
//
//protected:
//	I2C_HandleTypeDef* handle;
//};
//
//
//#endif // HAL_I2C_MODULE_ENABLED
//
//#if defined HAL_SPI_MODULE_ENABLED
//
//// SPI device handle
//// #define GMP_PORT_HSPI_T SPI_HandleTypeDef*
//
//// TODO the SPI impl should be tested
//class gmp_spi_stm32_impl_t
//	: public gmp_spi_entity
//{
//public:
//	// typedef
//	typedef addr_gt addr_type;
//	typedef uint16_t data_type;
//
//public:
//	// ctor & dtor
//	gmp_spi_stm32_impl_t(SPI_HandleTypeDef* handle)
//		:handle(handle)
//	{}
//
//public:
//	// utilities
//	void set_clk_div(uint32_t div) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_mode(uint8_t polarity) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_bit_order(uint8_t order) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//	void set_frame_size(uint8_t size) override
//	{
//		// This function is not implemented here
//		gmp_not_impl(__FILE__, __LINE__);
//	}
//
//public:
//	// Basic IO functions defined here
//	data_type read() override;
//
//	size_gt write(data_type data) override;
//
//	data_type readwrite(data_type data) override;
//
//	size_gt read(data_type* data, size_gt length) override;
//
//	size_gt write(data_type* data, size_gt length) override;
//
//	size_gt readwrite(data_type* data_in, data_type* data_out, size_gt length) override;
//
//protected:
//	SPI_HandleTypeDef* handle;
//};
//
//#endif // HAL_SPI_MODULE_ENABLED
//
//#if defined HAL_TIM_MODULE_ENABLED
//
//class gmp_timer_stm32_impl_t
//	:public gmp_timer_entity
//{
//public:
//	//ctor & dtor
//	gmp_timer_stm32_impl_t(
//		TIM_HandleTypeDef* handle
//	)
//		:handle(handle)
//	{}
//
//public:
//	// utilities function
//
//	// Set frequency of timer
//	virtual void set_freq(time_gt freq_value)
//	{
//		// ensure parameter
//		assert(handle);
//
//		__HAL_TIM_SET_COUNTER(handle, freq_value);
//	}
//
//	// get frequency of timer
//	virtual time_gt get_freq()
//	{
//		assert(handle);
//
//		return __HAL_TIM_GET_COUNTER(handle);
//	}
//
//
//	// Enable modulator output
//	virtual void enable()
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_Base_Stop(handle);
//
//	}
//
//	// Disable modulator output
//	virtual void disable()
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_Base_Start(handle);
//	}
//
//public:
//	TIM_HandleTypeDef* handle;
//};
//
//
//// Implement of modulator entity
//class gmp_pwm1ch_stm32_impl_t
//	:public gmp_modulator_entity_1ch
//{
//public:
//	// ctor & dtor
//	gmp_pwm1ch_stm32_impl_t(
//		TIM_HandleTypeDef* handle,
//		uint32_t channel,
//		uint8_t enable_complement = 0 // Disable output complement
//	)
//		:handle(handle), channel(channel),
//		enable_complement(enable_complement)
//	{}
//
//public:
//	// utilities function 
//
//	// Get modulator frequency
//	time_gt get_modulator_freq() override
//	{
//		assert(handle);
//
//		return __HAL_TIM_GET_COUNTER(handle);
//	}
//
//	// Set modulator frequency
//	void set_modulator_freq(time_gt freq_value) override
//	{
//		// ensure parameter
//		assert(handle);
//
//		__HAL_TIM_SET_COUNTER(handle, freq_value);
//	}
//
//	// Set modulator compare value
//	void set_modulator_cmp(time_gt cmp_tick) override
//	{
//		__HAL_TIM_SET_COMPARE(handle, channel, cmp_tick);
//	}
//
//	// Enable modulator output
//	void enable() override
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_Base_Stop(handle);
//		HAL_TIM_PWM_Start(handle, channel);
//		if (enable_complement)
//			HAL_TIMEx_OCN_Start(handle, channel);
//	}
//
//	// Disable modulator output
//	void disable() override
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_PWM_Stop(handle, channel);
//		if (enable_complement)
//			HAL_TIMEx_OCN_Stop(handle, channel);
//		HAL_TIM_Base_Start(handle);
//	}
//
//
//	// Enable complementary output
//	// Enable function will not start the channel
//	void enable_pair_output()
//	{
//		enable_complement = 1;
//	}
//
//	// Disable complementary output
//	void disable_pair_output()
//	{
//		// Close the complement channel anyway
//		HAL_TIMEx_OCN_Stop(handle, channel);
//
//		enable_complement = 0;
//	}
//public:
//	TIM_HandleTypeDef* handle;
//	uint32_t channel;
//
//private:
//	uint8_t enable_complement;
//
//};
//
//// This class has 2 channels
//class gmp_pwm2ch_stm32_impl_t
//	:public gmp_modulator_entity_2ch
//{
//public:
//	// ctor & dtor
//	gmp_pwm2ch_stm32_impl_t(
//		TIM_HandleTypeDef* handle,
//		uint32_t channel1, uint32_t channel2,
//		uint8_t enable_complement = 0 // Disable output complement
//	)
//		:handle(handle),
//		channel1(channel1), channel2(channel2),
//		enable_complement(enable_complement)
//	{}
//
//public:
//	// utilities function 
//
//	// Get modulator frequency
//	time_gt get_modulator_freq() override
//	{
//		assert(handle);
//
//		return __HAL_TIM_GET_COUNTER(handle);
//	}
//
//	// Set modulator frequency
//	void set_modulator_freq(time_gt freq_value) override
//	{
//		// ensure parameter
//		assert(handle);
//
//		__HAL_TIM_SET_COUNTER(handle, freq_value);
//	}
//
//	// Set modulator compare value
//	void set_modulator_cmp(time_gt cmp_tick1,
//		time_gt cmp_tick2) override
//	{
//		__HAL_TIM_SET_COMPARE(handle, channel1, cmp_tick1);
//		__HAL_TIM_SET_COMPARE(handle, channel2, cmp_tick2);
//	}
//
//	// Enable modulator output
//	void enable() override
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_Base_Stop(handle);
//		HAL_TIM_PWM_Start(handle, channel1);
//		HAL_TIM_PWM_Start(handle, channel2);
//		if (enable_complement)
//		{
//			HAL_TIMEx_OCN_Start(handle, channel1);
//			HAL_TIMEx_OCN_Start(handle, channel2);
//		}
//	}
//
//	// Disable modulator output
//	void disable() override
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_PWM_Stop(handle, channel1);
//		HAL_TIM_PWM_Stop(handle, channel2);
//		if (enable_complement)
//		{
//			HAL_TIMEx_OCN_Stop(handle, channel1);
//			HAL_TIMEx_OCN_Stop(handle, channel2);
//		}
//		HAL_TIM_Base_Start(handle);
//	}
//
//
//	// Enable complementary output
//	// Enable function will not start the channel
//	void enable_pair_output()
//	{
//		enable_complement = 1;
//	}
//
//	// Disable complementary output
//	void disable_pair_output()
//	{
//		// Close the complement channel anyway
//		HAL_TIMEx_OCN_Stop(handle, channel1);
//		HAL_TIMEx_OCN_Stop(handle, channel2);
//
//		enable_complement = 0;
//	}
//public:
//	TIM_HandleTypeDef* handle;
//	uint32_t channel1;
//	uint32_t channel2;
//
//private:
//	uint8_t enable_complement;
//};
//
//// This class has 3 channels
//class gmp_pwm3ch_stm32_impl_t
//	:public gmp_modulator_entity_3ch
//{
//public:
//	// ctor & dtor
//	gmp_pwm3ch_stm32_impl_t(
//		TIM_HandleTypeDef* handle,
//		uint32_t channel1, uint32_t channel2, uint32_t channel3,
//		uint8_t enable_complement = 0 // Disable output complement
//	)
//		:handle(handle),
//		channel1(channel1), channel2(channel2),channel3(channel3),
//		enable_complement(enable_complement)
//	{}
//
//public:
//	// utilities function 
//
//	// Get modulator frequency
//	time_gt get_modulator_freq() override
//	{
//		assert(handle);
//
//		return __HAL_TIM_GET_COUNTER(handle);
//	}
//
//	// Set modulator frequency
//	void set_modulator_freq(time_gt freq_value) override
//	{
//		// ensure parameter
//		assert(handle);
//
//		__HAL_TIM_SET_COUNTER(handle, freq_value);
//	}
//
//	// Set modulator compare value
//	void set_modulator_cmp(time_gt cmp_tick1,
//		time_gt cmp_tick2,
//		time_gt cmp_tick3) override
//	{
//		__HAL_TIM_SET_COMPARE(handle, channel1, cmp_tick1);
//		__HAL_TIM_SET_COMPARE(handle, channel2, cmp_tick2);
//		__HAL_TIM_SET_COMPARE(handle, channel3, cmp_tick3);
//	}
//
//	// Enable modulator output
//	void enable() override
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_Base_Stop(handle);
//		HAL_TIM_PWM_Start(handle, channel1);
//		HAL_TIM_PWM_Start(handle, channel2);
//		HAL_TIM_PWM_Start(handle, channel3);
//		if (enable_complement)
//		{
//			HAL_TIMEx_OCN_Start(handle, channel1);
//			HAL_TIMEx_OCN_Start(handle, channel2);
//			HAL_TIMEx_OCN_Start(handle, channel3);
//		}
//	}
//
//	// Disable modulator output
//	void disable() override
//	{
//		// ensure parameters
//		assert(handle);
//
//		HAL_TIM_PWM_Stop(handle, channel1);
//		HAL_TIM_PWM_Stop(handle, channel2);
//		HAL_TIM_PWM_Stop(handle, channel3);
//		if (enable_complement)
//		{
//			HAL_TIMEx_OCN_Stop(handle, channel1);
//			HAL_TIMEx_OCN_Stop(handle, channel2);
//			HAL_TIMEx_OCN_Stop(handle, channel3);
//		}
//		HAL_TIM_Base_Start(handle);
//	}
//
//
//	// Enable complementary output
//	// Enable function will not start the channel
//	void enable_pair_output()
//	{
//		enable_complement = 1;
//	}
//
//	// Disable complementary output
//	// DANGEROUS: This function should NOT be invoked during controller running.
//	void disable_pair_output()
//	{
//		// Close the complement channel anyway
//		HAL_TIMEx_OCN_Stop(handle, channel1);
//		HAL_TIMEx_OCN_Stop(handle, channel2);
//		HAL_TIMEx_OCN_Stop(handle, channel3);
//
//		enable_complement = 0;
//	}
//public:
//	TIM_HandleTypeDef* handle;
//	uint32_t channel1;
//	uint32_t channel2;
//	uint32_t channel3;
//
//private:
//	uint8_t enable_complement;
//};
//
//#endif // HAL_TIM_MODULE_ENABLED
//
//
//#endif // _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_
