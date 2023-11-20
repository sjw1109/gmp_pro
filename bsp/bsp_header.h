
// User should include the file to introduce basic GMP support.

#include <bsp/user/user_config.h>


#ifdef GENERAL_MOTOR_PLAT

// standard typedef
#include <core/std/typedef.h>

// global variables and global functions
#include <core/std/global.h>

#ifdef GMP_MEMORY_MANAGEMENT
#include <core/mm/mem.h>
#endif // GMP_MEMORY_MANAGEMENT

#ifdef GMP_PERIPHERAL_IO
#include <core/dev/io_functions.h>
//#include <core/dev/iic.h>
#ifdef GMP_CPLUSPLUS_ENABLE
#include <core/dev/io_function.hpp>
#endif

// call peripheral mapping header
#include <bsp/user/peripheral_mapping.h>
#endif // GMP_PERIPHERAL_IO

#endif // GENERAL_MOTOR_PLAT



#ifndef _FILE_BSP_HEADER_H_
#define _FILE_BSP_HEADER_H_






#endif // _FILE_BSP_HEADER_H_
	