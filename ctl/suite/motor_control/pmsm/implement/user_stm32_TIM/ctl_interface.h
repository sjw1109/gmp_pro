
// Using this header may avoid redefine controller variables.
#include <ctl_main.h>

// This file is platform related

#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

// Internal 2.5V voltage reference
#define ADC_REFERENCE ((3.3))

// Controller Frequency
#define CONTROLLER_FREQUENCY ((10000))

// PWM depth
#define GONTROLLER_PWM_CMP_MAX ((8500))

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // position encoder object
    extern ctl_pos_encoder_t pos_enc;

    // ADC Sample objects
    enum ADC1_ITEMS
    {
        MOTOR_IB = 0,
        MOTOR_UC,
        MOTOR_UB,
        ADC1_SEQ_SIZE
    };

    extern uint32_t adc1_res[ADC1_SEQ_SIZE];

    // ADC2 Sample objects
    enum ADC2_ITEMS
    {
        MOTOR_IA = 0,
        MOTOR_IC,
        MOTOR_UA,
        MOTOR_UDC,
        ADC2_SEQ_SIZE
    };

    extern uint32_t adc2_res[ADC2_SEQ_SIZE];

    // Peripheral Objects
    extern TIM_HandleTypeDef htim1;
    extern ADC_HandleTypeDef hadc1;
    extern ADC_HandleTypeDef hadc2;
    extern SPI_HandleTypeDef hspi2;

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

#ifdef __cplusplus
}
#endif // _cplusplus

#endif //_FILE_CTL_INTERFACE_H_
