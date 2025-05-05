// MPPT Algorithm: Perturb and Observe
// reference implement: https://ww2.mathworks.cn/discovery/mppt-algorithm.html
#ifndef _FILE_PNO_ALGORITHM_H_
#define _FILE_PNO_ALGORITHM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct _tag_mppt_pno_algo
    {
        // input section
        // ctrl_gt current_voltage;
        // ctrl_gt current_current;

        // output section
        ctrl_gt v_ref;

        // intermediate section
        ctrl_gt last_voltage;
        ctrl_gt last_power;

        ctrl_gt current_power;

        // parameterss
        ctrl_gt voltage_increment;

        // flag enable MPPT controller
        fast_gt flag_enable_mppt;

        // output limit
        ctrl_gt max_voltage_limit;
        ctrl_gt min_voltage_limit;

    } mppt_po_algo_t;

    // current_voltage current voltage measurement result
    // current_current current current measurement result
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_mppt_inc_algo(mppt_po_algo_t *mppt, ctrl_gt current_voltage, ctrl_gt current_current)
    {
        mppt->current_power = ctl_mul(current_voltage, current_current);

        ctrl_gt delta_power = mppt->current_power - mppt->last_power;
        ctrl_gt delta_voltage = current_voltage - mppt->last_voltage;

        // MPPT algorithm
        if (mppt->flag_enable_mppt)
        {
            if (delta_power > 0)
            {
                if (delta_voltage > 0)
                    // decrease vref
                    mppt->v_ref -= mppt->voltage_increment;
                else
                    // increase vref
                    mppt->v_ref += mppt->voltage_increment;
            }
            else if (delta_power < 0)
            {
                if (delta_voltage > 0)
                    // decrease vref
                    mppt->v_ref += mppt->voltage_increment;
                else
                    // increase vref
                    mppt->v_ref -= mppt->voltage_increment;
            }
            // else if (delta_power == 0)
            // keep vref
        }

        // voltage limit range
        mppt->v_ref = ctl_sat(mppt->v_ref, mppt->min_voltage_limit, mppt->max_voltage_limit);

        // update last value
        mppt->last_power = mppt->current_power;
        mppt->last_voltage = current_voltage;

        // return vref
        return mppt->v_ref;
    }

    GMP_STATIC_INLINE
    void ctl_enable_mppt_po_algo(mppt_po_algo_t *mppt)
    {
        mppt->flag_enable_mppt = 1;
    }

    GMP_STATIC_INLINE
    void ctl_disable_mppt_po_algo(mppt_po_algo_t *mppt)
    {
        mppt->flag_enable_mppt = 0;
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PNO_ALGORITHM_H_
