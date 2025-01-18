/**
 * @file csp.general.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// This file is designed for DSP auto configuration.

//////////////////////////////////////////////////////////////////////////
// Step I import DSP C2000 Headers
//

// The header file is provided by C2000Ware.
// You should install the software firstly.
// More informations please visit the TI website:
// 
// https://www.ti.com/tool/C2000WARE?keyMatch=c2000ware
// 
// Step I: You may add the following three headers path:
// 
// + 1
// C2000Ware_<version>/device_support/<chip_select>
//
// + 2
// C2000Ware_<version>/device_support/<chip_select>/headers/include.
//
// + 3
// C2000Ware_<version>/device_support/<chip_select>/common/include.
//
// For instance, for TMS320F28069, with default installed C2000Ware_4_00_00_00 Software,
// you should add the following include scanning path:
// + C:\ti\c2000\C2000Ware_4_00_00_00\device_support\f2806x
// + C:\ti\c2000\C2000Ware_4_00_00_00\device_support\f2806x\headers\include
// + C:\ti\c2000\C2000Ware_4_00_00_00\device_support\f2806x\common\include
//
// Step II: You may add correct source files
//  
// + 1 
// C2000Ware_<version>\device_support\f2806x\headers\source\F2806x_GlobalVariableDefs.c
//
// + 2 
// C2000Ware_<version>\device_support\f2806x\common\source\*
// except, F2806x_SWPrioritizedDefaultIsr.c, F2806x_SWPrioritizedPieVect.c, 
//

// Device Header file and Examples Include File
#include <common/include/DSP28x_Project.h>

// invoke gpio model
#include <csp/dspc2000/common/gpio_model.c2000.h>


#ifndef _FILE_DSP_C2000_GENERAL_H_
#define _FILE_DSP_C2000_GENERAL_H_


// System parameters
#define SYSTEM_MAIN_CLK_MHz (90) // MHz




//////////////////////////////////////////////////////////////////////////
// Step II Chip default configuration
//

// ....................................................................//
// Chip frequency
//
#ifndef GMP_TIMER_BASE_FREQ
#define GMP_TIMER_BASE_FREQ (100000000U)
#endif // GMP_TIMER_BASE_FREQ


//////////////////////////////////////////////////////////////////////////
// Step III Basic type definition

// ....................................................................//
// basic element data type
// This type is smallest unit of the chip
// generally, it's a 8-bit number.
//
#ifndef GMP_PORT_DATA_T
#define GMP_PORT_DATA_T				    uint16_t
#define GMP_PORT_DATA_SIZE_PER_BITS		(16)
#define GMP_PORT_DATA_SIZE_PER_BYTES    (2)
#endif // GMP_PORT_DATA_T


// ....................................................................//
// basic element data type which is fast one
// This type is determined by the width of the chip data bus.
// 
#ifndef GMP_PORT_FAST_T
#define GMP_PORT_FAST_T                int_fast16_t
#define GMP_PORT_FAST_SIZE_PER_BITS    (16)
#define GMP_PORT_FAST_SIZE_PER_BYTES   (2)
#endif // GMP_PORT_FAST_T

// ....................................................................//
// basic container of PWM results
//
#ifndef GMP_PORT_PWM_T
#define GMP_PORT_PWM_T                  uint16_t
#define GMP_PORT_PWM_SIZE_PER_BITS      (16)
#define GMP_PORT_PWM_SIZE_PER_BYTES     (2)
#endif // GMP_PORT_PWM_T



//typedef GMP_PORT_ADC_T pwm_gt;


// Basic handle definition
#define GMP_PORT_HGPIO_T    volatile struct GPIO_CTRL_REGS*
#define GMP_PORT_HIIC_T		volatile struct I2C_REGS*
#define GMP_PORT_HUART_T	volatile struct SCI_REGS*
#define GMP_PORT_HSPI_T		volatile struct SPI_REGS*
#define GMP_PORT_HCAN_T     volatile struct ECAN_REGS*



//////////////////////////////////////////////////////////////////////////
// Step IV implement of GPIO entity.


// Invoke default type
#include <core/std/default.types.h>

#include <core/std/gmp_peripheral_definition.h>

// Invoke peripheral headers

////////////////////////////////////////////////////////////////////////
// global variables

// CPU tick
extern time_gt g_cpu_hal_tick;

// CPU tick ISR
void cpu_tick_routine(void);


#endif // _FILE_DSP_C2000_GENERAL_H_


