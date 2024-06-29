
// This file is GMP library C source header


//////////////////////////////////////////////////////////////////////////
// Step I import general library config
// 
#include <.config.h>


//////////////////////////////////////////////////////////////////////////
// Step II System includes
// 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#ifdef USING_FIXED_LIBRARY
#include <IQmathLib.h>
#endif // USING_FIXED_LIBRARY

//////////////////////////////////////////////////////////////////////////
// Step III GMP basement
// 
// Compiler support
#include <core/std/compiler_sup.h>

// GMP error code support
#include <core/std/error_code.h>

// <csp> default chip type definition
#include <csp/chip_port.h>

// public C ports function
// memory management support
#include <core/std/gmp_cport.h>

//////////////////////////////////////////////////////////////////////////
// Step IV other C core modules
//

// (MM) Memory Management module
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
#include <core/mm/block_mem.h>
#endif // SPECIFY_GMP_BLOCK_MEMORY_ENABLE
 
// + (WF) Workflow module


// + (SCH) Scheduling module


