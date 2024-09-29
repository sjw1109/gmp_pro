
#include <core/gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// Computing Model
#include <csp/stm32/common/computing_model.stm32.h>

#ifdef HAL_CORDIC_MODULE_ENABLED

#endif // HAL_CORDIC_MODULE_ENABLED

//////////////////////////////////////////////////////////////////////////
// GPIO Model
#include <csp/stm32/common/gpio_model.stm32.h>

#ifdef HAL_GPIO_MODULE_ENABLED

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
void gmp_gpio_write(hgpio_gt *hgpio, fast_gt level)
{
    HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)level);
}

/**
 * @brief Read GPIO port, This port should be an input port.
 * Or the return value is undefined.
 * @param hgpio handle of GPIO
 * @return fast_gt return GPIO electrical level
 */
fast_gt gmp_gpio_read(hgpio_gt *hgpio)
{
    return HAL_GPIO_ReadPin(hgpio->gpio_port, hgpio->gpio_pin);
}

/**
 * @brief Set GPIO electrical level to high.
 * if GPIO mode is not output mode, the result is undefined.
 * @param hgpio handle of GPIO
 */
void gmp_gpio_set(hgpio_gt *hgpio)
{
    HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)GPIO_PIN_SET);
}

/**
 * @brief Set GPIO electrical level to low.
 * if GPIO mode is not output mode, the result is undefined.
 * @param hgpio handle of GPIO
 */
void gmp_gpio_clear(hgpio_gt *hgpio)
{
    HAL_GPIO_WritePin(hgpio->gpio_port, hgpio->gpio_pin, (GPIO_PinState)GPIO_PIN_RESET);
}

#endif // HAL_GPIO_MODULE_ENABLED

//////////////////////////////////////////////////////////////////////////
// peripheral Model
#include <csp/stm32/common/peripheral_model.stm32.h>

#ifdef HAL_UART_MODULE_ENABLED

/**
 * @brief Setup GMP UART handle.
 * This function should be called in `peripheral_mapping.c`
 * @param huart handle of GMP handle
 * @param uart_handle STM32 handle of UART
 * @param uart_tx_dma_handle STM32 DMA handle of UART TX
 * @param uart_rx_dma_handle STM32 DMA handle of UART RX
 * @param data data buffer, DMA mode only
 * @param recv_buf data buffer, DMA mode only
 */
void gmp_uart_setup(stm32_uart_t *huart, UART_HandleTypeDef *uart_handle, DMA_HandleTypeDef *uart_tx_dma_handle,
                    DMA_HandleTypeDef *uart_rx_dma_handle, duplex_ift *data_buffer, data_gt *recv_buf)
{
    huart->uart_handle = uart_handle;
    huart->uart_tx_dma_handle = uart_tx_dma_handle;
    huart->uart_rx_dma_handle = uart_rx_dma_handle;

    huart->recv_buf = recv_buf;
    huart->buffer = data_buffer;
}

/**
 * @brief send data via UART
 * @param huart handle of UART
 * @param data half_duplex data interface
 */
void gmp_uart_send(stm32_uart_t *huart, half_duplex_ift *data)
{
}

/**
 * @brief receive data via UART
 * @param huart handle of UART
 * @param data half_duplex data interface
 */
void gmp_uart_recv(stm32_uart_t *huart, half_duplex_ift *data)
{
}

/**
 * @brief bind a duplex data buffer to UART channel.
 * @param huart handle of UART
 * @param data duplex data buffer
 */
void gmp_uart_bind_duplex_dma(stm32_uart_t *huart, duplex_ift *data)
{
    huart->buffer = data;
}

/**
 * @brief start UART listen to receive routine
 * @param huart handle of UART
 */
void gmp_uart_listen(stm32_uart_t *huart)
{
    HAL_UART_Receive_DMA(huart->uart_handle, (uint8_t *)huart->recv_buf, huart->buffer->capacity);
}

/**
 * @brief Get UART listen status, return current receive bytes number.
 * @param huart
 * @return size_gt size of received bytes.
 */
size_gt gmp_uart_get_listen_status(stm32_uart_t *huart)
{
    size_gt data_length = huart->buffer->capacity - __HAL_DMA_GET_COUNTER(huart->uart_rx_dma_handle);
    return data_length;
}

