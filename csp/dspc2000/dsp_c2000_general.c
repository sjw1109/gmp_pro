
// invoke GMP header
#include <core/gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// Timer function 

// This function get the system tick.
time_gt gmp_port_system_tick(
    void
)
{
    return g_cpu_hal_tick;
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

//
#define EPWM1_TIMER_TBPRD  2000  // Period register
#define EPWM1_MAX_CMPA     1950
#define EPWM1_MIN_CMPA       50
#define EPWM1_MAX_CMPB     1950
#define EPWM1_MIN_CMPB       50
#define EPWM1_DEADBAND       16

void init_epwm_updown(struct EPWM_REGS* handle_epwm,
 uint16_t timer_period,
 uint16_t cmp_a_init,
 uint16_t cmp_b_init,
 uint16_t db_rising,
 uint16_t db_falling
)
{
    // handle of EPWM module
    volatile struct EPWM_REGS* epwm = handle_epwm;

    // Sets the pulse width modulation (PWM) one shot trip
    EALLOW;
    epwm->TZFRC.bit.OST = 1;
    EDIS;

    // Set Time Base counter mode
    epwm->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up & down
    epwm->TBCTL.bit.PHSEN = TB_DISABLE; // Disable phase loading
    epwm->TBCTL.bit.PRDLD = TB_IMMEDIATE; // period load
    epwm->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    epwm->TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    epwm->TBCTL.bit.CLKDIV = TB_DIV1;
    epwm->TBCTL.bit.PHSDIR = TB_UP;
    epwm->TBCTL.bit.FREE_SOFT = 2; // Free Run mode

    // Stop Time Base clk, for synchronization
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    // Set Timer base period and phase
    epwm->TBPRD = timer_period;     // Set timer period 801 TBCLKs
    epwm->TBPHS.half.TBPHS = 0x0000;     // Phase is 0
    epwm->TBCTR = 0x0000;                // Clear counter

    // Set Counter Compare shadowing
    epwm->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
    epwm->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;
    epwm->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    epwm->CMPCTL.bit.SHDWBMODE = CC_IMMEDIATE;

    // Set Action-Qualifier
    epwm->AQCTLA.bit.CAU = AQ_SET;    // Set PWM1A on event A, up count
    epwm->AQCTLA.bit.CAD = AQ_CLEAR;  // Clear PWM1A on event A, down count

    epwm->AQCTLB.bit.CBU = AQ_SET;    // Set PWM1B on event B, up count
    epwm->AQCTLB.bit.CBD = AQ_CLEAR;  // Clear PWM1B on event B, down count

    // set dead-band generator
    epwm->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
    epwm->DBCTL.bit.POLSEL = DB_ACTV_HIC;

    // setup the Dead-Band Rising Edge Delay Register (DBRED)
    epwm->DBRED = db_rising;
    // setup the Dead-Band Falling Edge Delay Register (DBFED)
    epwm->DBFED = db_falling;

    // Disable PWM-Chopper
    epwm->PCCTL.bit.CHPEN = CHP_DISABLE;

    // Trip Zone Select
    EALLOW;
    epwm->TZSEL.all = 0;
    EDIS;

    // Event Trigger config
    // Disable INT
    epwm->ETSEL.bit.INTEN = 0;

    // SOC-A event
    epwm->ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    epwm->ETSEL.bit.SOCAEN = 1; // enable SOCA

    // SOC-A interrupt period
    epwm->ETPS.bit.INTPRD = ET_2ND;
    // SOC-A period
    epwm->ETPS.bit.SOCAPRD = ET_2ND;

    // Clear Event Trigger
    epwm->ETCLR.bit.INT = 1;
    epwm->ETCLR.bit.SOCA = 1;

    // Set Compare values
    epwm->CMPA.half.CMPA = cmp_a_init;     // Set compare A value
    epwm->CMPB = cmp_b_init;               // Set Compare B value

    // Enable Time Base clock to synchronize the pwms
//    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;

}


void enable_timebase_clock()
{
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
}


void c2000_init_gpio(void)
{
    // Phase I: kernel peripiherals GPIO definitions

    // Enable Sci-A and Sci-B gpio properties.
    // These two functions are defined in the F2806x_Sci.c file.
    InitSciaGpio();
    InitScibGpio();

    // Phase II: User GPIO definitions

}

void gmp_csp_startup()
{
    /// .........................................................///
    // Phase I: Initialize System Control Registers, 
    // reset PLL, WatchDog, Clocks to default state.
    // Defined in the F2806x_SysCtrl.c file.
    InitSysCtrl();

    /// .........................................................///
    // Phase II: Sepecify all the GPIO usage.
    c2000_init_gpio();

    /// .........................................................///
    // Phase III: Stop all the interrupts & config system interrupt.
    
    // Disable CPU interrupts
    DINT;

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags are cleared.
    // This function is found in the F2806x_PieCtrl.c file.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2806x_DefaultIsr.c.
    // This function is found in F2806x_PieVect.c.
    InitPieVectTable();

    /// .........................................................///
    // For now, all the system core module have been ready
    // User may start config peripherals and interrupt tables.
    // After all the peripherals setup process is done, then enable interrupt.
    // These process will be done in `gmp_set_peripheral()` function.

}

typedef volatile struct CPUTIMER_REGS peripheral_timer_t;



// Init all the DSP peripheral & register all the interrupt vector
void gmp_setup_peripheral()
{
    /// .........................................................///
    // Stage I: CPU timer initilize
    
    // This function is defined in `F2806x_CpuTimers.c`
    // This function may init three CPU timer handle.
    InitCpuTimers();

    // CPU timer1 will treated as system tick clk,
    // the frequency of the timer1 is 1kHz,
    // If user need to add some more routine for the CpuTimer1 interrupt isr
    // please use callback function.
    ConfigCpuTimer(&CpuTimer1, SYSTEM_MAIN_CLK_MHz, 1000);

    //
    // To ensure precise timing, use write-only instructions to write to the 
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the below 
    // settings must also be updated.
    //

    //
    // Use write-only instruction to set TSS bit = 0,
    // Enable the CPU Timer1
    //
    CpuTimer1Regs.TCR.all = 0x4000;


    //

    init_epwm_updown(&EPwm3Regs, EPWM1_TIMER_TBPRD, EPWM1_MAX_CMPA, EPWM1_MAX_CMPB, EPWM1_DEADBAND, EPWM1_DEADBAND);

    // re - mapped to ISR functions
    EALLOW; 
    PieVectTable.TINT1 = &cpu_tick_routine;
    EDIS;

    // enable interrupt channel
    // CPU timer interrupt is mapped to INT13.7;
    IER |= M_INT13;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    /// .........................................................///
    // Stage II: ePWM channels




    /// .........................................................///
    // Stage X: Final Round
    // Enable global Interrupts and higher priority real-time debug events:
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

}

// GPIO Control handle
// 
// This is the only entry point of GPIO Data Register pointer.
//gpio_dat_t* gpio_dat_handle = (gpio_dat_t*)&GpioDataRegs;

//////////////////////////////////////////////////////////////////////////
// IO peripheral function implement

// CPU tick routine

time_gt g_cpu_hal_tick;

peripheral_timer_t* cpu_tick_peripheral;

interrupt 
void cpu_tick_routine(void)
{
    g_cpu_hal_tick += 1;
}


