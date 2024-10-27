/**
 * @file example.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

// This file provide a set of function that CSP must defined.

#include <gmp_core.h>

// User should invoke this function to get time (system tick).
time_gt gmp_port_system_tick(void)
{
    return 0;
}

// This function may be called and used to initialize all the peripheral. 
void gmp_csp_startup(void)
{
    WDogDisable(); // Disable the watchdog initially

    DINT;         // Global Disable all Interrupts
    IER = 0x0000; // Disable CPU interrupts
    IFR = 0x0000; // Clear all CPU interrupt flags

    // Switch to Internal Oscillator 1 and turn off all other clock
    // sources to minimize power consumption
    EnableInternalClock();

    // PLL set core speed 60 MHz
    PLLset(0x0C);

    // Initialize interrupt controller and Vector Table to defaults for now.
    // Application ISR mapping done later.
    PieCntlInit();
    PieVectTableInit();

    // LOW SPEED CLOCKS pre-scale register settings
    EALLOW;
    SysCtrlRegs.LOSPCP.all = 0x0002; // Sysclk / 4 (15 MHz)
    SysCtrlRegs.XCLK.bit.XCLKOUTDIV = 2;
    EDIS;

    // ADC CALIBRATION
    CalibrateADC();

    // Enable necessary Peripheral CLK
    ConfigPeripheralClock();

    // Assign GPIO MUX
    ConfigGPIOMUX();
}

// This function would be called when fatal error occurred.
void gmp_port_system_stuck(void)
{}

// This function would be called when all the initialization process happened.
void gmp_csp_post_process(void)
{
    EALLOW;

    // Enable global Interrupts and higher priority real-time debug events:
        EINT;   // Enable Global interrupt INTM
        ERTM;   // Enable Global realtime interrupt DBGM

    EDIS;
}
    
// This function is unreachable.
void gmp_exit_routine(void)
{}

// This function may invoke when main loop occurred.
void gmp_csp_loop(void)
{}


//////////////////////////////////////////////////////////////////////////
// TMS320F28035 utilities function


// Functions that will be run from RAM need to be assigned to
// a different section.  This section will then be mapped to a load and
// run address using the linker cmd file.
#pragma CODE_SECTION(InitFlash, "ramfuncs");
#define Device_cal (void (*)(void))0x3D7C80

void CalibrateADC(void)
{
    EALLOW;

    // ADC CALIBRATION
    //---------------------------------------------------
    // The Device_cal function, which copies the ADC & oscillator calibration values
    // from TI reserved OTP into the appropriate trim registers, occurs automatically
    // in the Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC and oscillators to function according
    // to specification.

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // Enable ADC peripheral clock
    (*Device_cal)();                      // Auto-calibrate from TI OTP
    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 0; // Return ADC clock to original state

    EDIS;
}

void WDogDisable(void)
{
    EALLOW;
    SysCtrlRegs.WDCR = 0x0068;
    EDIS;
}

// This function initializes the PLLCR register.
// void InitPll(Uint16 val, Uint16 clkindiv)

void EnableInternalClock(void)
{

    // Switch to Internal Oscillator 1 and turn off all other clock
    // sources to minimize power consumption
    EALLOW;
    SysCtrlRegs.CLKCTL.bit.INTOSC1OFF = 0;
    SysCtrlRegs.CLKCTL.bit.OSCCLKSRCSEL = 0; // Clk Src = INTOSC1
    SysCtrlRegs.CLKCTL.bit.XCLKINOFF = 1;    // Turn off XCLKIN
    SysCtrlRegs.CLKCTL.bit.XTALOSCOFF = 1;   // Turn off XTALOSC
    SysCtrlRegs.CLKCTL.bit.INTOSC2OFF = 1;   // Turn off INTOSC2
    EDIS;


}


// SYSTEM CLOCK speed based on internal oscillator = 10 MHz
// 0xC =  60	MHz		(12)
// 0xB =  55	MHz		(11)
// 0xA =  50	MHz		(10)
// 0x9 =  45	MHz		(9)
// 0x8 =  40	MHz		(8)
// 0x7 =  35	MHz		(7)
// 0x6 =  30	MHz		(6)
// 0x5 =  25	MHz		(5)
// 0x4 =  20	MHz		(4)
// 0x3 =  15	MHz		(3)
// 0x2 =  10	MHz		(2)

void PLLset(Uint16 val)
{
    volatile Uint16 iVol;

    // Make sure the PLL is not running in limp mode
    if (SysCtrlRegs.PLLSTS.bit.MCLKSTS != 0)
    {
        EALLOW;
        // OSCCLKSRC1 failure detected. PLL running in limp mode.
        // Re-enable missing clock logic.
        SysCtrlRegs.PLLSTS.bit.MCLKCLR = 1;
        EDIS;
        // Replace this line with a call to an appropriate
        // SystemShutdown(); function.
        asm("        ESTOP0"); // Uncomment for debugging purposes
    }

    // DIVSEL MUST be 0 before PLLCR can be changed from
    // 0x0000. It is set to 0 by an external reset XRSn
    // This puts us in 1/4
    if (SysCtrlRegs.PLLSTS.bit.DIVSEL != 0)
    {
        EALLOW;
        SysCtrlRegs.PLLSTS.bit.DIVSEL = 0;
        EDIS;
    }

    // Change the PLLCR
    if (SysCtrlRegs.PLLCR.bit.DIV != val)
    {

        EALLOW;
        // Before setting PLLCR turn off missing clock detect logic
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 1;
        SysCtrlRegs.PLLCR.bit.DIV = val;
        EDIS;

        // Optional: Wait for PLL to lock.
        // During this time the CPU will switch to OSCCLK/2 until
        // the PLL is stable.  Once the PLL is stable the CPU will
        // switch to the new PLL value.
        //
        // This time-to-lock is monitored by a PLL lock counter.
        //
        // Code is not required to sit and wait for the PLL to lock.
        // However, if the code does anything that is timing critical,
        // and requires the correct clock be locked, then it is best to
        // wait until this switching has completed.

        // Wait for the PLL lock bit to be set.
        // The watchdog should be disabled before this loop, or fed within
        // the loop via ServiceDog().

        // Uncomment to disable the watchdog
        WDogDisable();

        while (SysCtrlRegs.PLLSTS.bit.PLLLOCKS != 1)
        {
        }

        EALLOW;
        SysCtrlRegs.PLLSTS.bit.MCLKOFF = 0;
        EDIS;
    }

    // divide down SysClk by 2 to increase stability
    EALLOW;
    SysCtrlRegs.PLLSTS.bit.DIVSEL = 2;
    EDIS;
}

// This function initializes the PIE control registers to a known state.
//
void PieCntlInit(void)
{
    // Disable Interrupts at the CPU level:
    DINT;

    // Disable the PIE
    PieCtrlRegs.PIECTRL.bit.ENPIE = 0;

    // Clear all PIEIER registers:
    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;

    // Clear all PIEIFR registers:
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;
}

void PieVectTableInit(void)
{
    int16 i;
    volatile PINT *Dest = &PieVectTable.TINT1;

    EALLOW;
    for (i = 0; i < 115; i++)
        *Dest++ = &ISR_ILLEGAL;
    EDIS;

    // Enable the PIE Vector Table
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
}

// Illegal operation TRAP
interrupt void ISR_ILLEGAL(void) 
{
    // Insert ISR Code here

    // invoke GMP illegal function here


    // Next two lines for debug only to halt the processor here
    // Remove after inserting ISR Code
    asm("          ESTOP0");
    for (;;)
        ;
}

// This function initializes the Flash Control registers

// CAUTION
// This function MUST be executed out of RAM. Executing it
// out of OTP/Flash will yield unpredictable results

void InitFlash(void)
{
    EALLOW;
    // Enable Flash Pipeline mode to improve performance
    // of code executed from Flash.
    FlashRegs.FOPT.bit.ENPIPE = 1;

    //                CAUTION
    // Minimum waitstates required for the flash operating
    // at a given CPU rate must be characterized by TI.
    // Refer to the datasheet for the latest information.

    // Set the Paged Waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = 3;

    // Set the Random Waitstate for the Flash
    FlashRegs.FBANKWAIT.bit.RANDWAIT = 3;

    // Set the Waitstate for the OTP
    FlashRegs.FOTPWAIT.bit.OTPWAIT = 5;

    //                CAUTION
    // ONLY THE DEFAULT VALUE FOR THESE 2 REGISTERS SHOULD BE USED
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = 0x01FF;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = 0x01FF;
    EDIS;

    // Force a pipeline flush to ensure that the write to
    // the last register configured occurs before returning.

    asm(" RPT #7 || NOP");
}

// This function will copy the specified memory contents from
// one location to another.
//
//	Uint16 *SourceAddr        Pointer to the first word to be moved
//                          SourceAddr < SourceEndAddr
//	Uint16* SourceEndAddr     Pointer to the last word to be moved
//	Uint16* DestAddr          Pointer to the first destination word
//
// No checks are made for invalid memory locations or that the
// end address is > then the first start address.

void MemCopy(Uint16 *SourceAddr, Uint16 *SourceEndAddr, Uint16 *DestAddr)
{
    while (SourceAddr < SourceEndAddr)
    {
        *DestAddr++ = *SourceAddr++;
    }
    return;
}

