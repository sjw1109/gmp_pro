

// Select Master chip

#if (MASTERCHIP == GMP_STM32)
#include <csp/stm32/stm32_general.h>

#elif (MASTERCHIP == GMP_TIC2000)
#include <csp/dspc2000/dsp_c2000_general.h>

#elif (MASTERCHIP == GMP_WINDOWS)
#include <csp/windows/win_include.h>

#endif//MASTERCHIP


// STMicroelectronics
#if defined STM32G030xx
#include <csp/stm32/stm32g0/stm32g0_general.h>
#elif defined STM32F411xx
#include <csp/stm32/stm32f4/stm32f4_general.h>
#elif defined STM32L151xx
#include <csp/stm32/stm32l1/stm32l1_general.h>
#elif defined STM32L151xB
#include <csp/stm32/stm32l1/stm32l1_general.h>
#elif defined STM32H743xx
#include <csp/stm32/stm32h7/stm32h7_general.h>
#elif defined STM32F103xB
#include <csp/stm32/stm32f1/stm32f1_general.h>
#elif defined STM32F103x6
#include <csp/stm32/stm32f1/stm32f1_general.h>
#endif

// Texas Instrument
#if defined TMS320F2806x
#include <csp/dspc2000/dsp_c2000_general.h>
#include <csp/dspc2000/dsp_c28x_general.h>
#elif defined TMS320F2803x
#include <csp/dspc2000/dsp_c2000_general.h>
#include <csp/dspc2000/dsp_c28x_general.h>
#elif defined TMS320F2802x
#include <csp/dspc2000/dsp_c2000_general.h>
#include <csp/dspc2000/dsp_c28x_general.h>
#endif

#if defined DSP_C2000_280049
#include <csp/dspc2000/dsp_c2000_general.h>
#endif

