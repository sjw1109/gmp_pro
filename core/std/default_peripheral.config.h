/**
 * @file default_peripheral.config.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

#ifndef _FILE_DEFAULT_PERIPHERAL_CONFIG_H_
#define _FILE_DEFAULT_PERIPHERAL_CONFIG_H_

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

// ....................................................................//
// Handle of debug print peripheral
//
#ifndef GMP_PORT_DBG_PRINT_HANDLE_T

// if this macro is defined the dbg_print function may be prevented.
#define GMP_PORT_DBG_PRINT_NOT_IMPLEMENTED

#define GMP_PORT_DBG_PRINT_HANDLE_T void *

#endif // GMP_PORT_DBG_PRINT_HANDLE_T

typedef GMP_PORT_DBG_PRINT_HANDLE_T dbgptr_gt;


#endif //_FILE_DEFAULT_PERIPHERAL_CONFIG_H_
