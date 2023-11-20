
#ifndef _FILE_STM32_GENERAL_H_
#define _FILE_STM32_GENERAL_H_


// Select HAL library
#if defined STM32G030xx
#include "stm32g0xx_hal.h"
#elif defined STM32F411xx
#include "stm32f4xx_hal.h"
#elif defined STM32L151xx
#include "stm32l1xx_hal.h"
#elif defined STM32L151xB
#include "stm32l1xx_hal.h"
#endif



// basic type definitions
#define GMP_PORT_IIC_HANDLE_T		I2C_HandleTypeDef*
#define GMP_PORT_UART_HANDLE_T		UART_HandleTypeDef*
#define GMP_PORT_SPI_HANDLE_T		SPI_HandleTypeDef*
#define GMP_PORT_IIS_HANDLE_T
#define GMP_PORT_USART_HANDLE_T



// type definition
#define GMP_DATA_ELEMENT_T		uint8_t
#define GMP_LENGTH_T			uint16_t

#include <csp/chip_default_type.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Port Functions Prototype
GMP_LENGTH_T gmp_iic_transmit(GMP_PORT_IIC_HANDLE_T hi2c, GMP_LENGTH_T iic_dev_addr, GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len);

void gmp_iic_request_read_mem
(
	GMP_PORT_IIC_HANDLE_T hi2c,
	GMP_CMD_T cmd,
	GMP_LENGTH_T length
);

GMP_LENGTH_T gmp_iic_receive(
	GMP_PORT_IIC_HANDLE_T hi2c,
	GMP_LENGTH_T iic_dev_addr,
	GMP_CMD_T cmd, GMP_LENGTH_T cmd_len,
	GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_STM32_GENERAL_H_

#include <csp/stm32/stm32_inline.h>
