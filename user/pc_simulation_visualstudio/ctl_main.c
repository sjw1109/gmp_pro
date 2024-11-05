/**
 * @file ctl_main.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/ctl_core.h>
#include <gmp_core.h>
#include <math.h>

// User may set (get handle) TX content via `gmp_csp_sl_get_tx_buffer`
// User may get (get handle) RX content via `gmp_csp_sl_get_rx_buffer`

ctl_object_nano_t ctl_obj;

// CTL initialize routine
void ctl_init()
{
    ctl_fm_init_nano_header(&ctl_obj);
    ctl_fm_setup_nano_header(&ctl_obj, (uint32_t)10e3);

    ctl_setup_default_ctl_nano_obj(&ctl_obj);

    ctl_fm_force_online(&ctl_obj);
}

// CTL loop routine
void ctl_dispatch(void)
{

    // User Controller logic here.
}

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_core_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
    gmp_csp_sl_get_tx_buffer()->output1 = gmp_csp_sl_get_rx_buffer()->input1;
}

void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
{
}

void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_enable_output();
}

void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
{
    csp_sl_disable_output();
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
