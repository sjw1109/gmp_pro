
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
	gmp_size_t write( data_type data) override;
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

	gmp_size_t write( gmp_data_t data) override;

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

	gmp_size_t write(addr_type device_addr,  data_type* data, gmp_size_t length) override;

	gmp_size_t read(addr_type device_addr, addr_type reg_addr, data_type* data, gmp_size_t length) override;

	gmp_size_t write(addr_type device_addr, addr_type reg_addr,  data_type* data, gmp_size_t length) override;

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

	gmp_size_t write( data_type data) override;

	data_type readwrite( data_type data) override;

	gmp_size_t read(data_type* data, gmp_size_t length) override;

	gmp_size_t write( data_type* data, gmp_size_t length) override;

	gmp_size_t readwrite(data_type* data_in,  data_type* data_out, gmp_size_t length) override;

protected:
	SPI_HandleTypeDef* handle;
};

#endif // HAL_SPI_MODULE_ENABLED

#endif // _FILE_STM32_PERIPHERAL_IMPLEMENT_HPP_
