// This is an example of peripheral.c

// GMP basic core header
#include <gmp_core.h>
#include <ctl/ctl.config.h>

// user main header
#include "user_main.h"


//#define   MATH_TYPE      IQ_MATH
//// invoke iqmath lib
//#include <third_party/iqmath/IQmathLib.h>

uint32_t adc1_res[ADC1_SEQ_SIZE] = { 0 };

uint32_t adc2_res[ADC2_SEQ_SIZE] = { 0 };


tri_ptr_adc_channel_t uabc;
tri_ptr_adc_channel_t iabc;

ptr_adc_channel_t udc;
ptr_adc_channel_t idc;

pos_autoturn_encoder_t pos_enc;

pwm_tri_channel_t pwm_out;

uint32_t idc_placeholder;

uint32_t iabc_reform[3];
uint32_t uabc_reform[3];

// encoder result
uint32_t encoder_result;


////////////////////////////////////////////////////////////////////////////
//// Devices on the peripheral

// User should setup all the peripheral in this function.
// This function has been completed by syscfg
void setup_peripheral(void)
{

		idc_placeholder = 0;
	

    HAL_ADC_Start_DMA(&hadc1, adc1_res, ADC1_SEQ_SIZE);
		HAL_ADC_Start_DMA(&hadc2, adc2_res, ADC2_SEQ_SIZE);
	
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

		ctl_init_ptr_adc_channel(
        // bind idc channel with idc address
        &idc, &idc_placeholder,
        // ADC gain, ADC bias
        float2ctrl(MTR_CTRL_CURRENT_GAIN), float2ctrl(MTR_CTRL_CURRENT_BIAS),
        // ADC resolution, IQN
        12, 24);

    ctl_init_tri_ptr_adc_channel(
        // bind ibac channel with iabc address
        &iabc, iabc_reform,
        // ADC gain, ADC bias
        float2ctrl(MTR_CTRL_CURRENT_GAIN), float2ctrl(MTR_CTRL_CURRENT_BIAS),
        // ADC resolution, IQN
        12, 24);

    ctl_init_ptr_adc_channel(
        // bind udc channel with udc address
        &udc, &adc2_res[MOTOR_UDC],
        // ADC gain, ADC bias
        float2ctrl(MTR_CTRL_VOLTAGE_GAIN), float2ctrl(MTR_CTRL_VOLTAGE_BIAS),
        // ADC resolution, IQN
        12, 24);

    ctl_init_tri_ptr_adc_channel(
        // bind vbac channel with vabc address
        &uabc, uabc_reform,
        // ADC gain, ADC bias
        float2ctrl(MTR_CTRL_VOLTAGE_GAIN), float2ctrl(MTR_CTRL_VOLTAGE_BIAS),
        // ADC resolution, IQN
        12, 24);

    ctl_init_autoturn_pos_encoder(&pos_enc, MOTOR_PARAM_POLE_PAIRS, ((uint32_t)1 << 14) - 1);

    // bind peripheral to motor controller
    ctl_attach_mtr_adc_channels(&pmsm_ctrl.mtr_interface,
                                // phase voltage & phase current
                                &iabc.control_port, &uabc.control_port,
                                // dc bus voltage & dc bus current
                                &idc.control_port, &udc.control_port);

    ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &pos_enc.encif);

    ctl_attach_pmsm_bare_output(&pmsm_ctrl, &pwm_out.raw);

    // output channel
    ctl_init_pwm_tri_channel(&pwm_out, 0, CONTROLLER_PWM_CMP_MAX);

    // communicate with SPI DAC
    //    SPI_writeDataBlockingNonFIFO(SPI0_BASE,
    //    DAC8554_LDCMD_SINGLE_CH_UPDATE|DAC8554_CHANNEL_A|DAC8554_DISABLE_POWERDOWN);
    //    SPI_writeDataBlockingNonFIFO(SPI0_BASE, 0x80);
    //    SPI_writeDataBlockingNonFIFO(SPI0_BASE, 0x00);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc2)
	{
		gmp_base_ctl_step();
	}
}


