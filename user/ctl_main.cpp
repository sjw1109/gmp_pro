#include <core/gmp_core.hpp>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>


// extern variables
extern uint16_t adc_data[6];
extern buck_boost_2ch_ctl_object_t buck_boost;

void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
{
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;
	int i = 0;

	for (i = 0; i < 6; ++i)
	{
		obj->adc_results[i].raw = adc_data[i];
	}

	return;
}



void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
{
	buck_boost_2ch_ctl_object_t* obj = (buck_boost_2ch_ctl_object_t*)pctl_obj;
	HRTIM_CompareCfgTypeDef CompareCfg = { 0 };

	CompareCfg.CompareValue = obj->pwm[0].value;
	HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, &CompareCfg);

	CompareCfg.CompareValue = obj->pwm[1].value;
	HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_1, &CompareCfg);
}


void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
{
	// Noting to do
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8);

	uint32_t tmp_tick = __HAL_TIM_GET_COUNTER(&htim16);

	controller_dispatch((ctl_object_nano_t*)&buck_boost);

	// Calculate CPU usage
	buck_boost.base.control_law_CPU_usage_tick = __HAL_TIM_GET_COUNTER(&htim16) - tmp_tick;
}



void controller_output_enable(ctl_object_nano_t* pctl_obj)
{
	// TA
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1);
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA2);

	// TB
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TB1);
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TB2);
}

void controller_output_disable(ctl_object_nano_t* pctl_obj)
{
	// TA
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA1);
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA2);

	// TB
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TB1);
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TB2);
}

