
// This file provide the default type of the whole environment.

#ifndef _FILE_CHIP_DEFAULT_TYPE_H_
#define _FILE_CHIP_DEFAULT_TYPE_H_

//////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL ((0))
#endif // NULL

#ifndef nullptr
#define nullptr (0x00000000)
#endif // nullptr


//////////////////////////////////////////////////////////////////////////
// peripheral handle type
#ifndef GMP_PHY_HANDLE_T
#define GMP_PHY_HANDLE_T		void*
#endif

#ifndef GMP_PORT_HANDLE_T
#define GMP_PORT_HANDLE_T       void * 
#endif // GMP_PORT_HANDLE_T

#ifndef GMP_PORT_HI2C_T
#define GMP_PORT_HI2C_T			void *
#endif // GMP_PORT_HI2C_T

#ifndef GMP_PORT_HUART_T
#define GMP_PORT_HUART_T		void *
#endif // GMP_PORT_HUART_T

#ifndef GMP_PORT_HSPI_T
#define GMP_PORT_HSPI_T			void *
#endif // GMP_PORT_HSPI_T

#ifndef GMP_PORT_HIIS_T
#define GMP_PORT_HIIS_T			void *
#endif // GMP_PORT_HIIS_T

#ifndef GMP_PORT_HUSART_T
#define GMP_PORT_HUSART_T		void *
#endif // GMP_PORT_HUSART_T


//////////////////////////////////////////////////////////////////////////
// Basic element type

	
// Port Type Definition
// GMP_PORT_DATA_T: the smallest unit for the chip, generally, it's a 8-bit number.
// GMP_BASIC_SIZE_PER_BITS: the smallest unit length.
// GMP_PORT_SIZE_T: the default counter type.
// GMP_PORT_ADDR_T: the default address type.

#ifndef GMP_PORT_DATA_T
#define GMP_PORT_DATA_T				char
#endif // GMP_PORT_DATA_T

#ifndef GMP_BASIC_SIZE_PER_BITS
#define GMP_BASIC_SIZE_PER_BITS		(8)  // Basic size of data_element_ts
#endif // GMP_BASIC_SIZE_PER_BITS

#ifndef GMP_PORT_SIZE_T
#define GMP_PORT_SIZE_T				uint32_t
#endif // GMP_PORT_SIZE_T

#ifndef GMP_PORT_ADDR_T
#define GMP_PORT_ADDR_T				uint32_t
#endif // GMP_PORT_ADDR_T

#ifndef GMP_PORT_TIME_T
#define GMP_PORT_TIME_T				uint32_t
#endif // GMP_PORT_TIME_T

#ifndef GMP_MAX_TIME_ITEM
#define GMP_MAX_TIME_ITEM			(((uint32_t)-1))
#endif // GMP_MAX_TIME_ITEM

#ifndef GMP_PORT_WPARAM_T
#define GMP_PORT_WPARAM_T			uint32_t
#endif // GMP_PORT_WPARAM_T

#endif // _FILE_CHIP_DEFAULT_TYPE_H_


//////////////////////////////////////////////////////////////////////////
// Translate default type to standard type name

#ifndef _DEFAULT_TYPEDEF_
#define _DEFAULT_TYPEDEF_

// translate macro into type name by standard name rule
typedef GMP_PHY_HANDLE_T gmp_phy_handle_t;
typedef GMP_PORT_DATA_T gmp_data_t;
typedef GMP_PORT_SIZE_T gmp_size_t;
typedef GMP_PORT_ADDR_T gmp_addr_t;
typedef GMP_PORT_TIME_T gmp_time_t;
typedef GMP_PORT_WPARAM_T gmp_param_t;

//typedef GMP_PORT_CMD_T gmp_cmd_t;


// peripheral type 
#ifdef GMP_PORT_IIC_HANDLE_T
typedef GMP_PORT_IIC_HANDLE_T gmp_hiic_t;
#endif // GMP_PORT_IIC_HANDLE_T

#ifdef GMP_PORT_UART_HANDLE_T
typedef GMP_PORT_UART_HADNLE_T gmp_huart_t;
#endif // GMP_PORT_UART_HANDLE_T


#endif // _DEFAULT_TYPEDEF_


