

#include <core/gmp_core.h>

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

    SysCtrlRegs.PCLKCR0.bit.I2CAENCLK = 0; // I2C
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.SPIAENCLK = 1; // SPI-A
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1; // SCI-A
    SysCtrlRegs.PCLKCR0.bit.SCIBENCLK = 0; // SCI-B
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.ECANAENCLK = 0; // eCAN-A
    SysCtrlRegs.PCLKCR0.bit.ECANBENCLK = 0; // eCAN-B
    //------------------------------------------------

    SysCtrlRegs.PCLKCR1.bit.ECAP1ENCLK = 1; // eCAP1
    SysCtrlRegs.PCLKCR1.bit.ECAP2ENCLK = 1; // eCAP2
    SysCtrlRegs.PCLKCR1.bit.ECAP3ENCLK = 1; // eCAP3
    SysCtrlRegs.PCLKCR1.bit.ECAP4ENCLK = 1; // eCAP4
    //------------------------------------------------

    SysCtrlRegs.PCLKCR1.bit.EPWM1ENCLK = 1; // ePWM1
    SysCtrlRegs.PCLKCR1.bit.EPWM2ENCLK = 1; // ePWM2
    SysCtrlRegs.PCLKCR1.bit.EPWM3ENCLK = 1; // ePWM3
    SysCtrlRegs.PCLKCR1.bit.EPWM4ENCLK = 1; // ePWM4
    SysCtrlRegs.PCLKCR1.bit.EPWM5ENCLK = 1; // ePWM5
    SysCtrlRegs.PCLKCR1.bit.EPWM6ENCLK = 1; // ePWM6
    //------------------------------------------------

    SysCtrlRegs.PCLKCR1.bit.EQEP1ENCLK = 1; // eQEP1
    SysCtrlRegs.PCLKCR1.bit.EQEP2ENCLK = 1; // eQEP2
    //------------------------------------------------

    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Enable TBCLK


    EDIS;
}

