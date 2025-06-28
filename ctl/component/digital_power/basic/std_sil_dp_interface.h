
// This file implement a universal GMP SIL simulation standard interface

#include <ctl/component/interface/interface_base.h>

#ifndef _FILE_GMP_SIL_STD_INTERFACE_H_
#define _FILE_GMP_SIL_STD_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#pragma pack(1)
// Specify Simulink communication TX structure type
// Default type name is tx_buf_t
typedef struct _tag_dp_sil_tx_buf
{
    // enable
    double enable;

    // Compare register
    uint32_t pwm_cmp[8];

    // DAC output
    uint32_t dac[8];

    // monitor port
    double monitor[16];

} dp_sil_tx_buf_t;
#pragma pack()

#pragma pack(1)
// Specify Simulink communication RX structure type
// Default type name is rx_buf_t
typedef struct _tag_dp_sil_rx_buf
{
    // time
    double time;

    // ADC feedback
    uint32_t adc_result[24];

    // panel input
    double panel[16];

    // DC bus current
    int32_t digital[8];

} dp_sil_rx_buf_t;
#pragma pack()

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GMP_SIL_STD_INTERFACE_H_s

