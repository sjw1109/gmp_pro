/**
 * @file dev_util.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

#include <core/gmp_core.h>

    /**
     * @brief initialize a half duplex channel
     * @param channel half_duplex_ift handle
     * @param buf
     * @param length
     * @param capacity
     */
    void init_half_duplex_channel(half_duplex_ift *channel, data_gt *buf, size_gt length, size_gt capacity)
    {
        channel->buf = buf;
        channel->length = length;
        channel->capacity = capacity;
    }

    /**
     * @brief initialize a duplex channel
     * @param channel duplex_ift handle
     * @param tx_buf transimit buffer
     * @param rx_buf receive buffer
     * @param length length of transmit and receive buffer
     * @param capacity capacity of tx and rx buffer
     */
    void init_duplex_channel(duplex_ift *channel, data_gt *tx_buf, data_gt *rx_buf, size_gt length, size_gt capacity)
    {
        channel->tx_buf = tx_buf;
        channel->rx_buf = rx_buf;
        channel->length = length;
        channel->capacity = capacity;
    }

        /**
     * @brief initialize a half duplex with address interface
     * @param channel half duplex with address interface handle
     * @param address initialize a address
     * @param msg msg source address
     * @param length length of address
     */
    void init_half_duplex_with_addr_channel(half_duplex_with_addr_ift *channel, addr32_gt address, data_gt *msg, size_gt length)
    {
        channel->address = address;
        channel->msg = msg;
        channel->length = length;
    }

    /**
     * @brief initialize iic memeory interface object.
     * @param channel handle of IIC memory 
     * @param dev_addr IIC device Address
     * @param mem_addr memory address of IIC device
     * @param mem_length mem address length
     * @param msg message object
     * @param length length of message
     */
    void init_iic_memory_channel(iic_memory_ift *channel, addr32_gt dev_addr, addr32_gt mem_addr, fast_gt mem_length, data_gt *msg, size_gt length)
    {
        channel->dev_addr = dev_addr;
        channel->mem_addr = mem_addr;
        channel->mem_length = mem_length;
        channel->msg = msg;
        channel->length = length;
    }


    /**
     * @brief initialize a can interface object
     * @param channel can interface handle
     * @param id can address
     * @param properties can frame type
     * @param length can data length
     */
    void init_can_channel(can_ift *channel, addr32_gt id, uint32_t properties)
    {
        channel->id = id;
        channel->properties = properties;
        channel->length = 0;
        memset(channel->data, 0, 8);
    }
