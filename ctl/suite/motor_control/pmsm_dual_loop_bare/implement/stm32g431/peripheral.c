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

//
//Task 1 (C) Variables
// NOTE: Do not initialize the Message RAM variables globally, they will be
// reset during the message ram initialization phase in the CLA memory
// configuration routine
//

//#ifdef __cplusplus
//
//#pragma DATA_SECTION("CpuToCla1MsgRAM");
//float fVal;
//
//#pragma DATA_SECTION("Cla1ToCpuMsgRAM");
//float fResult;
//
//#else
//
//#pragma DATA_SECTION(fVal,"CpuToCla1MsgRAM");
//adc_gt adc_data;
//
//#pragma DATA_SECTION(fResult,"Cla1ToCpuMsgRAM");
//float fResult;
//#endif //__cplusplus
//
//
//#ifdef __cplusplus
//#pragma DATA_SECTION("CLADataLS1")
//#else
//#pragma DATA_SECTION(CLAatan2Table,"CLADataLS1")
//#endif //__cplusplus
//float CLAatan2Table[]={
//    0.000000000000, 1.000040679675, -0.007811069750,
//    -0.000003807022, 1.000528067772, -0.023410345493
//};

//__interrupt void cla1Isr1()
//{
//    // output routine
//    ctl_fmif_output_stage_routine(&pmsm_servo.base);

//    // request routine
//    ctl_fmif_request_stage_routine(&pmsm_servo.base);

//    //
//    // Acknowledge the end-of-task interrupt for task 1
//    //
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP11);

//    //
//    // Uncomment to halt debugger and stop here
//    //
////    asm(" ESTOP0");
//}

////////////////////////////////////////////////////////////////////////////
//// Devices on the peripheral

// User should setup all the peripheral in this function.
// This function has been completed by syscfg
void setup_peripheral(void)
{
    // light two LED
//    gmp_hal_gpio_write(LEDR, 0);
//    gmp_hal_gpio_write(LEDG, 1);

    //    fVal = 10;
    //    GMP_DBG_SWBP;

    // Clear Interrupt
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP11);
//
//    CLA_forceTasks(CLA1_BASE, CLA_TASKFLAG_1);

//    debug_uart = debug_uart_BASE;
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


