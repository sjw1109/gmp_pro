
#ifndef _FILE_PERIPHERAL_H_
#define _FILE_PERIPHERAL_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include <gmp_core.h>

// select ADC PTR interface
#include <ctl/component/interface/adc_ptr_channel.h>

// select default encoder
#include <ctl/component/motor_control/basic/encoder.h>

// select SIL package
#include <ctl/component/motor_control/basic/motor_universal_interface.h>

#include <ctl/component/motor_control/basic/std_sil_motor_interface.h>

#include <ctl/component/interface/adc_channel.h>

    // buffer for rx & tx
    //    extern gmp_pc_simulink_rx_buffer_t simulink_rx_buffer;
    //    extern gmp_pc_simulink_tx_buffer_t simulink_tx_buffer;

    extern tri_ptr_adc_channel_t uabc;
    extern tri_ptr_adc_channel_t iabc;

    extern ptr_adc_channel_t udc;
    extern ptr_adc_channel_t idc;

    extern pos_autoturn_encoder_t pos_enc;

    extern pwm_tri_channel_t pwm_out;

    // position encoder object
    extern pos_autoturn_encoder_t pos_enc;

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

    // ADC Sample result will copy to these array.
    extern uint32_t idc_placeholder;
    extern uint32_t iabc_reform[3];
    extern uint32_t uabc_reform[3];

    // Peripheral Objects
    extern TIM_HandleTypeDef htim1;
    extern ADC_HandleTypeDef hadc1;
    extern ADC_HandleTypeDef hadc2;
    extern SPI_HandleTypeDef hspi2;

    // encoder result
    extern uint32_t encoder_result;
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PERIPHERAL_H_
