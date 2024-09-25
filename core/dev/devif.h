// This file will define all the device interface (Dev-IF) 

#ifndef _FILE_DEVICE_INTERFACE_H_
#define _FILE_DEVICE_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// This file provide a set of structure type which owns a suffix ift (interface type).

// This file is independent of the other modules.


// ....................................................................//
// half Duplex communication
// such as half duplex SPI, half duplex UART (RS485)
typedef struct _tag_half_duplex_channel_t
{
    // this is a pointer of data to be transmit
    data_gt* buf;

    // length of the message
    size_gt length;

    // capacity of the message
    size_gt capacity;

}half_duplex_ift;

// ....................................................................//
// Duplex communication
// Such as SPI, UART, USART
typedef struct _tag_duplex_channel_t
{
    // transmit buffer
    data_gt* tx_buf;

    // receive buffer
    data_gt* rx_buf;

    // length of the buffer (tx and rx buffer have the same length)
    size_gt length;

    // capacity of the buffer (tx and rx buf have the same capacity)
    size_gt capacity;
    
}duplex_ift;

// ....................................................................//
// half duplex with Address
// such as IIC
typedef struct _tag_half_duplex_with_addr_channel_t
{
    // Address of device
    addr32_gt address;

    // message buffer
    data_gt* msg;

    // length of buffer
    size_gt length;

}half_duplex_with_addr_ift;


// ....................................................................//
// half duplex with dual address for IIC memeory mode
typedef struct _tag_iic_memory_channel_t
{
    // device address 
    addr32_gt dev_addr;

    // device memory address
    addr32_gt mem_addr;

    // message buffer
    data_gt* msg;

    // length of buffer
    size_gt length;

}iic_memory_ift;

// ....................................................................//
// CAN interface
typedef struct _tag_can_channel_t
{
    // can address
    // std ID is a 11-bit number, 0 ~ 0x7FF
    // ext ID is a 29-bit number, 0 ~ 0x1FFF_FFFF
    addr32_gt id; 

    // can frame type
    // RTR flag: remote flag
    // IDE flag; extension flag
    uint32_t properties;

    // can data length
    fast8_gt length;

    // can data buffer
    data_gt data[8];

}can_ift;

// ....................................................................//
// Slot interface
// This struct contains a fixed length buffer
typedef struct _tag_slot_interface_t
{
    // slot size
    addr_gt length;

    // slot buffer pointer
    data_gt* buffer;
}interface_slot_ift;

// ....................................................................//
// Strip for slot interface
// This struct contains a list of slots.
typedef struct _tag_strip_interface_t
{
    // strip size
    // how many slots the slot has.
    uint32_t size;

    // pointer list of slot interface
    interface_slot_ift** slot_it;
}interface_strip_ift;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_DEVICE_INTERFACE_H_
