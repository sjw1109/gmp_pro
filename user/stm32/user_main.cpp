
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


//////////////////////////////////////////////////////////////////////////
// global variables here



//////////////////////////////////////////////////////////////////////////
// initialize routine here
void user_init(void)
	GMP_NO_OPT
{
	dbg_uart.write("Hello World!\r\n",14);
	

	pwm_ch1.enable();

	HAL_Delay(2000);

	for (int i = 0; i < 32; ++i)
	{
		pwm_ch1.set_modulator_cmp(100*i);
		HAL_Delay(1000);
	}

	//pwm_ch1.disable();
}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
void user_loop(void)
{
	HAL_Delay(1000);
}


//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here