//
/**
 * @brief This function check receive buffer and update rx_buf via receive buffer.
 * This function should be called in UART interrupt function
 * @param huart
 */
void gmp_uart_listen_routine(stm32_uart_t *uart)
{
	size_gt data_length;
	
    if (__HAL_UART_GET_FLAG(uart->uart_handle, UART_FLAG_IDLE) == SET)
    {
        // 清除空闲标志位
        __HAL_UART_CLEAR_IDLEFLAG(uart->uart_handle);

        // 停止DMA的传输过程
        HAL_UART_DMAStop(uart->uart_handle);

        // 计算接收到的数据长度
        data_length = uart->buffer->capacity - __HAL_DMA_GET_COUNTER(uart->uart_rx_dma_handle);

        // 判定是否真的接收到数据
        if (data_length >= 1)
        {
            // 此时确实有数据收到
            //received_flag = 1;
            // 将数据移出接收缓存，理论上应当移动到栈中
            memcpy(uart->buffer->rx_buf, uart->recv_buf, data_length);
        }

        // 重新启动DMA接收
        HAL_UART_Receive_DMA(uart->uart_handle, (uint8_t *)uart->recv_buf, uart->buffer->capacity);

        // 启动MDA接收使能
        __HAL_DMA_ENABLE(uart->uart_rx_dma_handle);

        // 再次启用UART空闲状态的中断
        __HAL_UART_ENABLE_IT(uart->uart_handle, UART_IT_IDLE);
    }
}

/**
 * @brief start UART consign to transmit routine.
 * @param huart handle of UART
 */
void gmp_uart_consign(stm32_uart_t *huart)
{
    HAL_StatusTypeDef stat;

    // judge if a buffer has bind to the object
    assert(huart != nullptr);

    if (huart->buffer == nullptr || huart->buffer->tx_buf == nullptr)
        // ignore this error
        return;

    // Call DMA to send these data
    if (HAL_DMA_GetState(huart->uart_tx_dma_handle) == HAL_DMA_STATE_READY)
    {
        stat = HAL_UART_Transmit_DMA(huart->uart_handle, huart->buffer->tx_buf, huart->buffer->length);
    }

    return;
    // if (stat == HAL_OK)
    //     return content->length;
    // else
    //     return 0;
}

/**
 * @brief Get UART consign status, return if consign routine is free.
 * @param huart
 * @return fast_gt
 */
fast_gt gmp_uart_get_consign_status(stm32_uart_t *huart)
{
    if (HAL_DMA_GetState(huart->uart_tx_dma_handle) == HAL_DMA_STATE_READY)
        return 1; // DMA has released
    else
        return 0; // DMA is still in using
}


#endif // HAL_UART_MODULE_ENABLED

//////////////////////////////////////////////////////////////////////////
// SPI Model

#if defined HAL_SPI_MODULE_ENABLED

size_gt spi_tx_direct(stm32_spi_t *hspi, data_gt *data, size_gt length)
{
    HAL_StatusTypeDef stat;
    data_gt spi_data[16];

    gmp_gpio_clear(&hspi->nchip_select);
    stat = HAL_SPI_TransmitReceive(hspi->hspi, data, spi_data, length, 10);
    gmp_gpio_set(&hspi->nchip_select);

    if (stat == HAL_OK)
        return length;
    else
        return 0;
}

#endif // HAL_SPI_MODULE_ENABLED

//////////////////////////////////////////////////////////////////////////
// System Model
#include <csp/stm32/common/sys_model.stm32.h>

#ifdef HAL_RCC_MODULE_ENABLED

time_gt gmp_port_system_tick(void)
{
    return HAL_GetTick();
}

#endif

void gmp_port_feed_dog(void)
{
#if defined SPECIFY_ENABLE_FEED_WATCHDOG
    HAL_IWDG_Refresh(&hiwdg);
#endif // SPECIFY_ENABLE_FEED_WATCHDOG
}

// This function may be called and used to initilize all the peripheral.
void gmp_csp_startup(void)
{
}

// This function would be called when fatal error occorred.
void gmp_port_system_stuck(void)
{
}

// This function would be called when all the initilization process happened.
void csp_post_process(void)
{
}

// This function is unreachable.
void gmp_exit_routine(void)
{
}

// This function may invoke when main loop occurred.
void gmp_csp_loop(void)
{
}
