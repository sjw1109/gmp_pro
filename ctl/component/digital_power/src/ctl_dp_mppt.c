#include <gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// BOOST Control
#include <ctl/component/digital_power/mppt/PnO_algorithm.h>

void ctl_init_mppt_PnO_algo(
    // MPPT object handle
    mppt_PnO_algo_t *mppt,
    // initial voltage reference
    parameter_gt voltage_0,
    // scope of search
    parameter_gt searching_range_max, parameter_gt searching_range_min,
    // Range of search speed
    parameter_gt searching_step_max, parameter_gt searching_step_min,
    // Convergence time constant of search speed when reaching steady state
    parameter_gt attenuation_time,
    // MPPT algorithm divider
    parameter_gt freq_mppt,
    // ISR frequency
    parameter_gt freq_ctrl)
{
    // power filter
    ctl_init_lp_filter(&mppt->power_filter, freq_ctrl, freq_mppt * 6.0f);

    // divider
    ctl_init_divider(&mppt->divider, freq_ctrl / freq_mppt);

    // initial voltage
    mppt->v_ref = voltage_0;

    mppt->searching_step_max = searching_step_max;
    mppt->searching_step_min = searching_step_min;

    mppt->max_voltage_limit = searching_range_max;
    mppt->min_voltage_limit = searching_range_min;

    mppt->inc_attenuation = ctl_helper_lp_filter(freq_mppt, 1.0f / attenuation_time);

    ctl_clear_mppt_PnO_algo(mppt);

    ctl_disable_mppt_PnO_algo(mppt);
    ctl_enable_adaptive_step_size(mppt);
}
