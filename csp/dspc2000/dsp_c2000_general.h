
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
// You may find the following headers in the following path:
// 
// C2000Ware_<version>/device_support/<chip_select>/common/include.
// 
// And you should add the path to the include scanning path.
//

// Device Header file and Examples Include File
#include <DSP28x_Project.h>

//#include <common/include/DSP28x_Project.h>


#ifndef _FILE_DSP_C2000_GENERAL_H_
#define _FILE_DSP_C2000_GENERAL_H_


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



// Basic handle definition
#define GMP_PORT_HGPIO_T    volatile struct GPIO_CTRL_REGS*
#define GMP_PORT_HIIC_T		volatile struct I2C_REGS*
#define GMP_PORT_HUART_T	volatile struct SCI_REGS*
#define GMP_PORT_HSPI_T		volatile struct SPI_REGS*
#define GMP_PORT_HCAN_T     volatile struct ECAN_REGS*



//////////////////////////////////////////////////////////////////////////
// Step IV implement of GPIO entity.



#endif // _FILE_DSP_C2000_GENERAL_H_

#include <core/std/default_types.h>

