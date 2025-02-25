
#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

// PWM depth
#define GONTROLLER_PWM_CMP_MAX ((0x84D0))

// This file is platform related
#define CONTROLLER_FREQUENCY ((10000))

// Internal 2.5V voltage reference
#define ADC_REFERENCE ((3.3))


#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Input routine
// Input ADCs and other necessary messages.
GMP_STATIC_INLINE
void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
{
    pmsm_servo_fm_t* pmsm = (pmsm_servo_fm_t*) pctl_obj;

//    ctl_input_pmsm_servo_framework(pmsm,
//          ADC_readResult(ADC_PHASE_U_RESULT_BASE, ADC_IU),
//          ADC_readResult(ADC_PHASE_V_BASE, ADC_IV),
//          ADC_readResult(ADC_PHASE_W_BASE, ADC_IW));

//    ctl_step_adc_tri_channel(&pmsm->vabc_input,
//                             ADC_readResult(ADC_PHASE_U_RESULT_BASE, ADC_UU),
//                             ADC_readResult(ADC_PHASE_V_RESULT_BASE, ADC_UV),
//                             ADC_readResult(ADC_PHASE_W_RESULT_BASE, ADC_UW));

//    ctl_step_adc_channel(&pmsm->udc_input, ADC_readResult(ADC_PHASE_U_RESULT_BASE, ADC_UDC));


}


// output all the PWM and other digital or analog signal
GMP_STATIC_INLINE
void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
{
    pmsm_servo_fm_t* pmsm = (pmsm_servo_fm_t*) pctl_obj;

//    ctl_vector3_t Tabc;
//
//    ctl_ct_svpwm_calc(&pmsm->current_ctrl.iab0, &Tabc);

//    EPWM_setCounterCompareValue(EPWMU_BASE, EPWM_COUNTER_COMPARE_A,
//                                ctl_get_pmsm_servo_modulation(pmsm, phase_A));
//    EPWM_setCounterCompareValue(EPWMV_BASE, EPWM_COUNTER_COMPARE_A,
//                                ctl_get_pmsm_servo_modulation(pmsm, phase_B));
//    EPWM_setCounterCompareValue(EPWMW_BASE, EPWM_COUNTER_COMPARE_A,
//                                ctl_get_pmsm_servo_modulation(pmsm, phase_C));


}


// request other information via peripheral, for instance SPI.
GMP_STATIC_INLINE
void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
{
// SPI Encoder
// CS black
// clk blue
// MISO green
// MOSI yellow
// GND white
// +5V red

}

GMP_STATIC_INLINE
void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
{
//    gmp_hal_gpio_write(MTR_ENABLE, 0);
}

GMP_STATIC_INLINE
void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
{
//    gmp_hal_gpio_write(MTR_ENABLE, 1);
}




#ifdef __cplusplus
}
#endif // _cplusplus

#endif //_FILE_CTL_INTERFACE_H_
