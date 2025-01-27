/**
 * @file csp.typedef.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */


#ifndef _FILE_CSP_TYPE_DEF_H_
#define _FILE_CSP_TYPE_DEF_H_


// This file is for STM32 series micro controller


// STM32 BASIC DATA TYPE
#define GMP_PORT_DATA_T				    int16_t
#define GMP_PORT_DATA_SIZE_PER_BITS		(16)
#define GMP_PORT_DATA_SIZE_PER_BYTES    (2)

// FAST TYPES

#define GMP_PORT_FAST8_T                 int16_t
#define GMP_PORT_FAST8_SIZE_PER_BITS     (16)
#define GMP_PORT_FAST8_SIZE_PER_BYTES    (2)
	
#define GMP_PORT_FAST16_T                int16_t
#define GMP_PORT_FAST16_SIZE_PER_BITS    (16)
#define GMP_PORT_FAST16_SIZE_PER_BYTES   (2)

// ....................................................................//
// basic element data type
// This type is smallest unit of the chip
// generally, it's a 8-bit number.
//
#ifndef GMP_PORT_DATA_T
#define GMP_PORT_DATA_T              uint16_t
#define GMP_PORT_DATA_SIZE_PER_BITS  (16)
#define GMP_PORT_DATA_SIZE_PER_BYTES (2)
#endif // GMP_PORT_DATA_T

// ....................................................................//
// basic element data type which is fast one
// This type is determined by the width of the chip data bus.
//
#ifndef GMP_PORT_FAST_T
#define GMP_PORT_FAST_T              int_fast16_t
#define GMP_PORT_FAST_SIZE_PER_BITS  (16)
#define GMP_PORT_FAST_SIZE_PER_BYTES (2)
#endif // GMP_PORT_FAST_T

// ....................................................................//
// basic container of PWM results
//
#ifndef GMP_PORT_PWM_T
#define GMP_PORT_PWM_T              uint16_t
#define GMP_PORT_PWM_SIZE_PER_BITS  (16)
#define GMP_PORT_PWM_SIZE_PER_BYTES (2)
#endif // GMP_PORT_PWM_T

#endif 


