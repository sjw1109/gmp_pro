
// This file provide the default type of the whole environment.
// This file should only be included to a headers after the chip configure is included.

#ifndef _FILE_CHIP_DEFAULT_TYPE_H_
#define _FILE_CHIP_DEFAULT_TYPE_H_

//////////////////////////////////////////////////////////////////////////
// Step I Necessary macros
#ifndef NULL
#define NULL ((0))
#endif // NULL


//////////////////////////////////////////////////////////////////////////
// Step II peripheral handle type
// 

// ....................................................................//
// handle of UART (Universal Asynchronous Receiver/Transmitter)
//
#ifndef GMP_PORT_HUART_T
#define GMP_PORT_HUART_T		void *
#endif // GMP_PORT_HUART_T

typedef GMP_PORT_HUART_T huart_gt;


// ....................................................................//
// handle of USART (Universal Synchronous/Asynchronous Receiver/Transmitter)
//
#ifndef GMP_PORT_HUSART_T
#define GMP_PORT_HUSART_T		void *
#endif // GMP_PORT_HUSART_T

typedef GMP_PORT_HUSART_T husart_gt;


// ....................................................................//
// handle of IIC (Inter-Integrated Circuit)
//
#ifndef GMP_PORT_HI2C_T
#define GMP_PORT_HI2C_T			void *
#endif // GMP_PORT_HI2C_T

typedef GMP_PORT_HI2C_T hiic_gt;


// ....................................................................//
// handle of IIS (Inter-IC Sound)
//
#ifndef GMP_PORT_HIIS_T
#define GMP_PORT_HIIS_T			void *
#endif // GMP_PORT_HIIS_T

typedef GMP_PORT_HIIS_T hiis_gt;


// ....................................................................//
// handle of SPI (Serial Peripheral Interface Bus)
//
#ifndef GMP_PORT_HSPI_T
#define GMP_PORT_HSPI_T			void *
#endif // GMP_PORT_HSPI_T

typedef GMP_PORT_HSPI_T hspi_gt;


// ....................................................................//
// handle of GPIO (General-purpose input/output)
//
#ifndef GMP_PORT_HGPIO_T
// WARN: if this default GPIO handle is used the GPIO function is out-of-service,
// so any other code which will use GPIO should judge the following macro,
// in order to ensure the GPIO handle is implemented.
#define GMP_GPIO_NOT_IMPLEMENTED

#define GMP_PORT_HGPIO_T        void *
#endif // GMP_PORT_HGPIO_T

typedef GMP_PORT_HGPIO_T hgpio_gt;


//////////////////////////////////////////////////////////////////////////
// Step III Basic element type


// ....................................................................//
// basic element data type
// This type is smallest unit of the chip
// generally, it's a 8-bit number.
//
#ifndef GMP_PORT_DATA_T
#define GMP_PORT_DATA_T				    char
#define GMP_PORT_DATA_SIZE_PER_BITS		(8)
#define GMP_PORT_DATA_SIZE_PER_BYTES    (1)
#endif // GMP_PORT_DATA_T

typedef GMP_PORT_DATA_T data_gt;


// ....................................................................//
// basic element data type which is fast one
// This type is determined by the width of the chip data bus.
// This type is only designed for flags, 
// so user should treat it as a 8-bit type.
// 
#ifndef GMP_PORT_FAST_T
#define GMP_PORT_FAST_T                int_fast32_t
#define GMP_PORT_FAST_SIZE_PER_BITS    (32)
#define GMP_PORT_FAST_SIZE_PER_BYTES   (4)
#endif // GMP_PORT_FAST_T

typedef GMP_PORT_FAST_T fast_gt;


// ....................................................................//
// basic element data type, which is for fast at least 16-bit type
// Any code may only user the first 16 bits contents
//
#ifndef GMP_PORT_FAST16_T
#define GMP_PORT_FAST16_T              int_fast16_t
#define GMP_PORT_FAST16_SIZE_PER_BITS  (16)
#define GMP_PORT_FAST16_SIZE_PER_BYTES (2)
#endif // GMP_PORT_FAST16_T

typedef GMP_PORT_FAST16_T fast16_gt;


// ....................................................................//
// basic element data type, which is for fast at least 16-bit type
// Any code may only user the first 16 bits contents
//
#ifndef GMP_PORT_FAST32_T
#define GMP_PORT_FAST32_T              int_fast32_t
#define GMP_PORT_FAST32_SIZE_PER_BITS  (32)
#define GMP_PORT_FAST32_SIZE_PER_BYTES (4)
#endif // GMP_PORT_FAST32_T

typedef GMP_PORT_FAST32_T fast32_gt;


