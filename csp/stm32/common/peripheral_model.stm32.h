
// This file contains all the peripheral implements

#ifndef _FILE_PERIPHERAL_MODEL_STM32_H_
#define _FILE_PERIPHERAL_MODEL_STM32_H_

// ....................................................................//
// PWM controller
//
// kernel functions:
// + Enable PWM output
// + Disable PWM output
// + Set Duty
// + Set frequency
//
#ifdef HAL_TIM_MODULE_ENABLED


#endif // HAL_TIM_MODULE_ENABLED


// ....................................................................//
// UART 
//
// kernel functions:
// + Write 
// + Read
// + Set Baud Rate
//
#ifdef HAL_UART_MODULE_ENABLED

// Note:
// 发送和接收提供基础版本
// 发送提供池模式，接收提供缓冲模式




#endif // HAL_UART_MODULE_ENABLED


// ....................................................................//
// SPI
//
// kernel functions:
// + Write
// + Read
// + ReadWrite
// + Set SPI Mode
// 
#ifdef HAL_SPI_MODULE_ENABLED

#endif // HAL_SPI_MODULE_ENABLED

// ....................................................................//
// IIC
//
// kernel functions:
// + Write
// + Read
//
#ifdef HAL_I2C_MODULE_ENABLED

#endif // HAL_I2C_MODULE_ENABLED

#endif // _FILE_PERIPHERAL_MODEL_STM32_H_