void ConfigGPIOMUX(void)
{
    EALLOW;

    // GPIO (GENERAL PURPOSE I/O) CONFIG
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    // QUICK NOTES on GPIO CONFIG USAGE:
    //----------------------------------
    // If GpioCtrlRegs.GP?MUX?bit.GPIO?= 1, 2 or 3 (i.e. Non GPIO func), then leave
    //	rest of lines commented
    // If GpioCtrlRegs.GP?MUX?bit.GPIO?= 0 (i.e. GPIO func), then:
    //	  1) uncomment GpioCtrlRegs.GP?DIR.bit.GPIO? = ? and choose pin to be IN or OUT direc.
    //	  2) If IN, can leave next two lines commented
    //	  3) If OUT, uncomment line with ..GPACLEAR.. to force pin LOW or
    //			     uncomment line with ..GPASET.. to force pin HIGH
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //  GPIO-00 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 =
        1; // 0=GPIO,  1=EPWM1A,  2=Resv,  3=Resv
           //	GpioCtrlRegs.GPADIR.bit.GPIO0 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO0 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-01 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 =
        1; // 0=GPIO,  1=EPWM1B,  2=ECAP6,  3=MFSR-B
           //	GpioCtrlRegs.GPADIR.bit.GPIO1 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO1 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-02 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 =
        1; // 0=GPIO,  1=EPWM2A,  2=Resv,  3=Resv
           //	GpioCtrlRegs.GPADIR.bit.GPIO2 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO2 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-03 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 =
        1; // 0=GPIO,  1=EPWM2B,  2=ECAP5,  3=MCLKR-B
           //	GpioCtrlRegs.GPADIR.bit.GPIO3 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO3 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-04 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 =
        1; // 0=GPIO,  1=EPWM3A,  2=Resv,  3=Resv
           //	GpioCtrlRegs.GPADIR.bit.GPIO4 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO4 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-05 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 =
        1; // 0=GPIO,  1=EPWM3B,  2=MFSR-A,  3=ECAP1
           //	GpioCtrlRegs.GPADIR.bit.GPIO5 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO5 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-06 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 =
        1; // 0=GPIO,  1=EPWM4A,  2=SYNCI,  3=SYNCO
           //	GpioCtrlRegs.GPADIR.bit.GPIO6 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO6 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-07 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 =
        1; // 0=GPIO,  1=EPWM4B,  2=MCLKR-A,  3=ECAP2
           //	GpioCtrlRegs.GPADIR.bit.GPIO7 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO7 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-08 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1; // 0=GPIO,  1=EPWM5A,  2=CANTX-B,  3=ADCSOC-A
    GpioCtrlRegs.GPADIR.bit.GPIO8 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO8 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-09 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1; // 0=GPIO,  1=EPWM5B,  2=SCITX-B,  3=ECAP3
    GpioCtrlRegs.GPADIR.bit.GPIO9 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO9 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-10 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1; // 0=GPIO,  1=EPWM6A,  2=CANRX-B,  3=ADCSOC-B
    GpioCtrlRegs.GPADIR.bit.GPIO10 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO10 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-11 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1; // 0=GPIO,  1=EPWM6B,  2=SCIRX-B,  3=ECAP4
    GpioCtrlRegs.GPADIR.bit.GPIO11 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO11 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-12 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 =
        1; // 0=GPIO,  1=TZ1,  2=CANTX-B,  3=MDX-B
           //  GpioCtrlRegs.GPADIR.bit.GPIO12 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO12 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-13 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0; // 0=GPIO,  1=TZ2,  2=CANRX-B,  3=MDR-B
    GpioCtrlRegs.GPADIR.bit.GPIO13 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO13 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO13 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-14 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0; // 0=GPIO,  1=TZ3,  2=SCITX-B,  3=MCLKX-B
    GpioCtrlRegs.GPADIR.bit.GPIO14 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO14 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO14 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-15 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0; // 0=GPIO,  1=TZ4,  2=SCIRX-B,  3=MFSX-B
    GpioCtrlRegs.GPADIR.bit.GPIO15 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO15 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO15 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //  GPIO-16 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0; // 0=GPIO,  1=SPISIMO-A,  2=CANTX-B,  3=TZ5
    GpioCtrlRegs.GPADIR.bit.GPIO16 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO16 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO16 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-17 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0; // 0=GPIO,  1=SPISOMI-A,  2=CANRX-B,  3=TZ6
    GpioCtrlRegs.GPADIR.bit.GPIO17 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO17 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-18 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 0; // 0=GPIO,  1=SPICLK-A,  2=SCITX-B,  3=CANRX-A
    GpioCtrlRegs.GPADIR.bit.GPIO18 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO18 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO18 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-19 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 0; // 0=GPIO,  1=SPISTE-A,  2=SCIRX-B,  3=CANTX-A
    GpioCtrlRegs.GPADIR.bit.GPIO19 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO19 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-20 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1; // 0=GPIO,  1=EQEPA-1,  2=MDX-A,  3=CANTX-B
    GpioCtrlRegs.GPADIR.bit.GPIO20 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO20 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-21 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1; // 0=GPIO,  1=EQEPB-1,  2=MDR-A,  3=CANRX-B
    GpioCtrlRegs.GPADIR.bit.GPIO21 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO21 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-22 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0; // 0=GPIO,  1=EQEPS-1,  2=MCLKX-A,  3=SCITX-B
    GpioCtrlRegs.GPADIR.bit.GPIO22 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO22 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-23 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1; // 0=GPIO,  1=EQEPI-1,  2=MFSX-A,  3=SCIRX-B
    GpioCtrlRegs.GPADIR.bit.GPIO23 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO23 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO23 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-24 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 0; // 0=GPIO,  1=ECAP1,  2=EQEPA-2,  3=MDX-B
    GpioCtrlRegs.GPADIR.bit.GPIO24 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO24 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO24 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-25 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 0; // 0=GPIO,  1=ECAP2,  2=EQEPB-2,  3=MDR-B
    GpioCtrlRegs.GPADIR.bit.GPIO25 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO25 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO25 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-26 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 0; // 0=GPIO,  1=ECAP3,  2=EQEPI-2,  3=MCLKX-B
    GpioCtrlRegs.GPADIR.bit.GPIO26 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO26 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO26 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-27 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0; // 0=GPIO,  1=ECAP4,  2=EQEPS-2,  3=MFSX-B
    GpioCtrlRegs.GPADIR.bit.GPIO27 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO27 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-28 - PIN FUNCTION = SCI-RX
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 =
        1; // 0=GPIO,  1=SCIRX-A,  2=Resv,  3=Resv
           //	GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO28 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-29 - PIN FUNCTION = SCI-TX
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 =
        1; // 0=GPIO,  1=SCITXD-A,  2=XA19,  3=Resv
           //	GpioCtrlRegs.GPADIR.bit.GPIO29 = 0;		// 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO29 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-30 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 0; // 0=GPIO,  1=CANRX-A,  2=XA18,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO30 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPACLEAR.bit.GPIO30 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPASET.bit.GPIO30 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-31 - PIN FUNCTION = LED2 (for Release 1.1 and up F2833x controlCARDs)
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 0; // 0=GPIO,  1=CANTX-A,  2=XA17,  3=Resv
    GpioCtrlRegs.GPADIR.bit.GPIO31 =
        1;                              // 1=OUTput,  0=INput
                                        //	GpioDataRegs.GPACLEAR.bit.GPIO31 = 1;	// uncomment if --> Set Low initially
    GpioDataRegs.GPASET.bit.GPIO31 = 1; // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //  GPIO-32 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0; // 0=GPIO,  1=I2C-SDA,  2=SYNCI,  3=ADCSOCA
    GpioCtrlRegs.GPBDIR.bit.GPIO32 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO32 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-33 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0; // 0=GPIO,  1=I2C-SCL,  2=SYNCO,  3=ADCSOCB
    GpioCtrlRegs.GPBDIR.bit.GPIO33 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO33 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-34 - PIN FUNCTION = LED3 (for Release 1.1 and up F2833x controlCARDs)
    GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0; // 0=GPIO,  1=ECAP1,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO34 =
        1;                              // 1=OUTput,  0=INput
                                        //	GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;	// uncomment if --> Set Low initially
    GpioDataRegs.GPBSET.bit.GPIO34 = 1; // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //  GPIO-35 - PIN FUNCTION = --Spare-- (SCI-TX on R1 F2833x controlCARD)
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0; // 0=GPIO,  1=SCIA-TX,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO35 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO35 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-36 - PIN FUNCTION = --Spare-- (SCI-RX on R1 F2833x controlCARD)
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0; // 0=GPIO,  1=SCIA-RX,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO36 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO36 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-38 - PIN FUNCTION = LED2 (for Release 1 F2833x controlCARDs)
    GpioCtrlRegs.GPBMUX1.bit.GPIO38 = 0; // 0=GPIO,  1=Resv,  2=Resv,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO38 =
        1;                              // 1=OUTput,  0=INput
                                        //	GpioDataRegs.GPBCLEAR.bit.GPIO38 = 1;	// uncomment if --> Set Low initially
    GpioDataRegs.GPBSET.bit.GPIO38 = 1; // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-39 - PIN FUNCTION = LED3 (for Release 1 F2833x controlCARDs)
    GpioCtrlRegs.GPBMUX1.bit.GPIO39 = 0; // 0=GPIO,  1=Resv,  2=XA16,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO39 =
        1;                              // 1=OUTput,  0=INput
                                        //	GpioDataRegs.GPBCLEAR.bit.GPIO39 = 1;	// uncomment if --> Set Low initially
    GpioDataRegs.GPBSET.bit.GPIO39 = 1; // uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------

    //  GPIO-48 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0; // 0=GPIO,  1=ECAP5,  2=XD31,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO48 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO48 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-49 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0; // 0=GPIO,  1=ECAP6,  2=XD30,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO49 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO49 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------

    //  GPIO-58 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0; // 0=GPIO,  1=MCLKR-A,  2=XD21,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO58 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO58 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-59 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0; // 0=GPIO,  1=MFSR-A,  2=XD20,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO59 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO59 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO59 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-60 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0; // 0=GPIO,  1=MCLKR-B,  2=XD19,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO60 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO60 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO60 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-61 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0; // 0=GPIO,  1=MFSR-B,  2=XD18,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO61 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO61 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO61 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-62 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0; // 0=GPIO,  1=SCIRX-C,  2=XD17,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO62 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO62 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO62 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-63 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0; // 0=GPIO,  1=SCITX-C,  2=XD16,  3=Resv
    GpioCtrlRegs.GPBDIR.bit.GPIO63 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPBSET.bit.GPIO63 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------

    //  GPIO-84 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 0; // 0=GPIO,  1=GPIO,  2=XA12,  3=Resv
    GpioCtrlRegs.GPCDIR.bit.GPIO84 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPCCLEAR.bit.GPIO84 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPCSET.bit.GPIO84 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-85 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 0; // 0=GPIO,  1=GPIO,  2=XA13,  3=Resv
    GpioCtrlRegs.GPCDIR.bit.GPIO85 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPCCLEAR.bit.GPIO85 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPCSET.bit.GPIO85 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-86 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 0; // 0=GPIO,  1=GPIO,  2=XA14,  3=Resv
    GpioCtrlRegs.GPCDIR.bit.GPIO86 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPCCLEAR.bit.GPIO86 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPCSET.bit.GPIO86 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------
    //  GPIO-87 - PIN FUNCTION = --Spare--
    GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 0; // 0=GPIO,  1=GPIO,  2=XA15,  3=Resv
    GpioCtrlRegs.GPCDIR.bit.GPIO87 =
        0; // 1=OUTput,  0=INput
           //	GpioDataRegs.GPCCLEAR.bit.GPIO87 = 1;	// uncomment if --> Set Low initially
           //	GpioDataRegs.GPCSET.bit.GPIO87 = 1;		// uncomment if --> Set High initially
    //--------------------------------------------------------------------------------------

    EDIS; 
}
