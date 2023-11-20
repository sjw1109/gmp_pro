// This file is used to register chip


// STM32 ARM Cortex-M 32
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
#endif


// Texas Instrument
#if defined DSP_C2000_28069
#include <csp/dspc2000/dsp_c2000_general.h>
#include <csp/dspc2000/dsp_c28x_general.h>
#elif defined DSP_C2000_28035
#include <csp/dspc2000/dsp_c2000_general.h>
#include <csp/dspc2000/dsp_c28x_general.h>
#elif defined DSP_C2000_28027
#include <csp/dspc2000/dsp_c2000_general.h>
#include <csp/dspc2000/dsp_c28x_general.h>
#endif

#if defined DSP_C2000_280049
#include <csp/dspc2000/dsp_c2000_general.h>
#endif

#if defined GMP_WINDOWS
#include <csp/windows/win_include.h>
#endif

// default chip definition
#include <csp\chip_default_type.h>
