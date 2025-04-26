/**
 * @file csp.config.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// This file provide a Chip Support Package configuration for SPC6L64B

#ifndef _FILE_SPC6L64B_CSP_CONFIG_H_
#define _FILE_SPC6L64B_CSP_CONFIG_H_

// using user specified default log print function
#ifndef USER_SPECIFIED_PRINT_FUNCTION
#define USER_SPECIFIED_PRINT_FUNCTION printf
#endif // USER_SPECIFIED_PRINT_FUNCTION

// use CSP specified Math Library
#define USING_CSP_MATH_LIBRARY

// disable CSP exit callback
#define SPECIFY_DISABLE_CSP_EXIT


#include "IQmathLib.h"
#include "uart1.h"

// System config

// REPAIR Necessary Constants
#define PI             3.14159265358979
#define SQRT3          1.732050807568877
#define SQRT2          1.414213562373095

// System config
#define SYS_CLK        72     // MHz,系统时钟  (不建议修改)
#define PWM_FREQUENCY  18     // PWM开关频率(kHZ)	(可修改，但不建议超过30KHZ)
#define T0_FREQUENCY   1      // 外环执行频率(kHZ)(不建议修改)
#define UART0_BAUDRATE 115200 // 串口波特率UART0
#define UART1_BAUDRATE 115200 // 串口波特率UART1

/*---------------------时间相关中间值计算，不需要修改--------------------------------*/
#define PWM_PERIOD  (SYS_CLK * 500 / PWM_FREQUENCY) //
#define TIM6_PERIOD (SYS_CLK * 1000 / T0_FREQUENCY) //
#define TPWM        (0.001 / PWM_FREQUENCY)         // PWM周期值 (s)
#define TSP         (TPWM)                          // PWM周期值 (s)
#define TS0         (0.001 / T0_FREQUENCY)          // T0周期值  (s)

/*-----------------------标幺值,单位均为pu ，不需要修改------------------------------*/

#define UART0_IBRD (unsigned long)(SYS_CLK * 1000000 / (16 * UART0_BAUDRATE))
#define UART0_FBRD                                                                                                     \
    (unsigned long)((SYS_CLK * 1000000 % UART0_BAUDRATE >= (UART0_BAUDRATE >> 1))                                      \
                        ? (SYS_CLK * 1000000 / UART0_BAUDRATE - UART0_IBRD * 16 + 1)                                   \
                        : (SYS_CLK * 1000000 / UART0_BAUDRATE - UART0_IBRD * 16))

#define UART1_IBRD (unsigned long)(SYS_CLK * 1000000 / (16 * UART1_BAUDRATE))
#define UART1_FBRD                                                                                                     \
    (unsigned long)((SYS_CLK * 1000000 % UART1_BAUDRATE >= (UART1_BAUDRATE >> 1))                                      \
                        ? (SYS_CLK * 1000000 / UART1_BAUDRATE - UART1_IBRD * 16 + 1)                                   \
                        : (SYS_CLK * 1000000 / UART1_BAUDRATE - UART1_IBRD * 16))   
    


#endif // _FILE_SPC6L64B_CSP_CONFIG_H_

