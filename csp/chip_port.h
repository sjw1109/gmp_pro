

//////////////////////////////////////////////////////////////////////////
// Section I import all chip library files
// 
// Select Master chip


// STMicroelectronics
//#if defined STM32G030xx
//#include <csp/stm32/stm32g0/stm32g0_general.h>
//#elif defined STM32G071xx
//#include <csp/stm32/stm32g0/stm32g0_general.h>
//#elif defined STM32F411xx
//#include <csp/stm32/stm32f4/stm32f4_general.h>
//#elif defined STM32L151xx
//#include <csp/stm32/stm32l1/stm32l1_general.h>
//#elif defined STM32L151xB
//#include <csp/stm32/stm32l1/stm32l1_general.h>
//#elif defined STM32H743xx
//#include <csp/stm32/stm32h7/stm32h7_general.h>
//#elif defined STM32F103xB
//#include <csp/stm32/stm32f1/stm32f1_general.h>
//#elif defined STM32F103x6
//#include <csp/stm32/stm32f1/stm32f1_general.h>
//#endif

// C2000 series C2000wares



//////////////////////////////////////////////////////////////////////////
// Section II import chip general config headers
//
// Select master chip config

#if (MASTERCHIP == GMP_AUTO_STM32)
// STMicroelectronics
#include <csp/stm32/stm32_general.h>

#elif (MASTERCHIP == GMP_AUTO_TIC2000)
// Texas Instrument
#include <csp/dspc2000/dsp_c2000_general.h>

#elif (MASTERCHIP == GMP_AUTO_WINDOWS)
// Windows
#include <csp/windows/win_include.h>

#endif//MASTERCHIP


//////////////////////////////////////////////////////////////////////////
// Step III import default types
//
// The rest key definitions will be filled.
//#include <core/std/default.types.h>


//////////////////////////////////////////////////////////////////////////
// Step IV import all inline functions
// 
// 


