/**
 * @file gmp_core.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */


// This file is GMP library C source header


//////////////////////////////////////////////////////////////////////////
// Step I import general library config
// 

// invoke user config file
#include <user.config.h>

// invoke system default config
#include <.config.h>

// invoke CSP default config
#include <csp.config.h>


//////////////////////////////////////////////////////////////////////////
// Step II System includes
// 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>


// TI fixed library
#ifdef ENABLE_IQMATH_HEADER_DIREDCT
#ifndef __IQMATHLIB_H_INCLUDED__

#include <third_party\iqmath\IQmathLib.h>
//#include "sw/modules/iqmath/src/32b/IQmathLib.h"
#endif
#endif // USING_FIXED_LIBRARY


//////////////////////////////////////////////////////////////////////////
// Step III GMP basement
// 

// Basic GMP standard type definition
#include <core/std/gmp.std.h>

// <csp> default chip type definition
#include <csp/chip_port.h>

// default peripheral types
// This header may be seized in `chip_port.h`
#include <core/std/default_peripheral.config.h>

// public C ports function
// memory management support
#include <core/std/gmp_cport.h>

// public C CSP ports functions
// peripheral functions
#include <core/std/gmp_csp_cport.h>

//////////////////////////////////////////////////////////////////////////
// Step IV other C core modules
//



// (MM) Memory Management module
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
#include <core/mm/block_mem.h>
#endif // SPECIFY_GMP_BLOCK_MEMORY_ENABLE
 
// + (WF) Workflow module


// + (SCH) Scheduling module


