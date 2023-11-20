


#ifndef _FILE_EXAMPLE_H_
#define _FILE_EXAMPLE_H_

//////////////////////////////////////////////////////////////////////////
// peripheral handle type
#ifndef GMP_PHY_HANDLE_T
#define GMP_PHY_HANDLE_T void*
#endif

#ifndef GMP_PORT_IIC_HANDLE_T
#define GMP_PORT_IIC_HANDLE_T		void *
#endif

#ifndef GMP_PORT_UART_HANDLE_T
#define GMP_PORT_UART_HANDLE_T		void *
#endif

#ifndef GMP_PORT_SPI_HANDLE_T
#define GMP_PORT_SPI_HANDLE_T		void *
#endif

#ifndef GMP_PORT_IIS_HANDLE_T
#define GMP_PORT_IIS_HANDLE_T		void *
#endif

#ifndef GMP_PORT_USART_HANDLE_T
#define GMP_PORT_USART_HANDLE_T		void *
#endif


//////////////////////////////////////////////////////////////////////////
// basic element type
#ifndef GMP_DATA_ELEMENT_T
#define GMP_DATA_ELEMENT_T char
#endif

#ifndef GMP_LENGTH_T
#define GMP_LENGTH_T uint32_t
#endif

#ifndef GMP_CMD_T
#define GMP_CMD_T uint32_t 
#endif

//////////////////////////////////////////////////////////////////////////
// Others

// Function descriptor
#ifndef WEAK_FUNCTION
#if defined GMP_WINDOWS
#define WEAK_FUNCTION 
#else
#define WEAK_FUNCTION __weak
#endif
#endif

#endif // _FILE_EXAMPLE_H_
