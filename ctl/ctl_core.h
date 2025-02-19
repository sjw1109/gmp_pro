/**
 * @file ctl_core.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// Invoke All the GMP features
#include <gmp_core.h>

#if !defined SPECIFY_DISABLE_GMP_CTL

// Invoke CTL configurations 
#include <ctl/ctl.config.h>

// Invoke Necessary math supports
#include <ctl/math_block/gmp_math.h>


#if defined SPECIFY_ENABLE_CTL_FRAMEWORK_NANO



// This file is provided by user,
// User should define controller related function in this header file
#include <ctl_main.h>

#include <ctl_interface.h>

#include <ctl/framework/ctl_dispatch.h>

// Invoke necessary CTL libraries
#include <ctl/framework/ctl_nano.h>

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO


#endif // SPECIFY_ENABLE_GMP_CTL







// register default controller objects
// and default controller object init function

//// default header object
//#ifndef CTL_OBJECT_HEADER_TYPE
//#define CTL_OBJECT_HEADER_TYPE ctl_object_nano_t
//#endif // CTL_OBJECT_HEADER_TYPE
//
//typedef CTL_OBJECT_HEADER_TYPE ctl_object_header_t;
//
//// default header initialize function 
//#ifndef CTL_OBJECT_HEADER_INIT_FUNC
//#define CTL_OBJECT_HEADER_INIT_FUNC init_ctl_obj_nano_header
//#endif // CTL_OBJECT_HEADER_INIT_FUNC
//
//#define ctl_init_header(x) init_ctl_obj_nano_header(x)
