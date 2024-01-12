//#include <csp/stm32/stm32_general.h>
#include <core/gmp_core.hpp>

//////////////////////////////////////////////////////////////////////////
// TIMER function

// get system default tick
GMP_PORT_TIME_T gmp_port_system_tick(
	void
)
{
	return HAL_GetTick();
}

// Prototype of Error handle
extern "C"
void Error_Handler(void);

void gmp_port_system_stuck(
	void
)
{
	// invoke the Error handler
	Error_Handler();
}

//////////////////////////////////////////////////////////////////////////
// IO peripheral function implement
#if defined HAL_GPIO_MODULE_ENABLED

void gmp_gpio_stm32_impl_t::set()
{
	HAL_GPIO_WritePin(gpio_group, pin_index, GPIO_PIN_SET);
}

void gmp_gpio_stm32_impl_t::clear()
{
	HAL_GPIO_WritePin(gpio_group, pin_index, GPIO_PIN_RESET);
}

void gmp_gpio_stm32_impl_t::toggle()
{
	HAL_GPIO_TogglePin(gpio_group, pin_index);
}

gmp_size_t gmp_gpio_stm32_impl_t::write(data_type data)
{
	HAL_GPIO_WritePin(gpio_group, pin_index, (GPIO_PinState)data);
	return 1;
}

gmp_gpio_stm32_impl_t::data_type gmp_gpio_stm32_impl_t::read()
{
	return HAL_GPIO_ReadPin(gpio_group, pin_index);
}

#endif // HAL_GPIO_MODULE_ENABLED


#if defined HAL_UART_MODULE_ENABLED
gmp_size_t gmp_uart_stm32_impl_t::read(gmp_data_t* data, gmp_size_t length) 
{
	HAL_StatusTypeDef stat = HAL_UART_Receive(handle, reinterpret_cast<uint8_t*>(data), length, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}


gmp_size_t gmp_uart_stm32_impl_t::write(gmp_data_t* data, gmp_size_t length)
{
	HAL_StatusTypeDef stat = HAL_UART_Transmit(handle, reinterpret_cast<uint8_t*>(data), length, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}

}

gmp_data_t gmp_uart_stm32_impl_t::read()
{
	gmp_data_t content;

	HAL_StatusTypeDef stat = HAL_UART_Receive(handle, reinterpret_cast<uint8_t*>(&content), 1, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		return content;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_uart_stm32_impl_t::write(gmp_data_t data)
{
	HAL_StatusTypeDef stat = HAL_UART_Transmit(handle, reinterpret_cast<uint8_t*>(&data), 1, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		return 1;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

#endif // HAL_UART_MODULE_ENABLED

#if defined HAL_I2C_MODULE_ENABLED
gmp_size_t gmp_iic_stm32_impl_t::read(addr_type device_addr, data_type* data, gmp_size_t length) 
{
	HAL_StatusTypeDef stat = HAL_I2C_Master_Receive(handle, device_addr << 1 | 1, data,
		length, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_iic_stm32_impl_t::write(addr_type device_addr, data_type* data, gmp_size_t length)
{
	HAL_StatusTypeDef stat = HAL_I2C_Master_Transmit(handle, device_addr << 1, data,
		length, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_iic_stm32_impl_t::read(addr_type device_addr, addr_type reg_addr, data_type* data, gmp_size_t length)
{
	HAL_StatusTypeDef stat = HAL_I2C_Mem_Read(handle, device_addr << 1 | 1, reg_addr,
		reg_addr_len, data, length, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}

}

gmp_size_t gmp_iic_stm32_impl_t::write(addr_type device_addr, addr_type reg_addr, data_type* data, gmp_size_t length)
{
	HAL_StatusTypeDef stat = HAL_I2C_Mem_Write(handle, device_addr << 1, reg_addr,
		reg_addr_len, data, length, g_delay_ms);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

#endif // HAL_I2C_MODULE_ENABLED

#if defined HAL_SPI_MODULE_ENABLED
gmp_spi_stm32_impl_t::data_type gmp_spi_stm32_impl_t::read() 
{
	data_type data;

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);

	HAL_StatusTypeDef stat = HAL_SPI_Receive(handle, (uint8_t*)&data, 1, g_delay_ms);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

	// translate error code
	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return data;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_TIMEOUT)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_INVALID_PARAM;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_spi_stm32_impl_t::write(data_type data) 
{
	HAL_StatusTypeDef stat = HAL_SPI_Transmit(handle, (uint8_t*)&data, 1, g_delay_ms);

	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return 1ul;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_TIMEOUT)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_INVALID_PARAM;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_spi_stm32_impl_t::data_type gmp_spi_stm32_impl_t::readwrite(data_type data) 
{
	data_type recv;
	HAL_StatusTypeDef stat = HAL_SPI_TransmitReceive(handle, (uint8_t*)&data,
		(uint8_t*)&recv, 1, g_delay_ms);

	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return recv;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_TIMEOUT)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_INVALID_PARAM;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_spi_stm32_impl_t::read(data_type* data, gmp_size_t length)
{
	HAL_StatusTypeDef stat = HAL_SPI_Receive(handle, (uint8_t*)data, length, g_delay_ms);

	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_TIMEOUT)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_INVALID_PARAM;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_spi_stm32_impl_t::write(data_type* data, gmp_size_t length) 
{
	// 		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET);

	HAL_StatusTypeDef stat = HAL_SPI_Transmit(handle, (uint8_t*)data, length, g_delay_ms);
	//		if (__HAL_SPI_GET_FLAG(handle, SPI_FLAG_TXTF))
	//{
	//    __HAL_SPI_CLEAR_TXTFFLAG(handle);
	//}

// 		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_TIMEOUT)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_INVALID_PARAM;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}

gmp_size_t gmp_spi_stm32_impl_t::readwrite(data_type* data_in, data_type* data_out, gmp_size_t length)
{
	HAL_StatusTypeDef stat = HAL_SPI_TransmitReceive(handle, (uint8_t*)data_in, (uint8_t*)data_out, length, g_delay_ms);

	if (stat == HAL_OK)
	{
		last_error = GMP_STAT_OK;
		return length;
	}
	else if (stat == HAL_BUSY)
	{
		last_error = GMP_STAT_BUSY;
		return 0;
	}
	else if (stat == HAL_TIMEOUT)
	{
		last_error = GMP_STAT_RESPONSE_TIMEOUT;
		return 0;
	}
	else if (stat == HAL_ERROR)
	{
		last_error = GMP_STAT_INVALID_PARAM;
		return 0;
	}
	else
	{
		last_error = GMP_STAT_UNDEFINED_ACTION;
		return 0;
	}
}
#endif // HAL_SPI_MODULE_ENABLED