

// Included Files

// The header file is provided by C2000Ware.
// In the folder C2000Ware_<version>/device_support/<chip_select>/common/include.

// Device Headerfile and Examples Include File
#include "DSP28x_Project.h"     


//// DSP Hal library header
//#if defined TMS320F2806x
//#include "F2806x_Device.h"     // F2806x Header file Include File
//#include "F2806x_Examples.h"   // F2806x Examples Include File
//#elif defined TMS320F2803x
//
//#elif defined TMS320F2802x
//
//
//#endif



#ifndef _FILE_DSP_C2000_GENERAL_H_
#define _FILE_DSP_C2000_GENERAL_H_

// Basic type definition
#define GMP_PORT_DATA_T			uint16_t 
#define GMP_BASIC_SIZE_PER_BITS ((16))
#define GMP_PORT_SIZE_T			uint16_t 

// Basic handle definition
#deifne GMP_PORT_HGPIO_T    volatile struct GPIO_CTRL_REGS*
#define GMP_PORT_HIIC_T		volatile struct I2C_REGS*
#define GMP_PORT_HUART_T	volatile struct SCI_REGS*
#define GMP_PORT_HSPI_T		volatile struct SPI_REGS*
#define GMP_PORT_HCAN_T     volatile struct ECAN_REGS*
//#define GMP_PORT_HIIS_T
//#define GMP_PORT_HUSART_T



#endif // _FILE_DSP_C2000_GENERAL_H_

#include <core/std/default_types.h>

