
// This file provide a set of function that CSP must defined.

#include <core/gmp_core.h>

// System Tick
time_gt DSPC2000_SystemTick = 0;

// User should invoke this function to get time (system tick).
time_gt gmp_base_get_system_tick(void)
{
    return DSPC2000_SystemTick;
}

// This function may be called and used to initialize all the peripheral.
void gmp_csp_startup(void)
{
    //
    // Initialize device clock and peripherals
    //
    InitSysCtrl();

    //
    // Initialize GPIO
    //
    InitGpio();

    // TODO:
    // USER GPIO Allocation
    //

    //
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR)
    //
    InitPieVectTable();

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

#ifdef FLASH
    // Copy time critical code and Flash setup code to RAM
    // The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
    // symbols are created by the linker. Refer to the linker files.
    MemCopy((Uint16 *)&RamfuncsLoadStart, (Uint16 *)&RamfuncsLoadEnd, (Uint16 *)&RamfuncsRunStart);

    // Call Flash Initialization to setup flash waitstates
    // This function must reside in RAM
    InitFlash(); // Call the flash wrapper init function
#endif           //(FLASH)

    SetupSystemTickTimer();
}
