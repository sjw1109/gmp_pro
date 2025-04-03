
#ifndef FILE_F28X_PWM_DRIVER_H_
#define FILE_F28X_PWM_DRIVER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*----------------------------------------------------------------------------
Initialization constant for the F2803X Time-Base Control Registers for PWM Generation.
Sets up the timer to run free upon emulation suspend, count up-down mode
prescaler 1.
----------------------------------------------------------------------------*/
#define PWM_INIT_STATE                                                                                                 \
    (FREE_RUN_FLAG + PRDLD_IMMEDIATE + TIMER_CNT_UPDN + HSPCLKDIV_PRESCALE_X_1 + CLKDIV_PRESCALE_X_1 + PHSDIR_CNT_UP + \
     CNTLD_DISABLE)

/*----------------------------------------------------------------------------
Initialization constant for the F2803X Compare Control Register.
----------------------------------------------------------------------------*/
#define CMPCTL_INIT_STATE (LOADAMODE_ZRO + LOADBMODE_ZRO + SHDWAMODE_SHADOW + SHDWBMODE_SHADOW)

/*----------------------------------------------------------------------------
Initialization constant for the F2803X Action Qualifier Output A Register.
----------------------------------------------------------------------------*/
#define AQCTLA_INIT_STATE (CAD_SET + CAU_CLEAR)

/*----------------------------------------------------------------------------
Initialization constant for the F2803X Dead-Band Generator registers for PWM Generation.
Sets up the dead band for PWM and sets up dead band values.
----------------------------------------------------------------------------*/
#define DBCTL_INIT_STATE (BP_ENABLE + POLSEL_ACTIVE_HI_CMP)

#define DBCNT_INIT_STATE 100 // 100 counts = 1.66 usec (delay) * 100 count/usec (for TBCLK = SYSCLK/1)

/*----------------------------------------------------------------------------
Initialization constant for the F2803X PWM Chopper Control register for PWM Generation.
----------------------------------------------------------------------------*/
#define PCCTL_INIT_STATE CHPEN_DISABLE

/*----------------------------------------------------------------------------
Initialization constant for the F2803X Trip Zone Select Register
----------------------------------------------------------------------------*/
#define TZSEL_INIT_STATE DISABLE_TZSEL

    /*----------------------------------------------------------------------------
Initialization constant for the F2833X Trip Zone Control Register
----------------------------------------------------------------------------*/
#define TZCTL_INIT_STATE (TZA_HI_Z + TZB_HI_Z + DCAEVT1_HI_Z + DCAEVT2_HI_Z + DCBEVT1_HI_Z + DCBEVT2_HI_Z)

    /*-----------------------------------------------------------------------------
        Define the structure of the PWM Driver Object
    -----------------------------------------------------------------------------*/
    typedef struct
    {
        Uint16 PeriodMax;  // Parameter: PWM Half-Period in CPU clock cycles (Q0)
        Uint16 HalfPerMax; // Parameter: Half of PeriodMax 				  (Q0)
        Uint16 Deadband;   // Parameter: PWM deadband in CPU clock cycles    (Q0)
        _iq MfuncC1;       // Input: EPWM1 A&B Duty cycle ratio (Q24)
        _iq MfuncC2;       // Input: EPWM2 A&B Duty cycle ratio (Q24)
        _iq MfuncC3;       // Input: EPWM3 A&B Duty cycle ratio (Q24)
    } PWMGEN;

    /*------------------------------------------------------------------------------
    Default Initializers for the F2803X PWMGEN Object
    ------------------------------------------------------------------------------*/
#define PWMGEN_DEFAULTS                                                                                                \
    {                                                                                                                  \
        1000, 1000, 100, 0x4000, 0x4000, 0x4000,                                                                       \
    }

    /*-----------------------------------------------------------------------------
        Define the structure of the Dead Band Compensator (DBC) Object
    -----------------------------------------------------------------------------*/
    typedef struct
    {
        _iq Kdtc,  // constant K
            Ith,   // phase current threshold
            scale, // ratio of deabdand count to current threshold
            gain;  // scale to adjust dead time compensation
    } DBC;

