// MPPT Algorithm: Perturb and Observe
// reference implement: https://ww2.mathworks.cn/discovery/mppt-algorithm.html

#include <ctl/component/intrinsic/discrete/discrete_filter.h>
#include <ctl/component/intrinsic/discrete/divider.h>

#ifndef _FILE_PNO_ALGORITHM_H_
#define _FILE_PNO_ALGORITHM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// This value means the border of the searching
#define CTL_MPPT_PNO_EPSILON (float2ctrl(-0.000001))

typedef struct _tag_mppt_PnO_algo
{
    // input section
    // ctrl_gt current_voltage;
    // ctrl_gt current_current;

    //
    // output section
    //
    ctrl_gt v_ref;

    //
    // parameters
    //

    // voltage searching step range
    // voltage increment will initially set to voltage_increment
    // and will generally reach searching_step_min after convergence
    ctrl_gt searching_step_max;
    ctrl_gt searching_step_min;

    // voltage increment attenuation
    ctrl_gt inc_attenuation;

    // output limit
    ctrl_gt max_voltage_limit;
    ctrl_gt min_voltage_limit;

    //
    // intermediate
    //

    ctrl_gt last_power;
    ctrl_gt current_power;

    ctrl_gt last_voltage;

    ctrl_gt delta_power;
    ctrl_gt delta_voltage;

    // voltage searching step size.
    ctrl_gt voltage_increment;

    // mppt up/down command record
    // 0: down, 1: up
    // bit0 is current movement, bit1 is last movement, and so forth.
    uint32_t mppt_record;

    // filter for power
    // filter stop frequency should greater than 5 times MPPT frequency
    ctl_low_pass_filter_t power_filter;

    // divider for MPPT algorithm
    ctl_divider_t divider;

    //
    // flag stack
    //

    // flag enable MPPT controller
    fast_gt flag_enable_mppt;

    // flag enable adaptive step size control
    fast_gt flag_enable_adaptive_step_size;

} mppt_PnO_algo_t;

GMP_STATIC_INLINE
ctrl_gt ctl_step_mppt_PnO_algo(
    // MPPT object handle
    mppt_PnO_algo_t *mppt,
    // current_voltage current voltage measurement result
    ctrl_gt current_voltage,
    // current_current current current measurement result
    ctrl_gt current_current)
{
    mppt->current_power = ctl_step_lowpass_filter(mppt->power_filter, ctl_mul(current_voltage, current_current));

    mppt->delta_power = mppt->current_power - mppt->last_power;
    mppt->delta_voltage = current_voltage - mppt->last_voltage;

    // MPPT algorithm
    if (mppt->flag_enable_mppt)
    {
        // MPPT controller divider
        if (ctl_step_divider(&mppt->divider))
        {

            if (mppt->delta_power > CTL_MPPT_PNO_EPSILON)
            {
                if (mppt->delta_voltage > 0)
                {
                    // increase vref
                    mppt->v_ref += mppt->voltage_increment;
                    mppt_record = mppt_record << 1 + 1;
                }
                else
                {
                    // decrease vref
                    mppt->v_ref -= mppt->voltage_increment;
                    mppt_record = mppt_record << 1;
                }
            }
            else // if (mppt->delta_power < CTL_MPPT_PNO_EPSILON)
            {
                if (mppt->delta_voltage > 0)
                {
                    // decrease vref
                    mppt->v_ref -= mppt->voltage_increment;
                    mppt_record = mppt_record << 1;
                }
                else
                {
                    // increase vref
                    mppt->v_ref += mppt->voltage_increment;
                    mppt_record = mppt_record << 1 + 1;
                }
            }
            // else if (delta_power == 0)
            // keep vref

            if (mppt->flag_enable_adaptive_step_size)
            {
                // if MPPT algorithm reach the oscillating, decrease searching step
                if (
                    // level I oscillation
                    ((mppt->mppt_record & 0x07) == 2) || ((mppt->mppt_record & 0x07) == 5) ||
                    // level II oscillation
                    ((mppt->mppt_record & 0x0F) == 12) || ((mppt->mppt_record & 0x0F) == 3))
                {
                    mppt->voltage_increment = ctl_mul(mppt->min_voltage_limit, mppt->inc_attenuation) +
                                              ctl_mul(mppt->voltage_increment, GMP_CONST_1 - mppt->inc_attenuation);
                }

                // if MPPT algorithm heads straight towards special direction
                else if (((mppt->mppt_record & 0x0F) == 0x0F) || ((mppt->mppt_record & 0x0F) == 0x00))
                {
                    mppt->voltage_increment = mppt->max_voltage_limit;
                }
            }

            // voltage limit range
            mppt->v_ref = ctl_sat(mppt->v_ref, mppt->min_voltage_limit, mppt->max_voltage_limit);

            // update last value
            mppt->last_power = mppt->current_power;
            mppt->last_voltage = mppt->current_voltage;
        } // end of divider
    }     // end of flag enable MPPT

    // return vref
    return mppt->v_ref;
}

GMP_STATIC_INLINE
void ctl_clear_mppt_PnO_algo(mppt_PnO_algo_t *mppt)
{
    ctl_clear_lowpass_filter(mppt->power_filter);
    ctl_clear_divider(mppt->divider);

    mppt->voltage_increment = mppt->searching_step_max;

    mppt->last_power = 0;
    mppt->current_power = 0;
    mppt->last_voltage = 0;
    mppt->delta_power = 0;
    mppt->delta_voltage = 0;

    mppt->mppt_record = 0;
}

GMP_STATIC_INLINE
void ctl_enable_mppt_PnO_algo(mppt_PnO_algo_t *mppt)
{
    mppt->flag_enable_mppt = 1;
}

GMP_STATIC_INLINE
void ctl_disable_mppt_PnO_algo(mppt_PnO_algo_t *mppt)
{
    mppt->flag_enable_mppt = 0;
}

GMP_STATIC_INLINE
void ctl_enable_adaptive_step_size(mppt_PnO_algo_t *mppt)
{
    mppt->flag_enable_adaptive_step_size = 1;
}

GMP_STATIC_INLINE
void ctl_disable_adaptive_step_size(mppt_PnO_algo_t *mppt)
{
    mppt->flag_enable_adaptive_step_size = 0;
}

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
    parameter_gt freq_ctrl);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PNO_ALGORITHM_H_
