
// This file will implement the gpio model of the STM32 chip.

#ifdef HAL_GPIO_MODULE_ENABLED

// invoke GMP core header
// This file will contain the correct chip HAL header.
//
#include <core/gmp_core.h>

gmp_stat_t gmp_gpio_set_mode(
	hgpio_gt hgpio,		     // handle of GPIO port
	gmp_gpio_mode_t mode     // input mode or output mode
)
{
	uint32_t temp = 0x00U;

	if (mode == GMP_GPIO_INPUT)
	{
		// Configure IO Direction mode (Input, Output) 
		//
		temp = hgpio->gpio_port->MODER;
		temp &= ~(GPIO_MODER_MODER0 << (hgpio->gpio_pin * 2U));
		temp |= ((GPIO_MODE_INPUT & GPIO_MODE) << (hgpio->gpio_pin * 2U));
		GPIOx->MODER = temp;

		return gmp_std_return(GMP_STAT_OK);
	}
	else if (mode == GMP_GPIO_OUTPUT)
	{
		// Configure IO Direction mode (Input, Output) 
		//
		temp = hgpio->gpio_port->MODER;
		temp &= ~(GPIO_MODER_MODER0 << (hgpio->gpio_pin * 2U));
		temp |= ((GPIO_MODE_OUTPUT_PP & GPIO_MODE) << (hgpio->gpio_pin * 2U));
		GPIOx->MODER = temp;

		return gmp_std_return(GMP_STAT_OK);
	}

	return gmp_std_return(GMP_STAT_INVALID_PARAM);
}

void gmp_gpio_write(
	hgpio_gt hgpio,	    // handle of GPIO port
	gpio_level_t level  // output level 
)
{
	HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState) level);

	return gmp_std_return(GMP_STAT_OK);
}

void gmp_gpio_set(
	hgpio_gt hgpio	    // handle of GPIO port
)
{
	HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)GPIO_PIN_SET);

	return gmp_std_return(GMP_STAT_OK);
}

void gmp_gpio_clear(
	hgpio_gt hgpio	    // handle of GPIO port
)
{
	HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)GPIO_PIN_RESET);

	return gmp_std_return(GMP_STAT_OK);
}

fast_gt gmp_gpio_read(
	hgpio_gt hgpio		// handle of GPIO port
)
{
	return HAL_GPIO_ReadPin(hgpio->gpio_port, hgpio->gpio_pin);
}

#endif // HAL_GPIO_MODULE_ENABLED
