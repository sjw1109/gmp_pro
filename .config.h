// Software release: GMP Core Library
// Release date: 
// Author:           JsScript
// Copyright
// Source repository: https://github.com/javnson/gmp_pro

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied.
// See the License for the specific language governing permissions and
// limitations under the License.


// This file contains all the leading macro definitions.
// This file may configure the operating mode of the whole library.

// User should config this file first.
// The following macro definitions will applied to all the modules.

#ifndef GENERAL_MOTOR_PLAT

// GMP library version definition
#define GENERAL_MOTOR_PLAT

// Library Serial numbers
#define GMP_SERIAL_NUM     ((00006ul))

#define GMP_AUTO 
#define GMP_AUTO_STM32   1
#define GMP_AUTO_TIC2000 2
#define GMP_AUTO_WINDOWS 3
#define GMP_AUTO_LINUX   4

// GMP support chip select
#define MASTERCHIP GMP_AUTO_STM32

// GMP Master chip set select
#define MASTER_CHIP_SET NULL

// little-endian OR big-endian, 
// use only one of them
#define LITTLE_ENDIAN
// #define BIG_ENDIAN

// enable int64_t & uint64_t
#define SPECIFY_ENABLE_INTEGER64

// enable IQ math library
#define USING_FIXED_LIBRARY

// Compiler options
// using CCS C2000 compiler
//#define COMPILER_CCS_C2000

// Enable unimplemented function warning 
#define SPECIFY_ENABLE_UNIMPL_FUNC_WARNING

// When meet an unimplemented function just stuck the program
#define SPECIFY_STUCK_WHEN_UNIMPL_FUNC 

// enable the test environment
// This switch is only for developing
// As a common user, we strongly suggest you disable the macro
//#define SPECIFY_ENABLE_TEST_ENVIRONMENT

// Specify the test environment is PC operation system
//#define SPECIFY_PC_TEST_ENV

// specify what happened when error happened.
// When you enable the flag, the system will stuck when error occurred.
// Or, when error occurred, the program will continue running.
//#define SPECIFY_STUCK_WHEN_ERROR

// specify GMP should feed watch dog
// The program will invoke feed watch dog
#define SPECIFY_ENABLE_FEED_WATCHDOG

// Disable the GMP LOGO output
// The code size will greatly lessen
//#define SPECIFY_DISABLE_GMP_LOGO

//////////////////////////////////////////////////////////////////////////
// This module permit GMP auto detect what chip is in use and configure it automatically.
#if defined GMP_AUTO

#if (MASTERCHIP == GMP_AUTO_WINDOWS)

// redirect debug print to std C library `printf`
#define gmp_dbg_prt printf

// Specify the environment is PC test environment
#ifndef SPECIFY_PC_TEST_ENV
#define SPECIFY_PC_TEST_ENV
#endif // SPECIFY_PC_TEST_ENV

// Number of Test Cycles
#define SPECIFY_PC_TEST_CYCLE_CNT 1000

// Enable GMP LOGO
#undef SPECIFY_DISABLE_GMP_LOGO
//#define SPECIFY_DISABLE_GMP_LOGO

#endif // MASTERCHIP

#endif // GMP_AUTO


//////////////////////////////////////////////////////////////////////////
// MEMORY CONTROLLER SETTINGS
// Memory Management Deposit
// 
// GMP/core/mm
// enable the block memory management
// if you disable the block, the block memory will not active.
#define SPECIFY_GMP_BLOCK_MEMORY_ENABLE

// specify the initial global memory heap bank.
#define GMP_GLOBAL_MEMORY_HEAP_HANDLE default_mem_heap

// specify the default memory control function, allocation and free
#define GMP_BLOCK_ALLOC_FUNC(size) gmp_block_alloc(default_mem_heap, size)
#define GMP_BLOCK_FREE_FUNC(ptr) gmp_block_free(default_mem_heap, ptr)

// #define GMP_BLOCK_ALLOC_FUNC(size) alloc(size)
// #define GMP_BLOCK_FREE_FUNC(ptr) free(ptr)

// Specify the default heap size
#define GMP_DEFAULT_HEAP_SIZE ((1536))

// The default print function rely on the allocation function,
// so the alloc_function and free_function is necessary
#define SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

// In default case, the print function may use gmp_malloc function,
// if you have to avoid the malloc and free you should enable the following function 
#define SPECIFY_DISABLE_DYNAMIC_ALLOC_OF_DBGPTR


#define GMP_TIMER_BASE_FREQ 64000000

//////////////////////////////////////////////////////////////////////////
// Device and IO device controller Settings
// Device and Peripheral Management Deposit
// 
// NOTICE: if you disable the module you will lose the ability of ext module
// 
// GMP/core/dev
#define GMP_PERIPHERAL_IO

// GMP IO device extention
// GMP/ext


//////////////////////////////////////////////////////////////////////////
// Timer of the STM32 device

// The base frequency of timer peripheral
// generally, this macro will be implemented by chip selection and auto config.
//#define GMP_TIMER_BASE_FREQ ((72000000U))


//////////////////////////////////////////////////////////////////////////
// GMP WF settings
// Work flow Management Deposit
// 
// GMP/core/workflow


// GMP/core/workflow_scheduling


//////////////////////////////////////////////////////////////////////////
// GMP CTL(Controller Template Library)

// Enable GMP CTL module
#define SPECIFY_ENABLE_GMP_CTL

// The rest of GMP CTL config please complete in <ctrl_config.h>


//////////////////////////////////////////////////////////////////////////
// Functional blocks



//////////////////////////////////////////////////////////////////////////
// validate parameters
#if defined BIG_ENDIAN && defined LITTLE_ENDIAN
#error "You should chose big-endian or little-endian."
#endif // BIG_ENDIAN && LITTLE_ENDIAN


//////////////////////////////////////////////////////////////////////////
// include default configuration
//
#include <core/std/default.config.h>

#endif // GENERAL_MOTOR_PLAT
