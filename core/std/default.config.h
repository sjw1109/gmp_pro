/**
 * @file default.config.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// This file is default parameters config for `.config.h` file.

//////////////////////////////////////////////////////////////////////////
// Step I: Global constant
// 

// ....................................................................//
// Function delay parameters
// This function would be store in the global variables. 
// Some more details may find in `gmp_cport.h`
//

// Timeout counter
// This parameters would be passed to the IO function to define the 
// Maximum loop waiting counts, unit cycles. 
//
#ifndef TIMEOUT_CNT
#define TIMEOUT_CNT 0X3FFF
#endif // TIMEOUT_CNT

// Timeout timer
// This parameters would be passed to the IO function to define the 
// Maximum waiting time, unit ms (millisecond).
//
#ifndef TIMEOUT_MS
#define TIMEOUT_MS 0x000A
#endif // TIMEOUT_MS



//////////////////////////////////////////////////////////////////////////
// Step II: Default Debug print function Module
//

// #define SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC
// #undef SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

// Default debug print function config
#ifdef SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

// default ext buffer size for default debug print function
// This macro will tell the default debug print function to create a
// buffer owning the length: `48 + _GMP_CHAR_EXT`.
// This value is used in `gmp_main.cpp`
//
#ifndef _GMP_CHAR_EXT
#define _GMP_CHAR_EXT ((48))
#endif // _GMP_CHAR_EXT

// debug output function
// This macro specify the default debug output function.
//
#ifndef gmp_dbg_prt
#define gmp_dbg_prt gmp_dbg_prt_fn
#endif // gmp_dbg_prt

#endif // SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC


//////////////////////////////////////////////////////////////////////////
// Step III: Default Memory management Module
//

// ....................................................................//
// block memory management module
//

// Default heap memory space
// The parameter is used in `gmp_cport.h`
//
#ifndef GMP_DEFAULT_HEAP_SIZE
#define GMP_DEFAULT_HEAP_SIZE ((1536))
#endif // GMP_DEFAULT_HEAP_SIZE

// Default heap size, byte(s)
// The parameter is used in `block_mem.c`
//
#ifndef GMP_DEFAULT_BUFFER_SIZE
#define GMP_DEFAULT_BUFFER_SIZE ((32))
#endif // GMP_DEFAULT_BUFFER_SIZE

// ....................................................................//
// + pool memory management module
//


//////////////////////////////////////////////////////////////////////////
// Step IV: Default peripheral config
//

// ....................................................................//
// + Watch Dog module
//


// ....................................................................//
// + GPIO module
//

