
#if defined HAL_GPIO_MODULE_ENABLED

class stm32g0_gpio_impl_t
	:public gmp_gpio_stm32_impl_t
{

public:
	// ctor & dtor
	stm32g0_gpio_impl_t(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
		:gmp_gpio_stm32_impl_t(GPIOx, GPIO_Pin)
	{}

public:
	// utilities
	void set_mode(uint8_t mode) override
	{
		uint16_t pin_index_temp = pin_index;
		uint16_t number;

		while ((pin_index >> number) != 0x00u)
		{
			if ((pin_index >> number) & 1)
			{
				uint32_t temp = gpio_group->MODER;
				temp &= ~(GPIO_MODER_MODE0 << (number * 2u));
				temp |= (mode << (number * 2u));
				gpio_group->MODER = temp;
			}
		}
	}
};

#endif // HAL_GPIO_MODULE_ENABLED

#if defined HAL_UART_MODULE_ENABLED

class stm32g0_uart_impl_t
	: public gmp_uart_stm32_impl_t
{
public:
	// ctor& dtor
	stm32g0_uart_impl_t(UART_HandleTypeDef* handle)
		:gmp_uart_stm32_impl_t(handle)
	{}

public:
	//utilities
	void set_baudrate(uint32_t BaudRate) override
	{
		handle->Init.BaudRate = BaudRate;

		HAL_StatusTypeDef stat = UART_SetConfig(handle);

		if (stat == HAL_OK)
		{
			last_error = GMP_STAT_OK;
		}
		else if (stat == HAL_ERROR)
		{
			last_error = GMP_STAT_REFUSED;
		}
		else
		{
			last_error = GMP_STAT_UNDEFINED_ACTION;
		}
	}

	void set_stop_bit(uint8_t stopbit) override
	{
		switch (stopbit)
		{
		case stop_bit_1:
			handle->Init.StopBits = UART_STOPBITS_1;
			break;
		case stop_bit_1_5:
			handle->Init.StopBits = UART_STOPBITS_1_5;
			break;
		case stop_bit_2:
			handle->Init.StopBits = UART_STOPBITS_2;
			break;
		default:
			last_error = GMP_STAT_INVALID_PARAM;
			return;

		}

		HAL_StatusTypeDef stat = UART_SetConfig(handle);

		if (stat == HAL_OK)
		{
			last_error = GMP_STAT_OK;
		}
		else if (stat == HAL_ERROR)
		{
			last_error = GMP_STAT_REFUSED;
		}
		else
		{
			last_error = GMP_STAT_UNDEFINED_ACTION;
		}
	}

	void set_data_bit(uint8_t databit) override
	{
		handle->Init.WordLength = databit;

		HAL_StatusTypeDef stat = UART_SetConfig(handle);

		if (stat == HAL_OK)
		{
			last_error = GMP_STAT_OK;
		}
		else if (stat == HAL_ERROR)
		{
			last_error = GMP_STAT_REFUSED;
		}
		else
		{
			last_error = GMP_STAT_UNDEFINED_ACTION;
		}
	}

	void set_parity(uint8_t parity_check) override
	{
		switch (parity_check)
		{
		case parity_none:
			handle->Init.Parity = UART_PARITY_NONE;
			break;
		case parity_even:
			handle->Init.Parity = UART_PARITY_EVEN;
			break;
		case parity_odd:
			handle->Init.Parity = UART_PARITY_ODD;
			break;
		}

		HAL_StatusTypeDef stat = UART_SetConfig(handle);

		if (stat == HAL_OK)
		{
			last_error = GMP_STAT_OK;
		}
		else if (stat == HAL_ERROR)
		{
			last_error = GMP_STAT_REFUSED;
		}
		else
		{
			last_error = GMP_STAT_UNDEFINED_ACTION;
		}
	}

};
#endif // HAL_UART_MODULE_ENABLED
