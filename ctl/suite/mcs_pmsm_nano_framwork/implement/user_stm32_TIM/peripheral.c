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

    HAL_ADC_Start_DMA(&hadc1, adc1_res, ADC1_SEQ_SIZE);
		HAL_ADC_Start_DMA(&hadc2, adc2_res, ADC2_SEQ_SIZE);
	
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
		HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

    //
    // Enable ePWM interrupts
    //
//    Interrupt_enable(INT_EPWM1);
//    Interrupt_enable(INT_EPWM2);
//    Interrupt_enable(INT_EPWM3);

    // Start up ADC A & B & C
//    Interrupt_enable(INT_ADCA1);
//    Interrupt_enable(INT_ADCB1);
//    Interrupt_enable(INT_ADCC1);

    //    fVal = fResult;

    // communicate with SPI DAC
    //    SPI_writeDataBlockingNonFIFO(SPI0_BASE,
    //    DAC8554_LDCMD_SINGLE_CH_UPDATE|DAC8554_CHANNEL_A|DAC8554_DISABLE_POWERDOWN);
    //    SPI_writeDataBlockingNonFIFO(SPI0_BASE, 0x80);
    //    SPI_writeDataBlockingNonFIFO(SPI0_BASE, 0x00);
}


////////////////////////////////////////////////////////////////////////////
//// device related functions
//#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
//


fast_gt ctl_fmif_security_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return GMP_EC_OK;
}
//
//
//#endif
//
//
////////////////////////////////////////////////////////////////////////////
//// interrupt functions and callback functions here
//
//// This function should be called in Main ISR
//void gmp_base_ctl_step(void);
//

//interrupt void MainISR(void)
//{

//    //
//    // call GMP ISR  Controller operation callback function
//    //
//    gmp_base_ctl_step();

//    //
//    // Clear the interrupt flag
//    //
//    ADC_clearInterruptStatus(ADC_PHASE_U_BASE, ADC_INT_NUMBER1);

//    //
//    // Check if overflow has occurred
//    //
//    if(true == ADC_getInterruptOverflowStatus(ADC_PHASE_U_BASE, ADC_INT_NUMBER1))
//    {
//        ADC_clearInterruptOverflowStatus(ADC_PHASE_U_BASE, ADC_INT_NUMBER1);
//        ADC_clearInterruptStatus(ADC_PHASE_U_BASE, ADC_INT_NUMBER1);
//    }

//    //
//    // Acknowledge the interrupt
//    //
//    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);

//}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc2)
	{
		gmp_base_ctl_step();
	}
}


