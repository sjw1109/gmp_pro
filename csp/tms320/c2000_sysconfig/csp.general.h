/**
 * @file csp.general.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-01-18
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#ifndef _FILE_CSP_GENERAL_H_
#define _FILE_CSP_GENERAL_H_

//
// ClockTree Tool
//
#include "clocktree.h"

//
// C2000Ware SysConfig 
//  - Software - Device Support
//
#ifndef _C2000_CSP_DISABLE_DEVICE_SUPPORT_
#include "device.h"
#endif // _C2000_CSP_DISABLE_DEVICE_SUPPORT_

//
// C2000Ware SysConfig
//
#include "board.h"

// 
// Linker CMD Tool
//
#ifndef _C2000_CSP_DISBALE_LINKER_CMD_TOOL_
#include "device_cmd.h"
#endif // _C2000_CSP_DISBALE_LINKER_CMD_TOOL_

//
// C200Ware Libraries
//
#ifndef _C2000_CSP_DISABLE_C2000WARE_LIBRARY_
#include "c2000ware_libraries.h"
#endif // _C2000_CSP_DISABLE_C2000WARE_LIBRARY_

//
// C2000 CLB Configuration Tool 
//
#ifndef _C2000_CSP_DISABLE_CLB_CONFIG_TOOL_
#include "clb_config.h"
#endif // _C2000_CSP_DISABLE_CLB_CONFIG_TOOL_


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CSP_GENERAL_H_
