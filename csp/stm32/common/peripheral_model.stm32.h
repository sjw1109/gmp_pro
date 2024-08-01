
// This file contains all the peripheral implements

#ifndef _FILE_PERIPHERAL_MODEL_STM32_H_
#define _FILE_PERIPHERAL_MODEL_STM32_H_

// ....................................................................//
// PWM controller
//
// kernel functions:
// + Enable PWM output
// + Disable PWM output
// + Set Duty
// + Set frequency
//
#ifdef HAL_TIM_MODULE_ENABLED


#endif // HAL_TIM_MODULE_ENABLED


// ....................................................................//
// UART 
//
// kernel functions:
// + Write 
// + Read
// + Set Baud Rate
//
#ifdef HAL_UART_MODULE_ENABLED

// Note:
// 发送和接收提供基础版本
// 发送提供池模式，接收提供缓冲模式


typedef struct _tag_uart_handle
{
	UART_HandleTypeDef* uart_handle;
	DMA_HandleTypeDef* uart_tx_dma_handle;
	DMA_HandleTypeDef* uart_rx_dma_handle;
}uart_handle_t;


size_gt uart_tx_dma(uart_handle_t* huart, uart_content_t* content);

size_gt uart_tx_direct(UART_HandleTypeDef* huart, uart_content_t* content);

#define GMP_PORT_HUART_T UART_HandleTypeDef
#define GMP_PORT_DBG_PRINT_HANDLE_T uart_handle_t*
#define GMP_PORT_DBG_PRINT_FUNC uart_tx_dma


#endif // HAL_UART_MODULE_ENABLED


// ....................................................................//
// SPI
//
// kernel functions:
// + Write
// + Read
// + ReadWrite
// + Set SPI Mode
// 
#ifdef HAL_SPI_MODULE_ENABLED

typedef struct _tag_spi_handle
{
	// chip select
	gpio_model_stm32_t nchip_select;

	// SPI handle 
	SPI_HandleTypeDef* hspi;
}spi_handle_t;

// length < 16
size_gt spi_tx_direct(spi_handle_t* hspi, data_gt* data, size_gt length);

#define GMP_PORT_HSPI_T SPI_HandleTypeDef


#endif // HAL_SPI_MODULE_ENABLED

// ....................................................................//
// IIC
//
// kernel functions:
// + Write
// + Read
//
#ifdef HAL_I2C_MODULE_ENABLED

#endif // HAL_I2C_MODULE_ENABLED

#endif // _FILE_PERIPHERAL_MODEL_STM32_H_
