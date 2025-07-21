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

extern ptr_adc_channel_t sinv_adc_idc;
extern ptr_adc_channel_t sinv_adc_udc;
extern ptr_adc_channel_t sinv_adc_il;
extern ptr_adc_channel_t sinv_adc_igrid;
extern ptr_adc_channel_t sinv_adc_ugrid;

typedef enum _tag_sinv_adc_index_items
{

    SINV_ADC_ID_IL = 0,
    SINV_ADC_ID_IDC = 1,
    SINV_ADC_ID_VDC = 2,
    SINV_ADC_ID_IC = 3,
    SINV_ADC_ID_VG = 4,
    SINV_ADC_ID_IG = 5

} sinv_adc_index_items;

extern pwm_channel_t sinv_pwm_out[2];

extern sinv_ctrl_t sinv_ctrl;

// Input Callback
GMP_STATIC_INLINE
void ctl_input_callback(void)
{
    //// invoke ADC p.u. routine
    ctl_step_ptr_adc_channel(&sinv_adc_idc);
    ctl_step_ptr_adc_channel(&sinv_adc_udc);
    ctl_step_ptr_adc_channel(&sinv_adc_il);
    ctl_step_ptr_adc_channel(&sinv_adc_igrid);
    ctl_step_ptr_adc_channel(&sinv_adc_ugrid);
}

// Output Callback
GMP_STATIC_INLINE
void ctl_output_callback(void)
{

    // simulink_tx_buffer.pwm_cmp[0] = ctl_calc_pwm_channel(&pwm_out, pwm_out_pu);

    simulink_tx_buffer.pwm_cmp[0] = ctl_calc_pwm_channel(&sinv_pwm_out[0], sinv_ctrl.sinv_pwm_pu[0]);
    simulink_tx_buffer.pwm_cmp[1] = ctl_calc_pwm_channel(&sinv_pwm_out[1], sinv_ctrl.sinv_pwm_pu[1]);

    // simulink_tx_buffer.enable = 1;
}

// Enable Motor Controller
// Enable Output
GMP_STATIC_INLINE
void ctl_enable_output()
{
    ctl_enable_sinv_ctrl(&sinv_ctrl);

    csp_sl_enable_output();

    flag_system_running = 1;
}

// Disable Output
GMP_STATIC_INLINE
void ctl_disable_output()
{
    flag_system_running = 0;
    csp_sl_disable_output();
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_INTERFACE_H_
