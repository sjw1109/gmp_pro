/**
 * @file user_main.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */


//////////////////////////////////////////////////////////////////////////
// invoke headers

// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <core/gmp_core.hpp>
#include <core/util/ds/data_ring_buffer.h>

// extensions

// Controller Template Library
#include <ctl/ctl_core.h>

// include the platform
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

//////////////////////////////////////////////////////////////////////////
// global variables here
uint16_t adc_data[6] = {0};
uint16_t pin_stat = 0;

buck_boost_2ch_ctl_object_t buck_boost;

//////////////////////////////////////////////////////////////////////////
// initialize routine here
GMP_NO_OPT_PREFIX
void user_init(void)
GMP_NO_OPT_SUFFIX
{
//	dbg_uart.write("Hello World!\r\n",14);
	gmp_dbg_prt("Hello World!\r\n");

	init_buck_boost_2ch_ctl(&buck_boost);


	// If the Controller cannot fit inspection
	if (controller_inspection((ctl_object_nano_t *)&buck_boost))
		while (1);


	//pwm_ch1.disable();
}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
void user_loop(void)
{

	uint32_t tmp_tick = __HAL_TIM_GET_COUNTER(&htim16);

	// Enable Controller state machine
	controller_state_dispatch((ctl_object_nano_t *)&buck_boost);

	buck_boost.base.mainloop_CPU_usage_tick = __HAL_TIM_GET_COUNTER(&htim16) - tmp_tick;
	
	// non-blocking LED blink
	if (buck_boost.base.isr_tick % buck_boost.base.ctrl_freq <= (buck_boost.base.ctrl_freq / 2))
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

}


//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here


