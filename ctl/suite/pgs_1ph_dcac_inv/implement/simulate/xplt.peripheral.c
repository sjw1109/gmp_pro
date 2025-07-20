//
// THIS IS A DEMO SOURCE CODE FOR GMP LIBRARY.
//
// User should add all definitions of peripheral objects in this file.
//
// User should implement the peripheral objects initialization in setup_peripheral function.
//
// This file is platform-related.
//

// GMP basic core header
#include <gmp_core.h>

// user main header
#include "user_main.h"

#include <xplt.peripheral.h>

//////////////////////////////////////////////////////////////////////////
// definitions of peripheral
//

ptr_adc_channel_t sinv_adc_idc;
ptr_adc_channel_t sinv_adc_udc;
ptr_adc_channel_t sinv_adc_il;
ptr_adc_channel_t sinv_adc_igrid;
ptr_adc_channel_t sinv_adc_ugrid;

pwm_channel_t sinv_pwm_out[2];

//////////////////////////////////////////////////////////////////////////
// peripheral setup function
//

// User should setup all the peripheral in this function.
void setup_peripheral(void)
{
    //
    // input channel
    //

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_idc,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_IDC],
        // ADC Channel settings.
        ctl_gain_calc_via_gain(CTRL_ADC_VOLTAGE_REF, CTRL_CURRENT_ADC_GAIN, CTRL_CURRENT_BASE),
        ctl_bias_calc_via_Vref_Vbias(CTRL_ADC_VOLTAGE_REF, CTRL_CURRENT_ADC_BIAS),
        // iqn is valid only when ctrl_gt is a fixed point type.
        CTRL_ADC_RESOLUTION, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_udc,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_VDC],
        // ADC Channel settings.
        ctl_gain_calc_via_gain(CTRL_ADC_VOLTAGE_REF, CTRL_VOLTAGE_ADC_GAIN, CTRL_VOLTAGE_BASE),
        ctl_bias_calc_via_Vref_Vbias(CTRL_ADC_VOLTAGE_REF, CTRL_VOLTAGE_ADC_BIAS),
        // iqn is valid only when ctrl_gt is a fixed point type.
        CTRL_ADC_RESOLUTION, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_il,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_IL],
        // ADC Channel settings.
        ctl_gain_calc_via_gain(CTRL_ADC_VOLTAGE_REF, CTRL_CURRENT_ADC_GAIN, CTRL_CURRENT_BASE),
        ctl_bias_calc_via_Vref_Vbias(CTRL_ADC_VOLTAGE_REF, CTRL_CURRENT_ADC_BIAS),
        // iqn is valid only when ctrl_gt is a fixed point type.
        CTRL_ADC_RESOLUTION, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_igrid,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_IG],
        // ADC Channel settings.
        ctl_gain_calc_via_gain(CTRL_ADC_VOLTAGE_REF, CTRL_CURRENT_ADC_GAIN, CTRL_CURRENT_BASE),
        ctl_bias_calc_via_Vref_Vbias(CTRL_ADC_VOLTAGE_REF, CTRL_CURRENT_ADC_BIAS),
        // iqn is valid only when ctrl_gt is a fixed point type.
        CTRL_ADC_RESOLUTION, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_ugrid,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_VG],
        // ADC Channel settings.
        ctl_gain_calc_via_gain(CTRL_ADC_VOLTAGE_REF, CTRL_VOLTAGE_ADC_GAIN, CTRL_VOLTAGE_BASE),
        ctl_bias_calc_via_Vref_Vbias(CTRL_ADC_VOLTAGE_REF, CTRL_VOLTAGE_ADC_BIAS),
        // iqn is valid only when ctrl_gt is a fixed point type.
        CTRL_ADC_RESOLUTION, 24);

    //
    // output channel
    //
    ctl_init_pwm_channel(&sinv_pwm_out[0], 0, CONTROLLER_PWM_CMP_MAX);
    ctl_init_pwm_channel(&sinv_pwm_out[1], 0, CONTROLLER_PWM_CMP_MAX);

    // attach
    ctl_attach_sinv_with_adc(&sinv_ctrl, &sinv_adc_udc.control_port, &sinv_adc_idc.control_port,
                             &sinv_adc_il.control_port, &sinv_adc_ugrid.control_port, &sinv_adc_igrid.control_port);

    // open hardware switch
    // ctl_output_enable();
}
