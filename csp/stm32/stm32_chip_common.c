
#include <core/gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// Computing Model
#include <csp/stm32/common/computing_model.stm32.h>


//////////////////////////////////////////////////////////////////////////
// GPIO Model
#include <csp/stm32/common/gpio_model.stm32.h>

#ifdef HAL_GPIO_MODULE_ENABLED

gmp_stat_t gmp_gpio_set_mode(
	hgpio_gt *hgpio,		     // handle of GPIO port
	gmp_gpio_mode_t mode     // input mode or output mode
)
{
	return gmp_std_return(GMP_STAT_INVALID_PARAM);
}


void gmp_gpio_write(
	hgpio_gt *hgpio,	    // handle of GPIO port
	fast_gt level  // output level 
)
{
	HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)level);
}


void gmp_gpio_set(
	hgpio_gt *hgpio	    // handle of GPIO port
)
{
	HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)GPIO_PIN_SET);
}


void gmp_gpio_clear(
	hgpio_gt *hgpio	    // handle of GPIO port
)
{
	HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)GPIO_PIN_RESET);
}


fast_gt gmp_gpio_read(
	hgpio_gt *hgpio		// handle of GPIO port
)
{
	return HAL_GPIO_ReadPin(hgpio->gpio_port, hgpio->gpio_pin);
}


#endif // HAL_GPIO_MODULE_ENABLED


//////////////////////////////////////////////////////////////////////////
// peripheral Model 
#include <csp/stm32/common/peripheral_model.stm32.h>

#ifdef HAL_UART_MODULE_ENABLED

size_gt uart_tx_dma(uart_handle_t* huart, uart_content_t* content)
{
	HAL_StatusTypeDef stat;

	// using DMA
	if (HAL_DMA_GetState(huart->uart_tx_dma_handle) == HAL_DMA_STATE_READY)
	{
		stat = HAL_UART_Transmit_DMA(huart->uart_handle, content->text, content->length);
	}

	if (stat == HAL_OK)
		return content->length;
	else
		return 0;
}


size_gt uart_tx_direct(UART_HandleTypeDef* huart, uart_content_t* content)
{
	HAL_StatusTypeDef stat;

	// without DMA
	stat = HAL_UART_Transmit(huart, content->text, content->length, 10);

	if (stat == HAL_OK)
		return content->length;
	else
		return 0;
}

size_gt uart_rx_direct(UART_HandleTypeDef* huart, uart_content_t* content)
{

}

#endif // HAL_UART_MODULE_ENABLED

//////////////////////////////////////////////////////////////////////////
// SPI Model

#if defined HAL_SPI_MODULE_ENABLED

size_gt spi_tx_direct(spi_handle_t* hspi, data_gt* data, size_gt length)
{
	HAL_StatusTypeDef stat;	
	data_gt spi_data[16];

	gmp_gpio_clear(&hspi->nchip_select);
	stat = HAL_SPI_TransmitReceive(hspi->hspi, data, spi_data, length, 10);
	gmp_gpio_set(&hspi->nchip_select);

	if (stat == HAL_OK)
		return length;
	else
		return 0;
}

#endif //HAL_SPI_MODULE_ENABLED

//////////////////////////////////////////////////////////////////////////
// System Model
#include <csp/stm32/common/sys_model.stm32.h>

#ifdef HAL_RCC_MODULE_ENABLED

time_gt gmp_port_system_tick(
	void
)
{
	return HAL_GetTick();
}


#endif

void gmp_port_feed_dog(
	void
)
{
#if defined SPECIFY_ENABLE_FEED_WATCHDOG
	HAL_IWDG_Refresh(&hiwdg);
#endif // SPECIFY_ENABLE_FEED_WATCHDOG
}


void gmp_csp_startup(

)
{

}


void gmp_port_system_stuck(
	void
)
{}

// This function is unreachable.
void gmp_exit_routine(void)
{}
