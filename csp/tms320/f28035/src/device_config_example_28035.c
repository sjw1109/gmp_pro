#include <core/gmp_core.h>

// Change this function to config peripherals
void ConfigPeripheralClock(void)
{
    EALLOW;

    // PERIPHERAL CLOCK ENABLES
    //---------------------------------------------------
    // If you are not using a peripheral you may want to switch
    // the clock off to save power, i.e. set to =0
    //
    // Note: not all peripherals are available on all 280x derivates.
    // Refer to the datasheet for your particular device.

    SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1; // ADC
    //------------------------------------------------

    SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 1; // COMP1
    SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1; // COMP2
    SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK = 1; // COMP3
    //------------------------------------------------

    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1; // eCAP1
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0; // eCAN-A
    //------------------------------------------------

    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1; // eQEP1
    //------------------------------------------------

    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1; // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1; // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1; // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1; // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1; // ePWM6
    SysCtrlRegs.PCLKCR1.bit.EPWM7ENCLK = 1; // ePWM7
    SysCtrlRegs.PCLKCR0.bit.HRPWMENCLK = 0; // HRPWM
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0; // I2C
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.LINAENCLK = 0; // LIN-A
    //------------------------------------------------

    SysCtrlRegs.PCLKCR3.bit.CLA1ENCLK = 0; // CLA1
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1; // SCI-A
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 0; // SPI-A
    SysCtrlRegs.PCLKCR0.bit.SPIBENCLK = 0; // SPI-B
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Enable TBCLK
    //------------------------------------------------

    EDIS;
}

