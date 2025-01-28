
#ifndef _FILE_PERIPHERAL_PORT_H_
#define _FILE_PERIPHERAL_PORT_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct _tag_ring_buffer_t
    {
        // buffer
        data_gt *buffer;

        // 
        size_gt read_pos;

        size_gt write_pos;

        size_gt length;

        fast_gt full;

    } ring_buffer_t;

    //////////////////////////////////////////////////////////////////////////
    // Virtual Port

    void gmp_hal_make_half_duplex_if(half_duplex_ift *if, data_gt *data, size_gt length);

    //////////////////////////////////////////////////////////////////////////
    // Virtual Port ring buffer

    //
    void gmp_hal_buffer_write(ring_buffer_t *ring, const data_gt *data, size_gt length);

    //
    void gmp_hal_buffer_read(ring_buffer_t *ring, data_gt *data, size_gt length);

    // General Peripheral Prototype functions
    // These functions may implement by CSP.

    //////////////////////////////////////////////////////////////////////////

    // This is a synchronize function
    void gmp_hal_spi_write(spi_halt spi, const data_gt *data, size_gt length);

    // This is a asynchronize function
    void gmp_hal_spi_write_async(spi_halt spi, const data_gt *data, size_gt length);

    // This is a synchronize function
    size_gt gmp_hal_spi_read(spi_halt spi, data_gt *data, size_gt length);

    // This is a asynchronize function
    //size_gt gmp_hal_spi_read_async(spi_halt spi, data_gt *data, size_gt length);

    // Wait till transimit/receive complete
    fast_gt gmp_hal_spi_wait(spi_halt spi);

    //////////////////////////////////////////////////////////////////////////

    void gmp_hal_uart_write(uart_halt uart, const data_gt *data, size_gt length);

    void gmp_hal_uart_write_async(uart_halt uart, const data_gt *data, size_gt length);

    size_gt gmp_hal_uart_read(uart_halt uart, data_gt *data, size_gt length);

    //size_gt gmp_hal_uart_read_async(uart_halt uart, data_gt *data, size_gt length);

    fast_gt gmp_hal_uart_wait(uart_halt uart);

    //////////////////////////////////////////////////////////////////////////

    void gmp_hal_iic_write(iic_halt iic, fast16_gt dev_addr, const data_gt *data, size_gt length);

    void gmp_hal_iic_write_async(iic_halt iic, fast16_gt dev_addr, const data_gt *data, size_gt length);

    void gmp_hal_iic_write_mem(iic_halt iic, fast16_gt dev_addr, fast32_gt mem_addr, const data_gt *data,
                               size_gt length);

    void gmp_hal_iic_write_mem_async(iic_halt iic, fast16_gt dev_addr, fast32_gt mem_addr, fast_gt mem_length,
                                     const data_gt *data, size_gt length);

    size_gt gmp_hal_iic_read(iic_halt iic, fast16_gt dev_addr, data_gt *data, size_gt length);

    //size_gt gmp_hal_iic_read_async(iic_halt iic, fast16_gt dev_addr, data_gt *data, size_gt length);

    size_gt gmp_hal_iic_read_mem(iic_halt iic, fast16_gt dev_addr, fast32_gt mem_addr, fast_gt mem_length,
                                 data_gt *data, size_gt length);

    //ssize_gt gmp_hal_iic_read_mem_async(iic_halt iic, fast16_gt dev_addr, fast32_gt mem_addr, fast_gt mem_length,
    //                                   data_gt *data, size_gt length);

    //////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PERIPHERAL_PORT_H_
