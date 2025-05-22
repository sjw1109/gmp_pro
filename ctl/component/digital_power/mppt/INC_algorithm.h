// MPPT Algorithm: INCremental Conductance Method
// reference implement: https://ww2.mathworks.cn/discovery/mppt-algorithm.html

#ifndef _FILE_INC_ALGORITHM_H_
#define _FILE_INC_ALGORITHM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_mppt_inc_algo
{
    // input section
    // ctrl_gt current_voltage;
    // ctrl_gt current_current;

    // output section
    ctrl_gt v_ref;

    // intermediate section
    ctrl_gt last_voltage;
    ctrl_gt last_current;

    // parameterss
    ctrl_gt voltage_increment;

    // flag enable MPPT controller
    fast_gt flag_enable_mppt;

    // output limit
    ctrl_gt max_voltage_limit;
    ctrl_gt min_voltage_limit;

} mppt_inc_algo_t;

// current_voltage current voltage measurement result
// current_current current current measurement result
GMP_STATIC_INLINE
ctrl_gt ctl_step_mppt_inc_algo(mppt_inc_algo_t *mppt, ctrl_gt current_voltage, ctrl_gt current_current)
{
    mppt->current_power = ctl_mul(current_voltage, current_current);

    ctrl_gt delta_current = current_current - mppt->last_current;
    ctrl_gt delta_voltage = current_voltage - mppt->last_voltage;

    ctrl_gt delta_current_judge;

    // MPPT algorithm
    if (mppt->flag_enable_mppt)
    {
        if (delta_voltage == 0)
        {
            if (delta_current > 0)
                // increase vref
                mppt->v_ref += mppt->voltage_increment;
            else if (delta_current < 0)
                // decrease vref
                mppt->v_ref -= mppt->voltage_increment;
        }
        else
        {
            delta_current_judge = current_current + ctl_mul(ctl_div(delta_current, delta_voltage), current_voltage);

            if (delta_current_judge > 0)
                // increase vref
                mppt->v_ref += mppt->voltage_increment;
            else if (delta_current_judge < 0)
                // decrease vref
                mppt->v_ref -= mppt->voltage_increment;
        }
    }

    // voltage limit range
    mppt->v_ref = ctl_sat(mppt->v_ref, mppt->min_voltage_limit, mppt->max_voltage_limit);

    // update last value
    mppt->last_voltage = current_voltage;
    mppt->last_current = current_current;

    // return vref
    return mppt->v_ref;
}

GMP_STATIC_INLINE
void ctl_enable_mppt_inc_algo(mppt_inc_algo_t *mppt)
{
    mppt->flag_enable_mppt = 1;
}

GMP_STATIC_INLINE
void ctl_disable_mppt_inc_algo(mppt_inc_algo_t *mppt)
{
    mppt->flag_enable_mppt = 0;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_INC_ALGORITHM_H_
