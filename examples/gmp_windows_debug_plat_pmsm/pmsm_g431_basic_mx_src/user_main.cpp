
//////////////////////////////////////////////////////////////////////////
// invoke headers

// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <core/gmp_core.hpp>
#include <core/util/ds/data_ring_buffer.h>

#include <user_main.hpp>

// extensions

// Controller Template Library
#include <ctl/ctl_core.h>

// include the platform
#include <ctl/suite/pmsm/pmsm_speed_closeloop/pmsm_speed_closeloop.h>

//////////////////////////////////////////////////////////////////////////
// global variables here
uint32_t adc_data[7] = {0};
uint16_t pin_stat = 0;



extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;

extern CORDIC_HandleTypeDef hcordic;

extern DAC_HandleTypeDef hdac1;

extern FDCAN_HandleTypeDef hfdcan1;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart2;

extern PCD_HandleTypeDef hpcd_USB_FS;

extern pmsm_ctl_object_t pmsm;

#define DR_EN_Pin GPIO_PIN_11
#define DR_EN_GPIO_Port GPIOB


//////////////////////////////////////////////////////////////////////////
// initialize routine here
GMP_NO_OPT_PREFIX
void user_init(void)
GMP_NO_OPT_SUFFIX
{
//	dbg_uart.write("Hello World!\r\n",14);
	gmp_dbg_prt("Hello World!\r\n");

	__HAL_ADC_ENABLE_IT(&hadc1, ADC_IT_EOS);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_data, 5);
	
	HAL_TIM_Base_Start(&htim1);
	
	HAL_Delay(1000);
	
	
}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
void user_loop(void)
{

//	uint32_t tmp_tick = __HAL_TIM_GET_COUNTER(&htim16);

//	// Enable Controller state machine
//	controller_state_dispatch((ctl_object_nano_t *)&buck_boost);

//	buck_boost.base.mainloop_CPU_usage_tick = __HAL_TIM_GET_COUNTER(&htim16) - tmp_tick;
//	
//	// non-blocking LED blink
//	if (buck_boost.base.isr_tick % buck_boost.base.ctrl_freq <= (buck_boost.base.ctrl_freq / 2))
//		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
//	else
//		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	

	HAL_TIM_Base_Start(&htim1);
	
	

}


//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here

void controller_output_enable(ctl_object_nano_t* pctl_obj)
{
// ????
//HAL_TIM_Base_Start(&htim1);
	HAL_GPIO_WritePin(DR_EN_GPIO_Port, DR_EN_Pin, GPIO_PIN_RESET);

// ??PWM??
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);

}

void controller_output_disable(ctl_object_nano_t* pctl_obj)
{
// ????
//HAL_TIM_Base_Stop(&htim1);
	
	// Disable output
	HAL_GPIO_WritePin(DR_EN_GPIO_Port, DR_EN_Pin, GPIO_PIN_SET);

// ??PWM??
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0);

}
