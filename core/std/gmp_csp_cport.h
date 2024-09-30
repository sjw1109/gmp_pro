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
    gmp_stat_t gmp_hal_gpio_set_mode(gpio_halt *hgpio, fast_gt mode);

    /**
     * @brief Write GPIO port. This port must be an output port.
     * Or, undefined things may happen.
     * @param hgpio handle of GPIO
     * @param level target electrical level of GPIO port.
     */
    gmp_stat_t gmp_hal_gpio_write(gpio_halt *hgpio, fast_gt level);

    /**
     * @brief Read GPIO port, This port should be an input port.
     * Or the return value is undefined.
     * @param hgpio handle of GPIO
     * @return fast_gt return GPIO electrical level
     */
    fast_gt gmp_hal_gpio_read(gpio_halt *hgpio);

    /**
     * @brief Set GPIO electrical level to high.
     * if GPIO mode is not output mode, the result is undefined.
     * @param hgpio handle of GPIO
     */
    gmp_stat_t gmp_hal_gpio_set(gpio_halt *hgpio);

    /**
     * @brief Set GPIO electrical level to low.
     * if GPIO mode is not output mode, the result is undefined.
     * @param hgpio handle of GPIO
     */
    gmp_stat_t gmp_hal_gpio_clear(gpio_halt *hgpio);

    //////////////////////////////////////////////////////////////////////////
    // UART general port

    /**
     * @brief send data via UART
     * @param huart handle of UART
     * @param data half_duplex data interface
     */
    gmp_stat_t gmp_hal_uart_send(uart_halt *huart, half_duplex_ift *data);

    /**
     * @brief receive data via UART
     * @param huart handle of UART
     * @param data half_duplex data interface
     */
    gmp_stat_t gmp_hal_uart_recv(uart_halt *huart, half_duplex_ift *data);

    /**
     * @brief bind a duplex data buffer to UART channel.
     * @param huart handle of UART
     * @param data duplex data buffer
     */
    gmp_stat_t gmp_hal_uart_bind_duplex_dma(uart_halt *huart, duplex_ift *data);

    /**
     * @brief start UART listen to receive routine
     * @param huart handle of UART
     */
    gmp_stat_t gmp_hal_uart_listen(uart_halt *huart);

    /**
     * @brief Get UART listen status, return current receive bytes number.
     * @param huart
     * @return size_gt size of received bytes.
     */
    size_gt gmp_hal_uart_get_listen_status(uart_halt *huart);

    /**
     * @brief start UART consign to transmit routine.
     * @param huart handle of UART
     */
    gmp_stat_t gmp_hal_uart_consign(uart_halt *huart);

    /**
     * @brief Get UART consign status, return if consign routine is free.
     * @param huart
     * @return fast_gt
     */
    fast_gt gmp_hal_uart_get_consign_status(uart_halt *huart);

    //////////////////////////////////////////////////////////////////////////
    // SPI general port

    /**
     * @brief send data via half duplex SPI
     * @param spi handle of SPI
     * @param data half_duplex data interface
     */
    gmp_stat_t gmp_hal_spi_send(spi_halt *spi, half_duplex_ift *data);

    /**
     * @brief receive data via SPI
     * @param spi handle of SPI
     * @param data half_duplex data interface
     */
    gmp_stat_t gmp_hal_spi_recv(spi_halt *spi, half_duplex_ift *data);

    /**
     * @brief receive and transmit data via SPI interface
     * This function should only be called in SPI duplex mode.
     * @param spi handle of SPI
     * @param data duplex data interface
     */
    gmp_stat_t gmp_hal_spi_send_recv(spi_halt *spi, duplex_ift *data);

    //////////////////////////////////////////////////////////////////////////
    // IIC general port

    /**
     * @brief IIC memory function, send a IIC memory frame.
     * @param iic handle of IIC
     * @param mem memory send message
     */
    gmp_stat_t gmp_hal_iic_mem_send(iic_halt *iic, iic_memory_ift *mem);

    /**
     * @brief IIC memory function, recveive a IIC memory frame.
     * @param iic handle of IIC
     * @param mem memory receive message
     */
    gmp_stat_t gmp_hal_iic_mem_recv(iic_halt *iic, iic_memory_ift *mem);

    /**
     * @brief IIC device send function 
     * @param iic handle of IIC
     * @param msg IIC send message
     */
    gmp_stat_t gmp_hal_iic_send(iic_halt *iic, half_duplex_with_addr_ift *msg);

    /**
     * @brief IIC device receive function
     * @param iic handle of IIC
     * @param msg IIC receive message
     */
    gmp_stat_t gmp_hal_iic_recv(iic_halt *iic, half_duplex_with_addr_ift *msg);

    //////////////////////////////////////////////////////////////////////////
    // CAN general port

    
    //////////////////////////////////////////////////////////////////////////
    // WatchDog peripheral general interface

    /**
     * @brief This function may fresh IWDG counter.
     * This function should be implemented by CSP,
     * Every Loop routine, this function would be called.
     * CSP implementation should ensure that the function has only one thing is to feed the watchdog
     */
    void gmp_hal_wd_feed(void);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GMP_CSP_CPORT_H_
