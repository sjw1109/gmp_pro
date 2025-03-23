/**
 * @file adc_channel.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// #include <gmp_core.h>
#include <ctl/component/interface/interface_base.h>

#ifndef _FILE_ADC_CHANNEL_H_
#define _FILE_ADC_CHANNEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // Single ADC Channel

    typedef struct _tag_adc_channel_t
    {
        // OUTPUT the ADC data in ctrl_gt type
        // Formal expression: value = gain * (raw - gain);
        adc_ift control_port;

        // INPUT raw data from adc
        adc_gt raw;

        // resolution of ADC bits
        // 16bit right aligned, 16
        // 12bit right aligned, 12
        // 12bit left aligned, 16
        fast_gt resolution;
        // The result of ADC is IQn number,IQ24->24
        fast_gt iqn;

        // The bias of the ADC data
        ctrl_gt bias;
        // The gain of the ADC data
        // Negative gain is permitted
        ctrl_gt gain;

    } adc_channel_t;

    // init object function
    void ctl_init_adc_channel(adc_channel_t *adc_obj, ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn);

    // transfer raw data to final value
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_adc_channel(adc_channel_t *adc, adc_gt raw)
    {
        adc->raw = raw;

#if defined CTRL_GT_IS_FIXED
        // for fixed point number, transfer ADC data to _IQ24
        ctrl_gt raw_data = adc->raw << (adc->iqn - adc_obj->resolution);
#elif defined CTRL_GT_IS_FLOAT
    // for float point number, transfer ADC data to p.u.
    ctrl_gt raw_data = (ctrl_gt)adc->raw / (1 << adc->resolution);
#else
#error("The system does not specify ctrl_gt is float or fixed. You should define CTRL_GT_IS_FLOAT or CTRL_GT_IS_FIXED.")
#endif // CTRL_GT_IS_XXX

        // remove bias
        ctrl_gt raw_without_bias = raw_data - adc->bias;

        // Gain
        adc->control_port.value = ctl_mul(raw_without_bias, adc->gain);

        return adc->control_port.value;
    }

    // get result
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_adc_data(adc_channel_t *adc)
    {
        return adc->control_port.value;
    }

    // set bias
    GMP_STATIC_INLINE
    void ctl_set_adc_channel_bias(adc_channel_t *adc, ctrl_gt bias)
    {
        adc->bias = bias;
    }

    // get bias
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_adc_channel_bias(adc_channel_t *adc)
    {
        return adc->bias;
    }

    // get control port
    GMP_STATIC_INLINE
    adc_ift *ctl_get_adc_channel_ctrl_port(adc_channel_t *adc)
    {
        return &adc->control_port;
    }

    //////////////////////////////////////////////////
    // Dual channel ADC

    // This is a dual channel ADC peripheral interface
    // generally, user may use it to sample two channel analog input.
    // or use it as a differential pair.
    typedef struct _tag_adc_dual_channel_t
    {
        // OUTPUT the ADC data in ctrl_gt type
        // Formal expression: value = gain * (raw - gain);
        dual_adc_ift control_port;

        // INPUT raw data from adc
        adc_gt raw[2];

        // resolution of ADC bits
        fast_gt resolution;
        // The result of ADC is IQn number
        fast_gt iqn;

        // The bias of the ADC data
        ctrl_gt bias[2];
        // The gain of the ADC data
        // Negative gain is permitted
        ctrl_gt gain[2];

        //// OUTPUT the ADC data in ctrl_gt type
        //// Formal expression: value = gain * (raw - gain);
        // ctrl_gt value[2];
    } adc_dual_channel_t, dual_adc_channel_t;

    // ec_gt ctl_init_adc_dual_channel(adc_dual_channel_t *adc);

    // ec_gt ctl_setup_adc_dual_channel(adc_dual_channel_t *adc, ctrl_gt gain, ctrl_gt bias, fast_gt resolution,
    //                                  fast_gt iqn);

    void ctl_init_dual_adc_channel(dual_adc_channel_t *adc, ctrl_gt gain, ctrl_gt bias, fast_gt resolution,
                                   fast_gt iqn);

    GMP_STATIC_INLINE
    void ctl_step_dual_adc_channel(dual_adc_channel_t *adc_obj, adc_gt raw1, adc_gt raw2)
    {
        ctrl_gt raw_data;
        ctrl_gt raw_without_bias;

        adc_obj->raw[0] = raw1;
        adc_obj->raw[1] = raw2;

        int i = 0;

        for (i = 0; i < 2; ++i)
        {
#if defined CTRL_GT_IS_FIXED
            // transfer ADC data to _IQ24
            raw_data = adc_obj->raw[i] << (adc_obj->iqn - adc_obj->resolution);
#elif defined CTRL_GT_IS_FLOAT
        // for float point number, transfer ADC data to p.u.
        raw_data = (ctrl_gt)adc_obj->raw[i] / (1 << adc_obj->resolution);
#else
#error("The system does not specify ctrl_gt is float or fixed. You should define CTRL_GT_IS_FLOAT or CTRL_GT_IS_FIXED.")
#endif // CTRL_GT_IS_XXX

            // remove bias
            raw_without_bias = raw_data - adc_obj->bias[i];

            // Gain
            adc_obj->control_port.value.dat[i] = ctl_mul(raw_without_bias, adc_obj->gain[i]);
        }
    }

    GMP_STATIC_INLINE
    void ctl_get_dual_adc_channel(dual_adc_channel_t *adc, ctrl_gt *dat1, ctrl_gt *dat2)
    {
        *dat1 = adc->control_port.value.dat[0];
        *dat2 = adc->control_port.value.dat[1];
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_dual_adc_channel1(dual_adc_channel_t *adc)
    {
        return adc->control_port.value.dat[0];
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_dual_adc_channel2(dual_adc_channel_t *adc)
    {
        return adc->control_port.value.dat[1];
    }

    // positive - negative
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_dual_adc_channel_diff(dual_adc_channel_t *adc)
    {
        return adc->control_port.value.dat[0] - adc->control_port.value.dat[1];
    }

    // (positive + negative) / 2
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_dual_adc_channel_comm(dual_adc_channel_t *adc)
    {
        return ctl_div2(adc->control_port.value.dat[0] + adc->control_port.value.dat[1]);
    }

    GMP_STATIC_INLINE
    void ctl_get_dual_adc_channel_via_vector2(dual_adc_channel_t *adc, ctl_vector2_t *vector)
    {
        vector->dat[0] = adc->control_port.value.dat[0];
        vector->dat[1] = adc->control_port.value.dat[1];
    }

    // get control port
    GMP_STATIC_INLINE
    dual_adc_ift *ctl_get_dual_adc_channel_ctrl_port(dual_adc_channel_t *adc)
    {
        return &adc->control_port;
    }

    //////////////////////////////////////////////////////////////////////////
    // ADC tri channel

    // This module is for three-phase system
    // Such as motor controller and power grid application
    typedef struct _tag_tri_adc_channel_t
    {
        // OUTPUT the ADC data in ctrl_gt type
        // Formal expression: value = gain * (raw - gain);
        tri_adc_ift control_port;

        // INPUT raw data from adc
        adc_gt raw[3];

        // resolution of ADC bits
        fast_gt resolution;
        // The result of ADC is IQn number
        fast_gt iqn;

        // The bias of the ADC data
        ctrl_gt bias[3];
        // The gain of the ADC data
        // Negative gain is permitted
        ctrl_gt gain[3];

    } tri_adc_channel_t;

    // ec_gt ctl_init_adc_tri_channel(adc_tri_channel_t *adc);

    // ec_gt ctl_setup_adc_tri_channel(adc_tri_channel_t *adc, ctrl_gt gain, ctrl_gt bias, fast_gt resolution,
    //                                 fast_gt iqn);

    void ctl_init_tri_adc_channel(tri_adc_channel_t *adc, ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn);

    GMP_STATIC_INLINE
    void ctl_step_adc_tri_channel(tri_adc_channel_t *adc_obj, adc_gt raw1, adc_gt raw2, adc_gt raw3)
    {
        ctrl_gt raw_data;
        ctrl_gt raw_without_bias;

        adc_obj->raw[0] = raw1;
        adc_obj->raw[1] = raw2;
        adc_obj->raw[2] = raw3;

        int i = 0;

        for (i = 0; i < 3; ++i)
        {

#if defined CTRL_GT_IS_FIXED
            // transfer ADC data to _IQ24
            raw_data = adc_obj->raw[i] << (adc_obj->iqn - adc_obj->resolution);
#elif defined CTRL_GT_IS_FLOAT
        // for float point number, transfer ADC data to p.u.
        raw_data = (ctrl_gt)adc_obj->raw[i] / (1 << adc_obj->resolution);
#else
#error("The system does not specify ctrl_gt is float or fixed. You should define CTRL_GT_IS_FLOAT or CTRL_GT_IS_FIXED.")
#endif // CTRL_GT_IS_XXX

            // remove bias
            raw_without_bias = raw_data - adc_obj->bias[i];

            // Gain
            adc_obj->control_port.value.dat[i] = ctl_mul(raw_without_bias, adc_obj->gain[i]);
        }

        // // NOTE: test if extend code is great choice, result no.

        // // Channel 0
        //         #if defined CTRL_GT_IS_FIXED
        //             // transfer ADC data to _IQ24
        //             raw_data = adc_obj->raw[0] << (adc_obj->iqn - adc_obj->resolution);
        // #elif defined CTRL_GT_IS_FLOAT
        //         // for float point number, transfer ADC data to p.u.
        //         raw_data = (ctrl_gt)adc_obj->raw[0] / (1 << adc_obj->resolution);
        // #endif // CTRL_GT_IS_XXX

        //             // remove bias
        //             raw_without_bias = raw_data - adc_obj->bias[0];

        //             // Gain
        //             adc_obj->value[0] = ctl_mul(raw_without_bias, adc_obj->gain[0]);

        // // Channel 1
        //             #if defined CTRL_GT_IS_FIXED
        //             // transfer ADC data to _IQ24
        //             raw_data = adc_obj->raw[1] << (adc_obj->iqn - adc_obj->resolution);
        // #elif defined CTRL_GT_IS_FLOAT
        //         // for float point number, transfer ADC data to p.u.
        //         raw_data = (ctrl_gt)adc_obj->raw[1] / (1 << adc_obj->resolution);
        // #endif // CTRL_GT_IS_XXX

        //             // remove bias
        //             raw_without_bias = raw_data - adc_obj->bias[1];

        //             // Gain
        //             adc_obj->value[1] = ctl_mul(raw_without_bias, adc_obj->gain[1]);

        // // Channel 2
        //             #if defined CTRL_GT_IS_FIXED
        //             // transfer ADC data to _IQ24
        //             raw_data = adc_obj->raw[2] << (adc_obj->iqn - adc_obj->resolution);
        // #elif defined CTRL_GT_IS_FLOAT
        //         // for float point number, transfer ADC data to p.u.
        //         raw_data = (ctrl_gt)adc_obj->raw[2] / (1 << adc_obj->resolution);
        // #endif // CTRL_GT_IS_XXX

        //             // remove bias
        //             raw_without_bias = raw_data - adc_obj->bias[2];

        //             // Gain
        //             adc_obj->value[2] = ctl_mul(raw_without_bias, adc_obj->gain[2]);

        return;
    }

    GMP_STATIC_INLINE
    void ctl_get_tri_adc_channel_via_vector3(tri_adc_channel_t *adc, ctl_vector3_t *vec)
    {
        vec->dat[0] = adc->control_port.value.dat[0];
        vec->dat[1] = adc->control_port.value.dat[1];
        vec->dat[2] = adc->control_port.value.dat[2];
    }

    GMP_STATIC_INLINE
    void ctl_set_tri_adc_channel_bias(tri_adc_channel_t *adc, fast_gt index, ctrl_gt bias)
    {
        adc->bias[index] = bias;
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_tri_adc_bias(tri_adc_channel_t *adc, fast_gt index)
    {
        return adc->bias[index];
    }

    // get control port
    GMP_STATIC_INLINE
    tri_adc_ift *ctl_get_tri_adc_channel_ctrl_port(tri_adc_channel_t *adc)
    {
        return &adc->control_port;
    }

    //////////////////////////////////////////////////////////////////////////
    // ADC bias calibrator

#include <ctl/component/intrinsic/discrete/filter.h>

    typedef struct _tag_adc_bias_calibrator_t
    {
        // OUTPUT ADC bias data
        // ctrl_gt bias_output;

        // output flag
        // fast_gt flag_output_valid;

        // INPUT ADC RAW data
        ctrl_gt raw;

        // tools filter for RAW data.
        ctl_filter_IIR2_t filter;

        // filter tick
        uint32_t filter_tick;

        // total observation period
        uint32_t filter_period;

        //// current period
        // uint32_t current_period;

        //// start tick
        //// start period == 0 means need a brand new beginning.
        // uint32_t start_period;

        // filter enable
        fast_gt enable_filter;

        // output valid
        fast_gt output_valid;

    } adc_bias_calibrator_t;

    // ec_gt ctl_init_adc_bias_calibrator(adc_bias_calibrator_t *obj);

    // ec_gt ctl_setup_adc_bias_calibrator(adc_bias_calibrator_t *obj, ctl_filter_IIR2_setup_t *filter_parameter);

    void ctl_init_adc_bias_calibrator(adc_bias_calibrator_t *obj, ctl_filter_IIR2_setup_t *filter_parameter);

    void ctl_enable_adc_calibrator(adc_bias_calibrator_t *obj);

    // judge if current ADC calibrate is completed.
    GMP_STATIC_INLINE
    fast_gt ctl_is_adc_calibrator_cmpt(adc_bias_calibrator_t *obj)
    {
        // return ((obj->current_period - obj->start_period) > filter_period) ? 1 : 0;
        return (obj->filter_tick > obj->filter_period) ? 1 : 0;
    }

    // This function should called in Main ISR function
    // When user enable ADC calibrator this function may running.
    // User should judge if calibrate is complete by function @ctl_is_adc_calibrator_cmpt,
    // and wait until adc calibrator is completed. Then call @ctl_get_adc_calibrator_result
    // to get result, and move to next position.
    GMP_STATIC_INLINE
    void ctl_step_adc_calibrator(adc_bias_calibrator_t *obj, ctrl_gt adc_value)
    {
        if (obj->enable_filter)
        {
            ctl_step_filter_iir2(&obj->filter, adc_value);
            obj->filter_tick += 1;

            if (ctl_is_adc_calibrator_cmpt(obj))
            {
                obj->enable_filter = 0;
                obj->output_valid = 1;
            }
        }
    }

    // Get ADC calibrate result
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_adc_calibrator_result(adc_bias_calibrator_t *obj)
    {
        return ctl_get_filter_iir2_output(&obj->filter);
    }

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_ADC_CHANNEL_H_
