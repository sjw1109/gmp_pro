// abolish

/**
 * @file gmp_peripheral_definition.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

#ifndef _FILE_GMP_PERIPHERAL_DEFINITION_H_
#define _FILE_GMP_PERIPHERAL_DEFINITION_H_

//////////////////////////////////////////////////////////////////////////
// GPIO definition

typedef enum _tag_gmp_port_t
{
	GMP_GPIO_INPUT = 0,
	GMP_GPIO_OUTPUT = 1
}gmp_gpio_mode_t;



//////////////////////////////////////////////////////////////////////////
// UART definition

// A basic content of UART peripheral,
// user may utilize this structure to receive and transmit message via GMP-UART port.
typedef struct _tag_uart_content
{
	data_gt* text;
	size_gt length;
}uart_content_t;


//////////////////////////////////////////////////////////////////////////
// SPI definition

typedef struct _tag_spi_reg_content
{
	uint32_t address;
	size_gt address_length;

	uint32_t data;
	size_gt data_length;
}spi_reg_content;

#endif // _FILE_GMP_PERIPHERAL_DEFINITION_H_
