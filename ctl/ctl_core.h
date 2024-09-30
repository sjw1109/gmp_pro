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
#include <core/gmp_core.h>

// Invoke CTL configurations 
#include <ctl/ctl.config.h>

// Invoke Necessary math supports
#include <ctl/component/common/gmp_math.h>

// necessary components
#include <ctl/component/interface_util/adc_channel.h>
#include <ctl/component/interface_util/pwm_channel.h>

#include <ctl/component/common/divider.h>

// Invoke necessary CTL libraries
#include <ctl/framework/ctl_nano.h>

#include <ctl/component/common/pid.h>
#include <ctl/component/common/slope_lim.h>
#include <ctl/component/common/fusing.h>


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
