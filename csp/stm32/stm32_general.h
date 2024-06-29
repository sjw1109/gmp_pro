
//////////////////////////////////////////////////////////////////////////
// Step I: Select HAL library
//

// Due to STM32 has unique macro to specify the chip set,
// just judge these macros.
//
#if defined STM32G030xx
#include "stm32g0xx_hal.h"
#elif defined STM32G071xx
#include "stm32g0xx_hal.h"
#elif defined STM32G474xx
#include "stm32g4xx_hal.h"
#elif defined STM32L151xx
#include "stm32l1xx_hal.h"
#elif defined STM32L151xB
#include "stm32l1xx_hal.h"
#elif defined STM32F103xB
#include "stm32f1xx_hal.h"
#elif defined STM32F103x6
#include "stm32f1xx_hal.h"
#elif defined STM32F411xx
#include "stm32f4xx_hal.h"
#endif


//////////////////////////////////////////////////////////////////////////
// Step II: Invoke all the STM32 general headers.
//

// STM32 System core support
#include <csp/stm32/common/sys_model.stm32.h>

// STM32 System Computing support
#include <csp/stm32/common/computing_model.stm32.h>

// STM32 GPIO support 
#include <csp/stm32/common/gpio_model.stm32.h>



//////////////////////////////////////////////////////////////////////////
// Step III Default types
//

// GMP default types
#include <core/std/default.types.h>

// #ifndef _FILE_STM32_GENERAL_H_
// #define _FILE_STM32_GENERAL_H_
// 
// 
// 
// #define GMP_PORT_DATA_T			char     // not for sure signed or unsigned
// #define GMP_PORT_SIZE_T			uint32_t
// 
// // Basic handle definition
// 
// #define GMP_PORT_HIIC_T		I2C_HandleTypeDef*
// #define GMP_PORT_HUART_T	UART_HandleTypeDef*
// #define GMP_PORT_HSPI_T		SPI_HandleTypeDef*
// #define GMP_PORT_HCAN_T     CAN_HandleTypeDef*
// #define GMP_PORT_HIIS_T
// #define GMP_PORT_HUSART_T
// 
// 
// #endif // _FILE_STM32_GENERAL_H_



