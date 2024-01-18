// This file is  GMP library C source header

// user config
#include <.config.h>

// System includes
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

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
// other C modules

#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
#include <core/mm/block_mem.h>
#endif // SPECIFY_GMP_BLOCK_MEMORY_ENABLE
 
