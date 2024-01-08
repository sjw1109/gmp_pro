// This file contains all the C++ interface of chips

#include <core/dev/io_concept.hpp>


// Select Master chip

#if (MASTERCHIP == GMP_STM32)
#include <csp/stm32/stm32_general.hpp>

#elif (MASTERCHIP == GMP_TIC2000)
#include <csp/dspc2000/dsp_c2000_general.hpp>

#elif (MASTERCHIP == GMP_WINDOWS)
#include <csp/windows/win_include.hpp>

#endif//MASTERCHIP


