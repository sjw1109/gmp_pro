// This file contains all the core functions
// If you need to use GMP library you should include this header first

extern "C"
{

#include <core/gmp_core.h>

}

// This library is only support the C++ 11 compiler environment 
// This file should be included by C++ source files

//////////////////////////////////////////////////////////////////////////
// include GMP C++ interface
// + memory management of new and delete
#include <core/std/gmp_cpp_port.hpp>


//////////////////////////////////////////////////////////////////////////
// invoke chip support package
#include <csp/chip_port.hpp>


//////////////////////////////////////////////////////////////////////////
// introduce core system modules

// invoke basic io concepts
#include <core/dev/io_concept.hpp>

// invoke peripheral entity class
#include <core/dev/peripheral_driver.hpp>

// invoke devices abstract
#include <core/dev/device_prototype.hpp>


//////////////////////////////////////////////////////////////////////////
// include peripheral mapping
#include <user/peripheral_mapping.hpp>

