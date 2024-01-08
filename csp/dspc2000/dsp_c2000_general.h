
// DSP Hal library header
#if defined TMS320F2806x
#include "F2806x_Device.h"     // F2806x Header file Include File
#include "F2806x_Examples.h"   // F2806x Examples Include File
#elif defined TMS320F2803x

#elif defined TMS320F2802x


#endif

// Language Support
#include <stdint.h>
#include <assert.h>

// GMP standard error
#include <core/std/error_code.h>


#ifndef _FILE_DSP_C2000_GENERAL_H_
#define _FILE_DSP_C2000_GENERAL_H_

// Basic type definition
#define GMP_PORT_DATA_T			uint16_t 
#define GMP_BASIC_SIZE_PER_BITS ((16))
#define GMP_PORT_SIZE_T			uint16_t 

// Basic handle definition
#define GMP_PORT_HIIC_T volatile struct I2C_REGS *
#define GMP_PORT_HSPI_T volatile struct SPI_REGS * 
#define GMP_PORT_HUART_T volatile struct SCI_REGS *

// Default Type Definition
#include <csp/chip_default_type.h>

// Default device prototype
#include <core/dev/prototypes.h>

// Interface Function Prototype
#include <csp/port_function_prototype.h>

// Inline Function
#include <csp/dspc2000/dsp_c2000_inline.h>

// external functions
#ifdef __cplusplus
extern "C"
{
#endif
	// trans_init IIC peripheral
	void I2CA_Init(
		void
	);

	// trans_init SCI peripheral
	void SCI_init(
		gmp_phy_mapping_t* handle
	);

	
	
#ifdef __cplusplus
}
#endif

#endif // _FILE_DSP_C2000_GENERAL_H_



