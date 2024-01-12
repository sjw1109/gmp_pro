
// Select HAL library
#if defined STM32G030xx
#include "stm32g0xx_hal.h"
#elif defined STM32F411xx
#include "stm32f4xx_hal.h"
#elif defined STM32L151xx
#include "stm32l1xx_hal.h"
#elif defined STM32L151xB
#include "stm32l1xx_hal.h"
#elif defined STM32F103xB
#include "stm32f1xx_hal.h"
#elif defined STM32F103x6
#include "stm32f1xx_hal.h"
#endif


#ifndef _FILE_STM32_GENERAL_H_
#define _FILE_STM32_GENERAL_H_

// Basic type definition
//#define GMP_DATA_ELEMENT_T		uint8_t
//#define GMP_LENGTH_T			uint16_t

#define GMP_PORT_DATA_T			uint8_t
#define GMP_PORT_SIZE_T			uint16_t

// Basic handle definition
#define GMP_PORT_HIIC_T		I2C_HandleTypeDef*
#define GMP_PORT_HUART_T	UART_HandleTypeDef*
#define GMP_PORT_HSPI_T		SPI_HandleTypeDef*
#define GMP_PORT_HIIS_T
#define GMP_PORT_HUSART_T


#endif // _FILE_STM32_GENERAL_H_

#include <core/std/default_types.h>


