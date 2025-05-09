// This is the standard single motor controller SIL simulation interface.

#ifndef _FILE_STD_SIL_MOTOR_INTERFACE_H_
#define _FILE_STD_SIL_MOTOR_INTERFACE_H_

#pragma pack(1)
// Specify Simulink communication TX structure type
// Default type name is tx_buf_t
typedef struct _tag_mtr_sil_tx_buf
{
    // enable
    double enable;

    // Compare register
    uint32_t tabc[3];

    // monitor port
    double monitor_port[8];
} mtr_sil_tx_buf_t;
#pragma pack()

#pragma pack(1)
// Specify Simulink communication RX structure type
// Default type name is rx_buf_t
typedef struct _tag_mtr_sil_rx_buf
{
    // time
    double time;

    // current feedback
    uint32_t iabc[3];

    // voltage feedback
    uint32_t uabc[3];

    // DC bus current
    uint32_t idc;

    // DC bus voltage
    uint32_t udc;

    // encoder feedback
    uint32_t encoder;

    // encoder revolution
    int32_t revolution;

    // controller panel
    double panel[4];

} mtr_sil_rx_buf_t;
#pragma pack()




#endif // _FILE_STD_SIL_MOTOR_INTERFACE_H_
