/**
 * @file gmp_core.hpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// This file contains all the core functions
// If you need to use GMP library you should include this header first

//////////////////////////////////////////////////////////////////////////
// Step I import all C headers and extern to C headers
//

extern "C"
{

#include <core/gmp_core.h>

}

// This library is only support the C++ 11 compiler environment 
// This file should be included by C++ source files

//////////////////////////////////////////////////////////////////////////
// Step II import System headers
//
// Three basic cases




//////////////////////////////////////////////////////////////////////////
// Step III import GMP basement
// 
// import GMP C++ interface
#include <core/std/gmp_cpp_port.hpp>

// + memory management of new and delete


// invoke chip support package
#include <csp/chip_port.hpp>


//////////////////////////////////////////////////////////////////////////
// Step IV import core system modules
//
// invoke basic IO concepts
#include <core/dev/io_concept.hpp>


//////////////////////////////////////////////////////////////////////////
// Step V import peripheral mapping
//

#include <user/user_main.hpp>

//////////////////////////////////////////////////////////////////////////
// Step VI import other necessary modules

