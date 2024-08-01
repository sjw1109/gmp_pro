
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



// default Type config
// Change some default type here.
#define GMP_PORT_DATA_T				    unsigned char
#define GMP_PORT_DATA_SIZE_PER_BITS		(8)
#define GMP_PORT_DATA_SIZE_PER_BYTES    (1)

// Invoke default type
#include <core/std/default.types.h>

#include <core/std/gmp_peripheral_definition.h>

// Invoke peripheral headers

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

// Invoke default peripheral definition here
#include <core/std/default_peripheral.config.h>


