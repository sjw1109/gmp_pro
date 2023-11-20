
// User config file
#include <bsp/user/user_config.h>

// invoke system basic type definition
#include <stdint.h>

// invoke standard error code
#include <core/std/error_code.h>

// invoke chip support package
#include <csp/chip_support_selection.h>

// invoke compiler support
#if defined COMPILER_AC6
#include <core/std/AC6_support.h>
#endif
#include <core/std/compiler_sup.h>



#ifndef _FILE_TYPEDEF_H_
#define _FILE_TYPEDEF_H_

// translate macro into type name by standard name rule
typedef GMP_PHY_HANDLE_T gmp_phy_handle_t;
typedef GMP_DATA_ELEMENT_T gmp_data_element_t;
typedef GMP_LENGTH_T gmp_length_t;
typedef GMP_CMD_T gmp_cmd_t;

// peripheral type 
#ifdef GMP_PORT_IIC_HANDLE_T
typedef GMP_PORT_IIC_HANDLE_T gmp_iic_handle_t;
#endif

// GLOBAL MACROS
#ifndef TIMEOUT_CNT
#define TIMEOUT_CNT 0X1FFF
#endif

#endif // _FILE_TYPEDEF_H_

