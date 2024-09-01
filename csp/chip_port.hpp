
// This file contains all the C++ interface of chips


//////////////////////////////////////////////////////////////////////////
// Step I import all C ports
//
// All the movements has been done by higher level headers.
// So Null here.
//


//////////////////////////////////////////////////////////////////////////
// Step II Import basic concept definition
//
#include <core/dev/io_concept.hpp>


//////////////////////////////////////////////////////////////////////////
// Step III Select Master chip
//
// All the definitions has been done by MASTERCHIP MACRO

#if (MASTERCHIP == GMP_AUTO_STM32)

#elif (MASTERCHIP == GMP_AUTO_TIC2000)
#include <csp/dspc2000/dsp_c2000_general.h>

#elif (MASTERCHIP == GMP_AUTO_WINDOWS)
#include <csp/windows/win_include.hpp>

#endif//MASTERCHIP


