/**
 * @file csp.general.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

#include <csp.config.h>

//////////////////////////////////////////////////////////////////////////
// Step I: Select HAL library
//

#ifndef SPECIFY_PROJECT_GENERATED_BY_CUBEMX

// Due to STM32 has unique macro to specify the chip set,
// just judge these macros.
//
#if defined STM32G030xx
#include "stm32g0xx_hal.h"
#elif defined STM32G071xx
#include "stm32g0xx_hal.h"
#elif defined STM32G474xx
#include "stm32g4xx_hal.h"
#elif defined STM32G431xx
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
#elif defined STM32F411xE
#include "stm32f4xx_hal.h"
#elif defined STM32U083xx
#include "stm32u0xx_hal.h"

#endif // STM32 SERIES

#else

// Cube MX will generate main.h so just use it.
#include "main.h"

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

// STM32 general peripheral
#include <csp/stm32/common/peripheral_model.stm32.h>


