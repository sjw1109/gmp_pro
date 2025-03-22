
// interface basic type
#include <ctl/math_block/vector_lite/vector2.h>
#include <ctl/math_block/vector_lite/vector3.h>
#include <ctl/math_block/vector_lite/vector4.h>

#ifndef _FILE_CTL_INTERFACE_BASE_H_
#define _FILE_CTL_INTERFACE_BASE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // Controller should call functions in this file to operate the control peripheral.

    //////////////////////////////////////////////////////////////////////////
    // ADC, DAC, PWM universal Channel object
    // User may invoke these name directly to implement a controller.
    //

    typedef struct _tag_channel_base_t
    {
        // for ADC: output result (after scaled, and biased)
        // for DAC: output analog (per unit value)
        // for PWM: compare target (per unit value)
        // for CAP: capture duty (per unit value)
        ctrl_gt value;
    } adc_ift, dac_ift, pwm_ift, cap_ift;

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_adc_result(adc_ift *adc)
    {
        return adc->value;
    }

    GMP_STATIC_INLINE
    void ctl_set_pwm_result(pwm_ift *pwm, ctrl_gt target)
    {
        pwm->value = target;
    }

    GMP_STATIC_INLINE
    void ctl_set_dac_result(dac_ift *dac, ctrl_gt target)
    {
        dac->value = target;
    }

    typedef struct _tag_dual_channel_base_t
    {
        // for ADC: output result (after scaled, and biased)
        // for DAC: output analog (per unit value)
        // for PWM: compare target (per unit value)
        vector2_gt value;
    } dual_adc_ift, dual_dac_ift, dual_pwm_ift;

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_adc_diff_result(dual_adc_ift *adc)
    {
        return adc->value.dat[0] - adc->value.dat[1];
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_get_dual_adc_result_vec(dual_adc_ift *adc, vector2_gt *vec)
    {
        ctl_vector2_copy(vec, &adc->value);
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_set_dual_pwm(dual_pwm_ift *pwm, ctrl_gt phase1, ctrl_gt phase2)
    {
        pwm->value.dat[0] = phase1;
        pwm->value.dat[1] = phase2;
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_set_dual_pwm_vec(dual_pwm_ift *pwm, vector2_gt *vec)
    {
        ctl_vector2_copy(&pwm->value, vec);
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_set_dual_dac(dual_pwm_ift *pwm, ctrl_gt phase1, ctrl_gt phase2)
    {
        pwm->value.dat[0] = phase1;
        pwm->value.dat[1] = phase2;
    }

    typedef struct _tag_tri_channel_base_t
    {
        // for ADC: output result (after scaled, and biased)
        // for DAC: output analog (per unit value)
        // for PWM: compare target (per unit value)
        vector3_gt value;
    } tri_adc_ift, tri_dac_ift, tri_pwm_ift;

    typedef struct _tag_quad_channel_base_t
    {
        // for ADC: output result (after scaled, and biased)
        // for DAC: output analog (per unit value)
        // for PWM: compare target (per unit value)
        vector4_gt value;
    } quad_adc_ift, quad_dac_ift, quad_pwm_ift;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_INTERFACE_BASE_H_
