// This is an example of peripheral.c

// GMP basic core header
#include <gmp_core.h>

#include <ctl/ctl.config.h>

//////////////////////////////////////////////////////////////////////////
// Hardware related global variables

//////////////////////////////////////////////////////////////////////////
// Devices on the peripheral

// User should setup all the peripheral in this function.
void setup_peripheral(void)
{
}

//////////////////////////////////////////////////////////////////////////
// device related functions
#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
{
}

fast_gt ctl_fmif_security_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return GMP_EC_OK;
}

#endif

//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here

// This function should be called in Main ISR
void gmp_base_ctl_step(void);
