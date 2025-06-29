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

ptr_adc_channel_t uin;
ptr_adc_channel_t uout;
ptr_adc_channel_t idc;

pwm_channel_t pwm_out;
pwm_channel_t sinv_pwm_out[2];

ptr_adc_channel_t sinv_il;
ptr_adc_channel_t sinv_uc;



//////////////////////////////////////////////////////////////////////////
// peripheral setup function
//

// User should setup all the peripheral in this function.
void setup_peripheral(void)
{
    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &idc,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[0],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &uin,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[1],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &uout,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[2],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_il,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[3],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_ptr_adc_channel(
        // ptr_adc object
        &sinv_uc,
        // pointer to ADC raw data
        &simulink_rx_buffer.adc_result[3],
        // ADC Channel settings.
        // iqn is valid only when ctrl_gt is a fixed point type.
        2, 0.5, 12, 24);

    ctl_init_pwm_channel(&pwm_out, 0, 5000);

    ctl_init_pwm_channel(&sinv_pwm_out[0], 0, 5000);
    ctl_init_pwm_channel(&sinv_pwm_out[1], 0, 5000);

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
