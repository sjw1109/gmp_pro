//
// THIS IS A DEMO SOURCE CODE FOR GMP LIBRARY.
//
// User should add all declarations of controller objects in this file.
//
// User should implement the Main ISR of the controller tasks.
//
// User should ensure that all the controller codes here is platform-independent.
//
// WARNING: This file must be kept in the include search path during compilation.
//

#include <ctl/component/motor_control/basic/std_sil_motor_interface.h>

#include <xplt.peripheral.h>

#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// device related functions
// Controller interface
//
//
extern ptr_adc_channel_t uin;
extern ptr_adc_channel_t uout;
extern ptr_adc_channel_t idc;

extern pwm_channel_t pwm_out;


extern ctrl_gt sinv_pwm_pu[2];
extern pwm_channel_t sinv_pwm_out[2];

extern ptr_adc_channel_t sinv_il;
extern ptr_adc_channel_t sinv_uc;

// Input Callback
GMP_STATIC_INLINE
void ctl_input_callback(void)
{
    //// invoke ADC p.u. routine
    ctl_step_ptr_adc_channel(&idc);
    ctl_step_ptr_adc_channel(&uin);
    ctl_step_ptr_adc_channel(&uout);

    ctl_step_ptr_adc_channel(&sinv_il);
    ctl_step_ptr_adc_channel(&sinv_uc);

    ac_input = float2ctrl(simulink_rx_buffer.panel[1]);



    // Get panel input here.
#if (BUILD_LEVEL == 1)

    // ctl_set_pmsm_ctrl_vdq_ff(&pmsm_ctrl, float2ctrl(csp_sl_get_panel_input(0)),
    // float2ctrl(csp_sl_get_panel_input(1)));

#endif // BUILD_LEVEL
}

// Output Callback
GMP_STATIC_INLINE
void ctl_output_callback(void)
{

    simulink_tx_buffer.pwm_cmp[0] = ctl_calc_pwm_channel(&pwm_out, pwm_out_pu);

    simulink_tx_buffer.pwm_cmp[1] = ctl_calc_pwm_channel(&sinv_pwm_out[0], sinv_pwm_pu[0]);
    simulink_tx_buffer.pwm_cmp[2] = ctl_calc_pwm_channel(&sinv_pwm_out[1], sinv_pwm_pu[1]);


    simulink_tx_buffer.enable = 1;

    //simulink_tx_buffer.monitor[0] = idc.control_port.value;
    //simulink_tx_buffer.monitor[1] = uin.control_port.value;
    //simulink_tx_buffer.monitor[2] = uout.control_port.value;

    simulink_tx_buffer.dac[0] = 20;

    simulink_tx_buffer.monitor[0] = spll.phasor.dat[phase_alpha];
    simulink_tx_buffer.monitor[1] = spll.phasor.dat[phase_beta];

    simulink_tx_buffer.monitor[2] = spll.frequency;
    simulink_tx_buffer.monitor[3] = spll.theta;

    simulink_tx_buffer.monitor[4] = ctl_div2(ctl_mul(spll.phasor.dat[phase_alpha], float2ctrl(0.2)) + float2ctrl(1));
    simulink_tx_buffer.monitor[5] = sinv_uc.control_port.value;

    simulink_tx_buffer.monitor[6] = sinv_il.control_port.value;
    simulink_tx_buffer.monitor[7] = ctl_mul(spll.phasor.dat[0], float2ctrl(0.15));



    // simulink_tx_buffer.pwm_cmp[1] = 2500.0 * sin(314.0 / 1000 * gmp_base_get_system_tick());
    // simulink_tx_buffer.pwm_cmp[1] = gmp_base_get_system_tick();
}

// Enable Motor Controller
// Enable Output
GMP_STATIC_INLINE
void ctl_enable_output()
{
    csp_sl_enable_output();
}

// Disable Output
GMP_STATIC_INLINE
void ctl_disable_output()
{
    csp_sl_disable_output();
}


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_INTERFACE_H_
