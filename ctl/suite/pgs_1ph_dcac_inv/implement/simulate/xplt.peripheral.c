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
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_udc,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_VDC],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_il,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_IL],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_igrid,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_IG],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_adc_ugrid,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[SINV_ADC_ID_VG],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    //
    // output channel
    //
    ctl_init_pwm_channel(&sinv_pwm_out[0], 0, 5000);
    ctl_init_pwm_channel(&sinv_pwm_out[1], 0, 5000);

    // attach 
    ctl_attach_sinv_with_adc(&sinv_ctrl, &sinv_adc_udc.control_port, &sinv_adc_idc.control_port,
                             &sinv_adc_il.control_port, &sinv_adc_ugrid.control_port, &sinv_adc_igrid.control_port);

    //// bind peripheral to motor controller
    // ctl_attach_mtr_adc_channels(&pmsm_ctrl.mtr_interface,
    //                             // phase voltage & phase current
    //                             &iabc.control_port, &uabc.control_port,
    //                             // dc bus voltage & dc bus current
    //                             &idc.control_port, &udc.control_port);

    // ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &pos_enc.encif);

    // ctl_attach_pmsm_bare_output(&pmsm_ctrl, &pwm_out.raw);

    //// output channel
    // ctl_init_pwm_tri_channel(&pwm_out, 0, CONTROLLER_PWM_CMP_MAX);

    // open hardware switch
    // ctl_output_enable();
}
