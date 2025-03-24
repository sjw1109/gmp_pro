
// Using this header may avoid redefine controller variables.
#include <ctl_main.h>

// This file is platform related

#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_



#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


		
		#ifndef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
		
		
		 GMP_STATIC_INLINE
    void ctl_input_callback(void)
    {
				iabc_reform[phase_A] = adc2_res[MOTOR_IA];
				iabc_reform[phase_B] = adc1_res[MOTOR_IB];
				iabc_reform[phase_C] = adc2_res[MOTOR_IC];
		
				uabc_reform[phase_A] = adc2_res[MOTOR_UA];
				uabc_reform[phase_B] = adc1_res[MOTOR_UB];
				uabc_reform[phase_C] = adc1_res[MOTOR_UC];
		
				// call encoder for position information
		
				// write to encoder result: encoder_result


		
        // invoke ADC p.u. routine
        ctl_step_tri_ptr_adc_channel(&iabc);
        ctl_step_tri_ptr_adc_channel(&uabc);
        ctl_step_ptr_adc_channel(&idc);
        ctl_step_ptr_adc_channel(&udc);
				

        // invoke position encoder routine.
        ctl_step_autoturn_pos_encoder(&pos_enc,encoder_result);
    }


    extern ctrl_gt param_test;
    GMP_STATIC_INLINE
    void ctl_output_callback(void)
    {
				// modulation
        ctl_calc_pwm_tri_channel(&pwm_out);
				
				// write to conpare
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pwm_out.value[phase_A]);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, pwm_out.value[phase_B]);
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, pwm_out.value[phase_C]);

    }
		
		#else // enable CTL controller nano framework
		

    // Input routine
    // Input ADCs and other necessary messages.
    GMP_STATIC_INLINE
    void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
    {
        pmsm_fm_t *pmsm = (pmsm_fm_t *)pctl_obj;

        ctl_input_pmsm_framework(pmsm, adc2_res[MOTOR_IA], adc1_res[MOTOR_IB], adc2_res[MOTOR_IC]);

        ctl_step_adc_tri_channel(&pmsm->vabc_input, adc2_res[MOTOR_UA], adc1_res[MOTOR_UB], adc1_res[MOTOR_UC]);

        ctl_step_adc_channel(&pmsm->udc_input, adc2_res[MOTOR_UDC]);
    }

    // output all the PWM and other digital or analog signal
    GMP_STATIC_INLINE
    void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
    {
        pmsm_fm_t *pmsm = (pmsm_fm_t *)pctl_obj;

        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ctl_get_pmsm_modulation(pmsm, phase_A));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ctl_get_pmsm_modulation(pmsm, phase_B));
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, ctl_get_pmsm_modulation(pmsm, phase_C));
    }

    // request other information via peripheral, for instance SPI.
    GMP_STATIC_INLINE
    void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
    {
        uint8_t enc_req[2] = {0xFF, 0xFF};
        uint16_t enc_res = 0;
        HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
        HAL_SPI_TransmitReceive(&hspi2, enc_req, (uint8_t *)&enc_res, 2, 10);
        HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
        ctl_step_pos_encoder(&pos_enc, 0x3FFF - (gmp_l2b16(enc_res) & 0x3FFF));
    }

    GMP_STATIC_INLINE
    void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
    {
        //    gmp_hal_gpio_write(MTR_ENABLE, 0);
        HAL_GPIO_WritePin(PWM_EN_GPIO_Port, PWM_EN_Pin, GPIO_PIN_RESET);
    }

    GMP_STATIC_INLINE
    void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
    {
        //    gmp_hal_gpio_write(MTR_ENABLE, 1);
        HAL_GPIO_WritePin(PWM_EN_GPIO_Port, PWM_EN_Pin, GPIO_PIN_SET);
    }
		
		#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifdef __cplusplus
}
#endif // _cplusplus

#endif //_FILE_CTL_INTERFACE_H_
