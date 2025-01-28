
// This is an example file for GMP project with Controller template.

// GMP basic core header
#include <gmp_core.h>

// Controller Template Library
#include <ctl/ctl_core.h>

// user main header
#include "user_main.h"

//////////////////////////////////////////////////////////////////////////
// initialize routine here

void ctl_init(void)
{

    return;
}

//////////////////////////////////////////////////////////////////////////
// endless loop function here

void ctl_mainloop(void)
{
    return;
}


//////////////////////////////////////////////////////////////////////////
// ISR callback function here

void ctl_dispatch(void)
{
return ;
}

//////////////////////////////////////////////////////////////////////////
// Controller Framework

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

// controller core
void ctl_fmif_core_stage_routine(ctl_object_nano_t *pctl_obj)
{

}

void ctl_fmif_monitor_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
}


// return value:
// 1 change to next progress
// 0 keep the same state
fast_gt ctl_fmif_sm_pending_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}

// return value:
// 1 change to next progress
// 0 keep the same state
fast_gt ctl_fmif_sm_calibrate_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 1;
}


fast_gt ctl_fmif_sm_ready_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}

// Main relay close, power on the main circuit
fast_gt ctl_fmif_sm_runup_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 1;
}


fast_gt ctl_fmif_sm_online_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}


fast_gt ctl_fmif_sm_fault_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}


#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
