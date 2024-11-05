
// User should implement RX & TX buffer prototype in this header.

#ifndef _FILE_PC_SIMULATE_BUFFER_H_
#define _FILE_PC_SIMULATE_BUFFER_H_


// Specify Simulink communication TX structure type
// Default type name is tx_buf_t
typedef struct _tag_tx_buf_t
{
    double output1;
} tx_buf_t;

// Specify Simulink communication RX structure type
// Default type name is rx_buf_t
typedef struct _tag_rx_buf_t
{
    double input1;
    double input2;
} rx_buf_t;

#endif // _FILE_PC_SIMULATE_BUFFER_H_
