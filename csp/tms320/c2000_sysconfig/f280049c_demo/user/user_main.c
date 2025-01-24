// This is the example of user main.
 
//////////////////////////////////////////////////////////////////////////
// headers here

// GMP basic core header
#include <gmp_core.h>

// Controller Template Library
#include <ctl/ctl_core.h>

// user main header
#include "user_main.h"


//////////////////////////////////////////////////////////////////////////
// global variables here



//////////////////////////////////////////////////////////////////////////
// initialize routine here
GMP_NO_OPT_PREFIX
void init(void)
GMP_NO_OPT_SUFFIX
{
    // light two LED
    gmp_hal_gpio_write(LEDR, 0);
    gmp_hal_gpio_write(LEDG, 1);

    fVal = 10;
    GMP_DBG_SWBP;

    // Clear Interrupt
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP11);

    CLA_forceTasks(CLA1_BASE, CLA_TASKFLAG_1);

    asm(" RPT #255 || NOP");

    fVal = fResult;
}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
void mainloop(void)
{
    gmp_hal_gpio_toggle(LEDG);

    CLA_forceTasks(CLA1_BASE, CLA_TASKFLAG_1);

    asm(" RPT #255 || NOP");

    fVal = fResult;

//    GMP_DBG_SWBP;

}




