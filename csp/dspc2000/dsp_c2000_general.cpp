
// invoke GMP header
#include <core/gmp_core.hpp>

//////////////////////////////////////////////////////////////////////////
// Timer function 

// This function get the system tick.
GMP_PORT_TIME_T gmp_port_system_tick(
    void
)
{
    return 0;
}

// This function would invoke when system meets some fatal errors.
void gmp_port_system_stuck(
    void
)
{
    // invoke the Error handler
    for(;;){}

}


// The following function is a feed a dog routine
void gmp_port_feed_dog()
{
    // This function is defined in TI C2000ware.
    // you may find the function definition in 
    // <ti/c2000/C2000Ware_<version>/device_support/<device>/common/source/<device>_SysCtrl.c>
    // prototype is void ServiceDog(void);
    ServiceDog();
}

// This function should complete all the initialization process
// This function is from TI C2000 Source library.
// Only for the following function. Here is the license.
//###########################################################################
// $TI Release: F2806x Support Library v2.06.00.00 $
// $Release Date: Fri Feb 12 19:15:11 IST 2021 $
// $Copyright:
// Copyright (C) 2009-2021 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

void gmp_setup_peripheral()
{
    //
    // Step 1. Initialize System Control registers, PLL, WatchDog, Clocks to 
    // default state: This function is found in the F2806x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Select GPIO for the device or for the specific application:
    // This function is found in the F2806x_Gpio.c file.
    //
    InitSciGpio();

    //
    // Step 3. Initialize PIE vector table:
    // The PIE vector table is initialized with pointers to shell Interrupt
    // Service Routines (ISR).  The shell routines are found in 
    // F2806x_DefaultIsr.c. Insert user specific ISR code in the appropriate 
    // shell ISR routine in the DSP28_DefaultIsr.c file.
    //

    //
    // Disable and clear all CPU interrupts
    //
    DINT;
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize Pie Control Registers To Default State:
    // This function is found in the F2806x_PieCtrl.c file.
    //
    //InitPieCtrl();  PIE is not used for this example

    //
    // Initialize the PIE Vector Table To a Known State:
    // This function is found in F2806x_PieVect.c.
    // This function populates the PIE vector table with pointers
    // to the shell ISR functions found in F2806x_DefaultIsr.c.
    //
    InitPieVectTable();

    //
    // Enable CPU and PIE interrupts
    // This example function is found in the F2806x_PieCtrl.c file.
    //
    EnableInterrupts();

    //
    // Step 4. Initialize all the Device Peripherals to a known state
    // others will be implement in other classes.
    //
}

// GPIO Control handle
// 
// This is the only entry point of GPIO Data Register pointer.
gpio_dat_t* gpio_dat_handle = (gpio_dat_t*)&GpioDataRegs;

//////////////////////////////////////////////////////////////////////////
// IO peripheral function implement