/*------------------------------------------------------------------------------
    Default Initializers for the DBC Object
------------------------------------------------------------------------------*/
#define DBC_DEFAULTS                                                                                                   \
    {                                                                                                                  \
        _IQ(0.0), _IQ(0.001), _IQ(0.0), _IQ(0.0) /* _IQ(0.5) */                                                        \
    }

    void PWMx_INIT_MACRO(volatile struct EPWM_REGS *ePWMx, PWMGEN *v)
    {
        /* Init Timer-Base Control Register for EPWMx */
        (*ePWMx).TBCTL.all = PWM_INIT_STATE;

        ///* Setup Sync*/
        //(*ePWMx).TBCTL.bit.SYNCOSEL = 0; /* Pass through*/

        ///* Allow each timer to be sync'ed*/
        //(*ePWMx).TBCTL.bit.PHSEN = 1;

        /* Init Timer-Base Period Register EPWMx */
        (*ePWMx).TBPRD = v->PeriodMax;

        /* Init Timer-Base Phase Register for EPWMx */
        (*ePWMx).TBPHS.half.TBPHS = 0;

        /* Init Compare Control Register for EPWMx */
        (*ePWMx).CMPCTL.all = CMPCTL_INIT_STATE;

        /* Init Action Qualifier Output A Register for EPWMx */
        (*ePWMx).AQCTLA.all = AQCTLA_INIT_STATE;

        /* Init Dead-Band Generator Control Register for EPWMx */
        (*ePWMx).DBCTL.all = DBCTL_INIT_STATE;

        /* Init Dead-Band Generator for EPWMx */
        (*ePWMx).DBFED = v->Deadband;
        (*ePWMx).DBRED = v->Deadband;

        /* Init PWM Chopper Control Register for EPWMx */
        (*ePWMx).PCCTL.all = PCCTL_INIT_STATE;

        /* Enable EALLOW */
        EALLOW;
        /* Init Trip Zone Select Register*/
        (*ePWMx).TZSEL.all = TZSEL_INIT_STATE;

        /* Init Trip Zone Select Register*/
        (*ePWMx).TZSEL.all = TZSEL_INIT_STATE;
        EDIS; /* Disable EALLOW*/
    }

    static inline
    void write_pwm_duty(volatile struct EPWM_REGS* ePWMx, uint16_t cmp)
    {
        (*ePWMx).CMPA.half.CMPA = cmp;  
    }

    /*  Mfuncx range is (-1,1)														*/                                                                         \
    /*  The code below changes PeriodMax*Mfuncx range ....  						*/                                                   \
    /*  from (-PeriodMax,PeriodMax) to (0,PeriodMax) where HalfPerMax=PeriodMax/2	*/   
    void write_xpwm_channel(uint16_t channel1, uint16_t channel2, uint16_t channel3, PWMGEN v)
    {
        (*ePWM[ch1]).CMPA.half.CMPA = _IQmpy(v.HalfPerMax, v.MfuncC1) + v.HalfPerMax;
        (*ePWM[ch2]).CMPA.half.CMPA = _IQmpy(v.HalfPerMax, v.MfuncC2) + v.HalfPerMax;
        (*ePWM[ch3]).CMPA.half.CMPA = _IQmpy(v.HalfPerMax, v.MfuncC3) + v.HalfPerMax;
    }

    
///************************************************************************
// *  PWM Gen w Dead Band Compensation Macro
// *     if curA > 0   Ta = Ta + Tdt
// *     else          Ta = Ta - Tdt
// ************************************************************************/
//#define PWMwDBC_MACRO(ch1, ch2, ch3, m, i, d)                                                                          \
//    (*ePWM[ch1]).CMPA.half.CMPA =                                                                                      \
//        _IQmpy(m.HalfPerMax, m.MfuncC1) + m.HalfPerMax + _IQmpy(_IQsat(i.As, d.Ith, -d.Ith), d.Kdtc);                  \
//    (*ePWM[ch2]).CMPA.half.CMPA =                                                                                      \
//        _IQmpy(m.HalfPerMax, m.MfuncC2) + m.HalfPerMax + _IQmpy(_IQsat(i.Bs, d.Ith, -d.Ith), d.Kdtc);                  \
//    (*ePWM[ch3]).CMPA.half.CMPA =                                                                                      \
//        _IQmpy(m.HalfPerMax, m.MfuncC3) + m.HalfPerMax + _IQmpy(_IQsat(i.Cs, d.Ith, -d.Ith), d.Kdtc);
//


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FILE_F28X_PWM_DRIVER_H_
