/**
 * @file adc_channel.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-03-21
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// #include <gmp_core.h>
#include <ctl/component/interface/interface_base.h>

// help user to calculate ADC bias and gain
#include <ctl/component/interface/bias_model.h>
#include <ctl/component/interface/gain_model.h>

// brief:
// This file implement a set of ADC post process module.
// These set of module do not need to pass a adc data to the entity,
// but achieve it by pointer.
//

#ifndef _FILE_ADC_PTR_CHANNEL_H_
#define _FILE_ADC_PTR_CHANNEL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// Single ADC Channel

typedef struct _tag_ptr_adc_channel_t
{
    // OUTPUT the ADC data in ctrl_gt type
    // Formal expression: value = gain * (raw - bias);
    adc_ift control_port;

    // INPUT raw data from ADC Peripheral,
    // point to ADC source data.
    adc_gt *raw;

    // resolution of ADC bits
    // 16bit right aligned, 16
    // 12bit right aligned, 12
    // 12bit left aligned, 16
    fast_gt resolution;

#if defined CTRL_GT_IS_FIXED
    // The result of ADC is IQn number
    fast_gt shift_index;
#elif defined CTRL_GT_IS_FLOAT
    // for float model,
    // saving a per unit base value.
    ctrl_gt per_unit_base;
#endif // CTRL_GT_IS_XXX

    // The bias of the ADC data
    ctrl_gt bias;

    // The gain of the ADC data
    // Negative gain is permitted
    ctrl_gt gain;
} ptr_adc_channel_t;

// init object function
void ctl_init_ptr_adc_channel(
    // ptr_adc object
    ptr_adc_channel_t *adc,
    // pointer to ADC raw data
    adc_gt *adc_target,
    // ADC Channel settings.
    // iqn is valid only when ctrl_gt is a fixed point type.
    ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn);

// transfer raw data to final value
// This function must be called every control cycle.
GMP_STATIC_INLINE
ctrl_gt ctl_step_ptr_adc_channel(ptr_adc_channel_t *adc_obj)
{
    // ensure this is valid pointer.
    gmp_base_assert(adc_obj->raw);

    // get raw data
    adc_gt raw = *adc_obj->raw;

#if defined CTRL_GT_IS_FIXED
    // for fixed point number, transfer ADC data to _IQ24
    ctrl_gt raw_data = raw << shift_index;
#elif defined CTRL_GT_IS_FLOAT
    // for float point number, transfer ADC data to p.u.
    ctrl_gt raw_data = (ctrl_gt)raw * adc_obj->per_unit_base;
#else
#error("The system does not specify ctrl_gt is float or fixed. You should define CTRL_GT_IS_FLOAT or CTRL_GT_IS_FIXED.")
#endif // CTRL_GT_IS_XXX

    // remove bias
    ctrl_gt raw_without_bias = raw_data - adc_obj->bias;

    // Gain
    adc_obj->control_port.value = ctl_mul(raw_without_bias, adc_obj->gain);

    return adc_obj->control_port.value;
}

// get result
GMP_STATIC_INLINE
ctrl_gt ctl_get_ptr_adc_data(ptr_adc_channel_t *adc)
{
    return adc->control_port.value;
}

// set bias
GMP_STATIC_INLINE
void ctl_set_ptr_adc_channel_bias(ptr_adc_channel_t *adc, ctrl_gt bias)
{
    adc->bias = bias;
}

// get bias
GMP_STATIC_INLINE
ctrl_gt ctl_get_ptr_adc_channel_bias(ptr_adc_channel_t *adc)
{
    return adc->bias;
}

// get control port
GMP_STATIC_INLINE
adc_ift *ctl_get_ptr_adc_channel_ctrl_port(ptr_adc_channel_t *adc)
{
    return &adc->control_port;
}

//////////////////////////////////////////////////
// Dual channel ADC

// This is a dual channel ADC peripheral interface.
// The raw data is a continuous array access by pinter.
// generally, user may use it to sample two channel analog input.
// or use it as a differential pair.
typedef struct _tag_adc_ptr_dual_channel_t
{
    // OUTPUT the ADC data in ctrl_gt type
    // Formal expression: value = gain * (raw - bias);
    dual_adc_ift control_port;

    // INPUT raw data from adc
    // pointer to a fixed length (2) array
    adc_gt *raw;

    // resolution of ADC bits
    fast_gt resolution;

#if defined CTRL_GT_IS_FIXED
    // The result of ADC is IQn number
    fast_gt shift_index;
#elif defined CTRL_GT_IS_FLOAT
    // for float model,
    // saving a per unit base value.
    ctrl_gt per_unit_base;
#endif // CTRL_GT_IS_XXX

    // The bias of the ADC data
    ctrl_gt bias[2];
    // The gain of the ADC data
    // Negative gain is permitted
    ctrl_gt gain[2];

} dual_ptr_adc_channel_t;

void ctl_init_dual_ptr_adc_channel(
    // ptr_adc object
    dual_ptr_adc_channel_t *adc,
    // pointer to ADC raw data
    adc_gt *adc_target,
    // ADC Channel settings.
    // iqn is valid only when ctrl_gt is a fixed point type.
    ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn);

GMP_STATIC_INLINE
void ctl_step_dual_ptr_adc_channel(dual_ptr_adc_channel_t *adc_obj)
{
    ctrl_gt raw_data;
    ctrl_gt raw_without_bias;

    int i = 0;

    // ensure this is valid pointer.
    gmp_base_assert(adc_obj->raw);

    for (i = 0; i < 2; ++i)
    {
#if defined CTRL_GT_IS_FIXED
        // transfer ADC data to _IQ24
        raw_data = adc_obj->raw[i] << shift_index;
#elif defined CTRL_GT_IS_FLOAT
        // for float point number, transfer ADC data to p.u.
        raw_data = (ctrl_gt)adc_obj->raw[i] * adc_obj->per_unit_base;
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
void ctl_get_dual_ptr_adc_channel(dual_ptr_adc_channel_t *adc, ctrl_gt *dat1, ctrl_gt *dat2)
{
    *dat1 = adc->control_port.value.dat[0];
    *dat2 = adc->control_port.value.dat[1];
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_dual_ptr_adc_channel1(dual_ptr_adc_channel_t *adc)
{
    return adc->control_port.value.dat[0];
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_dual_ptr_adc_channel2(dual_ptr_adc_channel_t *adc)
{
    return adc->control_port.value.dat[1];
}

// positive - negative
GMP_STATIC_INLINE
ctrl_gt ctl_get_dual_ptr_adc_channel_diff(dual_ptr_adc_channel_t *adc)
{
    return adc->control_port.value.dat[0] - adc->control_port.value.dat[1];
}

// (positive + negative) / 2
GMP_STATIC_INLINE
ctrl_gt ctl_get_dual_ptr_adc_channel_comm(dual_ptr_adc_channel_t *adc)
{
    return ctl_div2(adc->control_port.value.dat[0] + adc->control_port.value.dat[1]);
}

GMP_STATIC_INLINE
void ctl_get_dual_ptr_adc_channel_via_vector2(dual_ptr_adc_channel_t *adc, ctl_vector2_t *vector)
{
    vector->dat[0] = adc->control_port.value.dat[0];
    vector->dat[1] = adc->control_port.value.dat[1];
}

// get control port
GMP_STATIC_INLINE
dual_adc_ift *ctl_get_dual_ptr_adc_channel_ctrl_port(dual_ptr_adc_channel_t *adc)
{
    return &adc->control_port;
}

//////////////////////////////////////////////////////////////////////////
// ADC tri channel

// This module is for three-phase system
// Such as motor controller and power grid application
typedef struct _tag_tri_ptr_adc_channel_t
{
    // OUTPUT the ADC data in ctrl_gt type
    // Formal expression: value = gain * (raw - bias);
    tri_adc_ift control_port;

    // INPUT raw data from adc
    // pointer to a fixed length (3) array
    adc_gt *raw;

    // resolution of ADC bits
    fast_gt resolution;

#if defined CTRL_GT_IS_FIXED
    // The result of ADC is IQn number
    fast_gt shift_index;
#elif defined CTRL_GT_IS_FLOAT
    // for float model,
    // saving a per unit base value.
    ctrl_gt per_unit_base;
#endif // CTRL_GT_IS_XXX

    // The bias of the ADC data
    ctrl_gt bias[3];

    // The gain of the ADC data
    // Negative gain is permitted
    ctrl_gt gain[3];

} tri_ptr_adc_channel_t;

void ctl_init_tri_ptr_adc_channel(
    // ptr_adc object
    tri_ptr_adc_channel_t *adc,
    // pointer to ADC raw data
    adc_gt *adc_target,
    // ADC Channel settings.
    // iqn is valid only when ctrl_gt is a fixed point type.
    ctrl_gt gain, ctrl_gt bias, fast_gt resolution, fast_gt iqn);

GMP_STATIC_INLINE
void ctl_step_tri_ptr_adc_channel(tri_ptr_adc_channel_t *adc_obj)
{
    ctrl_gt raw_data;
    ctrl_gt raw_without_bias;

    int i = 0;

    // ensure this is valid pointer.
    gmp_base_assert(adc_obj->raw);

    for (i = 0; i < 3; ++i)
    {

#if defined CTRL_GT_IS_FIXED
        // transfer ADC data to _IQ24
        raw_data = adc_obj->raw[i] << adc_obj->shift_index;
#elif defined CTRL_GT_IS_FLOAT
        // for float point number, transfer ADC data to p.u.
        raw_data = (ctrl_gt)adc_obj->raw[i] * adc_obj->per_unit_base;
#else
#error("The system does not specify ctrl_gt is float or fixed. You should define CTRL_GT_IS_FLOAT or CTRL_GT_IS_FIXED.")
#endif // CTRL_GT_IS_XXX

        // remove bias
        raw_without_bias = raw_data - adc_obj->bias[i];

        // Gain
        adc_obj->control_port.value.dat[i] = ctl_mul(raw_without_bias, adc_obj->gain[i]);
    }

    return;
}

GMP_STATIC_INLINE
void ctl_get_tri_ptr_adc_channel_via_vector3(tri_ptr_adc_channel_t *adc, ctl_vector3_t *vec)
{
    vec->dat[0] = adc->control_port.value.dat[0];
    vec->dat[1] = adc->control_port.value.dat[1];
    vec->dat[2] = adc->control_port.value.dat[2];
}

GMP_STATIC_INLINE
void ctl_set_tri_ptr_adc_channel_bias(tri_ptr_adc_channel_t *adc, fast_gt index, ctrl_gt bias)
{
    gmp_base_assert(index < 3);
    adc->bias[index] = bias;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_tri_ptr_adc_bias(tri_ptr_adc_channel_t *adc, fast_gt index)
{
    gmp_base_assert(index < 3);
    return adc->bias[index];
}

// get control port
GMP_STATIC_INLINE
tri_adc_ift *ctl_get_tri_ptr_adc_channel_ctrl_port(tri_ptr_adc_channel_t *adc)
{
    return &adc->control_port;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_ADC_PTR_CHANNEL_H_
