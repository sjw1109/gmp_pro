

#ifndef _FILE_SIMULINK_BUFFER_H_
#define _FILE_SIMULINK_BUFFER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// User should implement RX & TX buffer prototype in this header.
#include <pc_simulate_buffer.h>

    // Receive structure
    // + 1 current simulate time: double
    // + other controller parameters
//#pragma pack(1)
    typedef struct _tag_pc_simulink_rx_buffer_t
    {
        double time;

        GMP_PC_SIMULINK_RX_STRUCT rx_buf;

    } gmp_pc_simulink_rx_buffer_t;
//#pragma pack()

    extern gmp_pc_simulink_rx_buffer_t simulink_rx_buffer;

    GMP_STATIC_INLINE
    GMP_PC_SIMULINK_RX_STRUCT *gmp_csp_sl_get_rx_buffer()
    {
        return &simulink_rx_buffer.rx_buf;
    }

    // Send structure
    // + 1 Enable flag: double, 0 is disable, 1 is enable.
    // + other controller parameters
//#pragma pack(1)
    typedef struct _tag_pc_simulink_tx_buffer_t
    {
        uint32_t enable;

        GMP_PC_SIMULINK_TX_STRUCT tx_buf;
    } gmp_pc_simulink_tx_buffer_t;
//#pragma pack()

    extern gmp_pc_simulink_tx_buffer_t simulink_tx_buffer;

    GMP_STATIC_INLINE
    GMP_PC_SIMULINK_TX_STRUCT *gmp_csp_sl_get_tx_buffer()
    {
        return &simulink_tx_buffer.tx_buf;
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_SIMULINK_BUFFER_H_
