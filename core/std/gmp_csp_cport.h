/**
 * @file gmp_csp_cport.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief This file contains all the functions that User should implement it.
 * @version 0.1
 * @date 2024-09-28
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include "core/std/default.types.h"
#include <core/dev/devif.h>

#ifndef _FILE_GMP_CSP_CPORT_H_
#define _FILE_GMP_CSP_CPORT_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // GPIO general port

    /**
     * @brief Set mode of gpio port, mode 0: input, mode 1: output.
     * @param hgpio handle of gpio. Type of GPIO handle is given by CSP.
     * @param mode target mode of GPIO. mode 0 is input mode, 1 is output mode.
     */
    void gmp_gpio_set_mode(hgpio_gt *hgpio, fast_gt mode);

    /**
     * @brief Write GPIO port. This port must be an output port.
     * Or, undefined things may happen.
     * @param hgpio handle of GPIO
     * @param level target electrical level of GPIO port.
     */
    void gmp_gpio_write(hgpio_gt *hgpio, fast_gt level);

    /**
     * @brief Read GPIO port, This port should be an input port.
     * Or the return value is undefined.
     * @param hgpio handle of GPIO
     * @return fast_gt return GPIO electrical level
     */
    fast_gt gmp_gpio_read(hgpio_gt *hgpio);

    /**
     * @brief Set GPIO electrical level to high.
     * if GPIO mode is not output mode, the result is undefined.
     * @param hgpio handle of GPIO
     */
    void gmp_gpio_set(hgpio_gt *hgpio);

    /**
     * @brief Set GPIO electrical level to low.
     * if GPIO mode is not output mode, the result is undefined.
     * @param hgpio handle of GPIO
     */
    void gmp_gpio_clear(hgpio_gt *hgpio);

    //////////////////////////////////////////////////////////////////////////
    // UART general port

    /**
     * @brief send data via UART
     * @param huart handle of UART
     * @param data half_duplex data interface
     */
    void gmp_uart_send(huart_gt *huart, half_duplex_ift *data);

    /**
     * @brief receive data via UART
     * @param huart handle of UART
     * @param data half_duplex data interface
     */
    void gmp_uart_recv(huart_gt *huart, half_duplex_ift *data);

    /**
     * @brief bind a duplex data buffer to UART channel.
     * @param huart handle of UART
     * @param data duplex data buffer
     */
    void gmp_uart_bind_duplex_dma(huart_gt *huart, duplex_ift *data);

    /**
     * @brief start UART listen to receive routine
     * @param huart handle of UART
     */
    void gmp_uart_listen(huart_gt *huart);

    /**
     * @brief Get UART listen status, return current receive bytes number.
     * @param huart
     * @return size_gt size of received bytes.
     */
    size_gt gmp_uart_get_listen_status(huart_gt *huart);

    /**
     * @brief start UART consign to transmit routine.
     * @param huart handle of UART
     */
    void gmp_uart_consign(huart_gt *huart);

    /**
     * @brief Get UART consign status, return if consign routine is free.
     * @param huart
     * @return fast_gt
     */
    fast_gt gmp_uart_get_consign_status(huart_gt *huart);

    //////////////////////////////////////////////////////////////////////////
    // SPI general port

    /**
     * @brief send data via half duplex SPI
     * @param spi handle of SPI
     * @param data half_duplex data interface
     */
    void gmp_spi_send(hspi_gt *spi, half_duplex_ift *data);

    /**
     * @brief receive data via SPI
     * @param spi handle of SPI
     * @param data half_duplex data interface
     */
    void gmp_spi_recv(hspi_gt *spi, half_duplex_ift *data);

    /**
     * @brief receive and transmit data via SPI interface
     * This function should only be called in SPI duplex mode.
     * @param spi handle of SPI
     * @param data duplex data interface
     */
    void gmp_spi_send_recv(hspi_gt *spi, duplex_ift *data);

    //////////////////////////////////////////////////////////////////////////
    // IIC general port

    /**
     * @brief IIC memory function, send a IIC memory frame.
     * @param iic handle of IIC
     * @param mem memory send message
     */
    void gmp_iic_mem_send(hiic_gt *iic, iic_memory_ift *mem);

    /**
     * @brief IIC memory function, recveive a IIC memory frame.
     * @param iic handle of IIC
     * @param mem memory receive message
     */
    void gmp_iic_mem_recv(hiic_gt *iic, iic_memory_ift *mem);

    /**
     * @brief IIC device send function 
     * @param iic handle of IIC
     * @param msg IIC send message
     */
    void gmp_iic_send(hiic_gt *iic, half_duplex_with_addr_ift *msg);

    /**
     * @brief IIC device receive function
     * @param iic handle of IIC
     * @param msg IIC receive message
     */
    void gmp_iic_recv(hiic_gt *iic, half_duplex_with_addr_ift *msg);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GMP_CSP_CPORT_H_
