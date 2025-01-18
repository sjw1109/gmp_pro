/*
 * Copyright (c) 2020 Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef BOARD_H
#define BOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "driverlib.h"
#include "device.h"

//*****************************************************************************
//
// PinMux Configurations
//
//*****************************************************************************

//
// FSIRXA -> myBoardFSI0_FSIRX Pinmux
//
//
// FSIRXA_D0 - GPIO Settings
//
#define GPIO_PIN_FSIRXA_D0 12
#define myBoardFSI0_FSIRX_FSIRXD0_GPIO 12
#define myBoardFSI0_FSIRX_FSIRXD0_PIN_CONFIG GPIO_12_FSIRXA_D0
//
// FSIRXA_D1 - GPIO Settings
//
#define GPIO_PIN_FSIRXA_D1 2
#define myBoardFSI0_FSIRX_FSIRXD1_GPIO 2
#define myBoardFSI0_FSIRX_FSIRXD1_PIN_CONFIG GPIO_2_FSIRXA_D1
//
// FSIRXA_CLK - GPIO Settings
//
#define GPIO_PIN_FSIRXA_CLK 33
#define myBoardFSI0_FSIRX_FSIRXCLK_GPIO 33
#define myBoardFSI0_FSIRX_FSIRXCLK_PIN_CONFIG GPIO_33_FSIRXA_CLK

//
// FSITXA -> myBoardFSI0_FSITX Pinmux
//
//
// FSITXA_D0 - GPIO Settings
//
#define GPIO_PIN_FSITXA_D0 6
#define myBoardFSI0_FSITX_FSITXD0_GPIO 6
#define myBoardFSI0_FSITX_FSITXD0_PIN_CONFIG GPIO_6_FSITXA_D0
//
// FSITXA_D1 - GPIO Settings
//
#define GPIO_PIN_FSITXA_D1 25
#define myBoardFSI0_FSITX_FSITXD1_GPIO 25
#define myBoardFSI0_FSITX_FSITXD1_PIN_CONFIG GPIO_25_FSITXA_D1
//
// FSITXA_CLK - GPIO Settings
//
#define GPIO_PIN_FSITXA_CLK 7
#define myBoardFSI0_FSITX_FSITXCLK_GPIO 7
#define myBoardFSI0_FSITX_FSITXCLK_PIN_CONFIG GPIO_7_FSITXA_CLK

//
// SCIB -> myEXPORT0_SCI Pinmux
//
//
// SCIB_RX - GPIO Settings
//
#define GPIO_PIN_SCIB_RX 13
#define myEXPORT0_SCI_SCIRX_GPIO 13
#define myEXPORT0_SCI_SCIRX_PIN_CONFIG GPIO_13_SCIB_RX
//
// SCIB_TX - GPIO Settings
//
#define GPIO_PIN_SCIB_TX 18
#define myEXPORT0_SCI_SCITX_GPIO 18
#define myEXPORT0_SCI_SCITX_PIN_CONFIG GPIO_18_SCIB_TX

//
// SPIA -> mySPI0 Pinmux
//
//
// SPIA_PICO - GPIO Settings
//
#define GPIO_PIN_SPIA_PICO 16
#define mySPI0_SPIPICO_GPIO 16
#define mySPI0_SPIPICO_PIN_CONFIG GPIO_16_SPIA_SIMO
//
// SPIA_POCI - GPIO Settings
//
#define GPIO_PIN_SPIA_POCI 17
#define mySPI0_SPIPOCI_GPIO 17
#define mySPI0_SPIPOCI_PIN_CONFIG GPIO_17_SPIA_SOMI
//
// SPIA_CLK - GPIO Settings
//
#define GPIO_PIN_SPIA_CLK 56
#define mySPI0_SPICLK_GPIO 56
#define mySPI0_SPICLK_PIN_CONFIG GPIO_56_SPIA_CLK
//
// SPIA_PTE - GPIO Settings
//
#define GPIO_PIN_SPIA_PTE 57
#define mySPI0_SPIPTE_GPIO 57
#define mySPI0_SPIPTE_PIN_CONFIG GPIO_57_SPIA_STE

//*****************************************************************************
//
// ADC Configurations
//
//*****************************************************************************
#define myADC0_BASE ADCA_BASE
#define myADC0_RESULT_BASE ADCARESULT_BASE
void myADC0_init();


//*****************************************************************************
//
// ASYSCTL Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// CLB Configurations
//
//*****************************************************************************
#define myCLB0_BASE CLB1_BASE
void myCLB0_init();

//*****************************************************************************
//
// FSIRX Configurations
//
//*****************************************************************************
//
// Rx Defines
// Pointer to be declared in example code before use
//
#define myBoardFSI0_FSIRX_BASE FSIRXA_BASE
#define myBoardFSI0_FSIRX_nWords 16
void myBoardFSI0_FSIRX_init();

//*****************************************************************************
//
// FSITX Configurations
//
//*****************************************************************************
//
// Tx Defines
// Pointer to be declared in example code before use
//
#define myBoardFSI0_FSITX_BASE FSITXA_BASE
#define myBoardFSI0_FSITX_PRESCALER_VAL 4
#define myBoardFSI0_FSITX_nWords 8
void myBoardFSI0_FSITX_init();

//*****************************************************************************
//
// INTERRUPT Configurations
//
//*****************************************************************************

// Interrupt Settings for INT_myADC0_1
// ISR need to be defined for the registered interrupts
#define INT_myADC0_1 INT_ADCA1
#define INT_myADC0_1_INTERRUPT_ACK_GROUP INTERRUPT_ACK_GROUP1
extern __interrupt void INT_myADC0_1_ISR(void);

//*****************************************************************************
//
// SCI Configurations
//
//*****************************************************************************
#define myEXPORT0_SCI_BASE SCIB_BASE
#define myEXPORT0_SCI_BAUDRATE 115200
#define myEXPORT0_SCI_CONFIG_WLEN SCI_CONFIG_WLEN_8
#define myEXPORT0_SCI_CONFIG_STOP SCI_CONFIG_STOP_ONE
#define myEXPORT0_SCI_CONFIG_PAR SCI_CONFIG_PAR_NONE
void myEXPORT0_SCI_init();

//*****************************************************************************
//
// SPI Configurations
//
//*****************************************************************************
#define mySPI0_BASE SPIA_BASE
#define mySPI0_BITRATE 1000000
#define mySPI0_DATAWIDTH 8
void mySPI0_init();

//*****************************************************************************
//
// SYSCTL Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************
void	Board_init();
void	ADC_init();
void	ASYSCTL_init();
void	CLB_init();
void	FSIRX_init();
void	FSITX_init();
void	INTERRUPT_init();
void	SCI_init();
void	SPI_init();
void	SYSCTL_init();
void	PinMux_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
