
//////////////////////////////////////////////////////////////////////////
// invoke headers

// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <core/gmp_core.hpp>
#include <core/util/ds/data_ring_buffer.h>

// extensions
#include <ext/eeprom/at24cxx/at24cxx.h>

//////////////////////////////////////////////////////////////////////////
// global variables here

at24cxx eeprom04(&iic, at24cxx::at24c04, 0);


//////////////////////////////////////////////////////////////////////////
// initialize routine here
void user_init(void)
	GMP_NO_OPT
{
	dbg_uart.write("Hello World!\r\n",14);
	
	eeprom04.write(255, 0x17);
	eeprom04.write(254, 0x18);
	eeprom04.write(253, 0x19);
	uint32_t result = eeprom04.read(253);
	gmp_dbg_prt("result: %x,error code: %d\r\n", result,eeprom04.iic->last_error);
	result = eeprom04.read(254);
	gmp_dbg_prt("result: %x,error code: %d\r\n", result,eeprom04.iic->last_error);

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
	uint32_t result = eeprom04.read(253);

	gmp_dbg_prt("result: %x,error code: %d\r\n", result, eeprom04.iic->last_error);
	HAL_Delay(1000);
}


//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{

//	// Toggle the Green LED
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_SET);

//	// ADC input
//// 	sdspc.adc.Vin = adc_data[1];
//// 	sdspc.adc.Vout = adc_data[2];
//// 	sdspc.adc.Iout = adc_data[3];

//	// call digital switching power controller
////	sdspc.gmp_sdspc_ctrl_g();

////	da.write_dual_channel(((uint16_t)sdspc.adc.Vin)<<4,((uint16_t)sdspc.adc.Vout)<<4);
//	
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15,GPIO_PIN_RESET);

//	
////	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);


//}