// ....................................................................//
// basic element of timing containing
//
#ifndef GMP_PORT_TIME_T
#define GMP_PORT_TIME_T                uint32_t
#define GMP_PORT_TIME_SIZE_PER_BITS    (32)
#define GMP_PORT_TIME_SIZE_PER_BYTES   (4)

// This is an addition item for time type
// This value will used to calculate time.
//
#define GMP_PORT_TIME_MAXIMUM          (UINT32_MAX)
#endif // GMP_PORT_TIME_T

typedef GMP_PORT_TIME_T time_gt;


// ....................................................................//
// basic element of timing containing
//
#ifndef GMP_PORT_SIZE_T
#define GMP_PORT_SIZE_T                uint32_t
#define GMP_PORT_SIZE_SIZE_PER_BITS    (32)
#define GMP_PORT_SIZE_SIZE_PER_BYTES   (4)
#endif // GMP_PORT_SIZE_T

typedef GMP_PORT_SIZE_T size_gt;


// ....................................................................//
// basic element of address containing
//
#ifndef GMP_PORT_ADDR_T
#define GMP_PORT_ADDR_T                uint32_t
#define GMP_PORT_ADDR_SIZE_PER_BITS    (32)
#define GMP_PORT_ADDR_SIZE_PER_BYTES   (4)
#endif // GMP_PORT_ADDR_T

typedef GMP_PORT_ADDR_T addr_gt;


// ....................................................................//
// basic element of address offset containing
//
#ifndef GMP_PORT_DIFF_T
#define GMP_PORT_DIFF_T                int32_t
#define GMP_PORT_DIFF_SIZE_PER_BITS    (32)
#define GMP_PORT_DIFF_SIZE_PER_BYTES   (4)
#endif // GMP_PORT_DIFF_T

typedef GMP_PORT_DIFF_T diff_gt;


// ....................................................................//
// basic container of parameters
//
#ifndef GMP_PORT_PARAM_T
#define GMP_PORT_PARAM_T                int32_t
#define GMP_PORT_PARAM_SIZE_PER_BITS    (32)
#define GMP_PORT_PARAM_SIZE_PER_BYTES   (4)
#endif // GMP_PORT_PARAM_T

typedef GMP_PORT_PARAM_T param_gt;



// #ifndef GMP_PORT_SIZE_T
// #define GMP_PORT_SIZE_T				uint32_t
// #endif // GMP_PORT_SIZE_T
// 
// #ifndef GMP_PORT_ADDR_T
// #define GMP_PORT_ADDR_T				uint32_t
// #endif // GMP_PORT_ADDR_T
// 
// #ifndef GMP_PORT_TIME_T
// #define GMP_PORT_TIME_T				uint32_t
// #endif // GMP_PORT_TIME_T
// 
// #ifndef GMP_MAX_TIME_ITEM
// #define GMP_MAX_TIME_ITEM			(((uint32_t)-1))
// #endif // GMP_MAX_TIME_ITEM
// 
// #ifndef GMP_PORT_WPARAM_T
// #define GMP_PORT_WPARAM_T			uint32_t
// #endif // GMP_PORT_WPARAM_T
// 
// #ifndef GMP_PORT_TIMER_T
// #define GMP_PORT_TIMER_T			uint32_t
// #endif // GMP_PORT_TIMER_T
// 
// #ifndef GMP_PORT_ELEMENT_T
// #define GMP_PORT_ELEMENT_T          uint8_t
// #endif // GMP_PORT_ELEMENT_T

#endif // _FILE_CHIP_DEFAULT_TYPE_H_


//////////////////////////////////////////////////////////////////////////
// Translate default type to standard type name

// #ifndef _DEFAULT_TYPEDEF_
// #define _DEFAULT_TYPEDEF_
// 
// // translate macro into type name by standard name rule
// typedef GMP_PHY_HANDLE_T gmp_phy_handle_t;
// typedef GMP_PORT_DATA_T data_gt;
// typedef GMP_PORT_SIZE_T size_gt;
// typedef GMP_PORT_ADDR_T addr_gt;
// typedef GMP_PORT_TIME_T time_gt;
// typedef GMP_PORT_WPARAM_T gmp_param_t;
// typedef GMP_PORT_TIMER_T time_gt;
// typedef GMP_PORT_ELEMENT_T fast_gt;
// //typedef GMP_PORT_CMD_T gmp_cmd_t;
// 
// 
// // peripheral type 
// #ifdef GMP_PORT_IIC_HANDLE_T
// typedef GMP_PORT_IIC_HANDLE_T gmp_hiic_t;
// #endif // GMP_PORT_IIC_HANDLE_T
// 
// #ifdef GMP_PORT_UART_HANDLE_T
// typedef GMP_PORT_UART_HADNLE_T gmp_huart_t;
// #endif // GMP_PORT_UART_HANDLE_T
// 
// 
// #endif // _DEFAULT_TYPEDEF_
// 