// Change here to config GPIO usage
void ConfigGPIOMUX(void)
{
    EALLOW;

    //--------------------------------------------------------------------------------------
    // GPIO (GENERAL PURPOSE I/O) CONFIG
    //--------------------------------------------------------------------------------------
    //-----------------------
    // QUICK NOTES on USAGE:
    //-----------------------
    // If GpioCtrlRegs.GP?MUX?bit.GPIO?= 1, 2 or 3 (i.e. Non GPIO func), then leave
    //	rest of lines commented
    // If GpioCtrlRegs.GP?MUX?bit.GPIO?= 0 (i.e. GPIO func), then:
    //	1) uncomment GpioCtrlRegs.GP?DIR.bit.GPIO? = ? and choose pin to be IN or OUT
    //	2) If IN, can leave next to lines commented
    //	3) If OUT, uncomment line with ..GPACLEAR.. to force pin LOW or
    //			   uncomment line with ..GPASET.. to force pin HIGH or
    //--------------------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-00 - PIN FUNCTION = PWM1A
    // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
    //	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO0 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-01 - PIN FUNCTION = PWM1B
    // 0=GPIO,  1=EPWM1B,  2=Resv,  3=COMP1OUT
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
    //	GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO1 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-02 - PIN FUNCTION = PWM2A
    // 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO2 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO2 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-03 - PIN FUNCTION = PWM2B
    // 0=GPIO,  1=EPWM2B,  2=SPISOMI-A,  3=COMP2OUT
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO3 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-04 - PIN FUNCTION = PWM3A
    // 0=GPIO,  1=EPWM3A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO4 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-05 - PIN FUNCTION = PWM3B
    // 0=GPIO,  1=EPWM3B,  2=SPISIMO-A,  3=ECAP1
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO5 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO5 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-06 - PIN FUNCTION = PWM4A
    // 0=GPIO,  1=EPWM4A,  2=SYNCI,  3=SYNCO
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO6 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-07 - PIN FUNCTION = PWM4B
    // 0=GPIO,  1=EPWM4B,  2=SCIRX-A,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO7 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO7 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-08 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=EPWM5A,  2=Resv,  3=ADCSOC-A
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO8 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO8 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-09 - PIN FUNCTION = Clear Fault
    // 0=GPIO,  1=EPWM5B,  2=LINTX-A,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
    // uncomment if --> Set High initially
    GpioDataRegs.GPASET.bit.GPIO9 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-10 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=EPWM6A,  2=Resv,  3=ADCSOC-B
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO10 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO10 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-11 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=EPWM6B,  2=LINRX-A,  3=Resv
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO11 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO11 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-12 - PIN FUNCTION = TZ1
    // 0=GPIO,  1=TZ1,  2=SCITX-A,  3=SPISIMO-B
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO12 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-13 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=TZ2,  2=Resv,  3=SPISOMI-B
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO13 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-14 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=TZ3,  2=LINTX-A,  3=SPICLK-B
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO14 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-15 - PIN FUNCTION = OCP
    // 0=GPIO,  1=TZ1,  2=LINRX-A,  3=SPISTE-B
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO15 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-16 - PIN FUNCTION = SPISIMO-A
    // 0=GPIO,  1=SPISIMO-A,  2=Resv,  3=TZ2
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO16 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO16 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-17 - PIN FUNCTION = SPISOMI-A
    // 0=GPIO,  1=SPISOMI-A,  2=Resv,  3=TZ3
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO17 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO17 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-18 - PIN FUNCTION = SPICLK-A
    // 0=GPIO,  1=SPICLK-A,  2=LINTX-A,  3=XCLKOUT
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO18 = 1;
    //	GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO18 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-19 - PIN FUNCTION = SPISTE-A
    // 0=GPIO,  1=SPISTE-A,  2=LINRX-A,  3=ECAP1
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO19 = 0;
    //	GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;	// uncomment if --> Set Low initially
    //	GpioDataRegs.GPASET.bit.GPIO19 = 1;		// uncomment if --> Set High initially
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-20 - PIN FUNCTION = EQEPA-1
    // 0=GPIO,  1=EQEPA-1,  2=Resv,  3=COMP1OUT
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO20 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-21 - PIN FUNCTION = EQEPB-1
    // 0=GPIO,  1=EQEPB-1,  2=Resv,  3=COMP2OUT
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO21 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-22 - PIN FUNCTION = GPIO
    // 0=GPIO,  1=EQEPS-1,  2=Resv,  3=LINTX-A
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;
    // uncomment if --> Set Low initially
    // GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPASET.bit.GPIO22 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-23 - PIN FUNCTION = EQEPI-1
    // 0=GPIO,  1=EQEPI-1,  2=Resv,  3=LINRX-A
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO23 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO23 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-24 - PIN FUNCTION = ECAP1
    // 0=GPIO,  1=ECAP1,  2=Resv,  3=SPISIMO-B
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO24 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO24 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-25 - PIN FUNCTION = GPIO
    // 0=GPIO,  1=Resv,  2=Resv,  3=SPISOMI-B
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO25 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO25 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-26 - PIN FUNCTION = GPIO
    // 0=GPIO,  1=Resv,  2=Resv,  3=SPICLK-B
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO26 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO26 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-27 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=Resv,  2=Resv,  3=SPISTE-B
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO27 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-28 - PIN FUNCTION = SCI-RX
    // 0=GPIO,  1=SCIRX-A,  2=I2CSDA-A,  3=TZ2
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO28 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-29 - PIN FUNCTION = SCI-TX
    // 0=GPIO,  1=SCITXD-A,  2=I2CSCL-A,  3=TZ3
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 1;
    // 1=OUTput,  0=INput
    // GpioCtrlRegs.GPADIR.bit.GPIO29 = 0;
    // uncomment if --> Set Low initially
    // GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPASET.bit.GPIO29 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-30 - PIN FUNCTION = CANRX-A
    // 0=GPIO,  1=CANRX-A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO30 = 0;
    // uncomment if --> Set Low initially
    // GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPASET.bit.GPIO30 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-31 - PIN FUNCTION = CANTX-A
    // 0=GPIO,  1=CANTX-A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPADIR.bit.GPIO31 = 1;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPASET.bit.GPIO31 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-32 - PIN FUNCTION = I2CSDA-A
    // 0=GPIO,  1=I2CSDA-A,  2=SYNCI,  3=ADCSOCA
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPBSET.bit.GPIO32 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-33 - PIN FUNCTION = I2CSCL-A
    // 0=GPIO,  1=I2CSCL-A,  2=SYNCO,  3=ADCSOCB
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPBSET.bit.GPIO33 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-34 - PIN FUNCTION = LED3 on controlCARD
    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    // uncomment if --> Set Low initially
    GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPBSET.bit.GPIO34 = 1;
    //
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    // GPIO 35-38 are defaulted to JTAG usage, and are not shown here to enforce JTAG debug
    // usage.
    //--------------------------------------------------------------------------------------
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-39 - PIN FUNCTION = GPIO
    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPBSET.bit.GPIO39 = 1;
    //--------------------------------------------------------------------------------------
    //  GPIO-40 - PIN FUNCTION = EPWM7A
    // 0=GPIO,  1=EPWM7A,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBMUX1.bit.GPIO40 = 1;
    // 1=OUTput,  0=INput
    //	GpioCtrlRegs.GPBDIR.bit.GPIO40 = 0;
    // uncomment if --> Set Low initially
    //	GpioDataRegs.GPBCLEAR.bit.GPIO40 = 1;
    // uncomment if --> Set High initially
    //	GpioDataRegs.GPBSET.bit.GPIO40 = 1;
    //--------------------------------------------------------------------------------------
    //  GPIO-41 - PIN FUNCTION = EPWM7B
    // 0=GPIO,  1=EPWM7B,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBMUX1.bit.GPIO41 = 1;
    // 1=OUTput,  0=INput
    // GpioCtrlRegs.GPBDIR.bit.GPIO41 = 1;
    // uncomment if --> Set Low initially
    // GpioDataRegs.GPBCLEAR.bit.GPIO41 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPBSET.bit.GPIO41 = 1;
    //--------------------------------------------------------------------------------------
    //  GPIO-42 - PIN FUNCTION = LED2
    // 0=GPIO,  1=Resv,  2=Resv,  3=COMP1OUT
    GpioCtrlRegs.GPBMUX1.bit.GPIO42 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPBDIR.bit.GPIO42 = 1;
    // uncomment if --> Set Low initially
    GpioDataRegs.GPBCLEAR.bit.GPIO42 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPBSET.bit.GPIO42 = 1;
    //
    //--------------------------------------------------------------------------------------
    //  GPIO-43 - PIN FUNCTION = --Spare--
    // 0=GPIO,  1=Resv,  2=Resv,  3=COMP2OUT
    GpioCtrlRegs.GPBMUX1.bit.GPIO43 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPBDIR.bit.GPIO43 = 0;
    // uncomment if --> Set Low initially
    // GpioDataRegs.GPBCLEAR.bit.GPIO43 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPBSET.bit.GPIO43 = 1;
    //--------------------------------------------------------------------------------------
    //  GPIO-44 - PIN FUNCTION = LED1
    // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBMUX1.bit.GPIO44 = 0;
    // 1=OUTput,  0=INput
    GpioCtrlRegs.GPBDIR.bit.GPIO44 = 1;
    // uncomment if --> Set Low initially
    GpioDataRegs.GPBCLEAR.bit.GPIO44 = 1;
    // uncomment if --> Set High initially
    // GpioDataRegs.GPBSET.bit.GPIO44 = 1;
    //--------------------------------------------------------------------------------------

    EDIS;
}
