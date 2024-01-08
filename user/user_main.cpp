
//////////////////////////////////////////////////////////////////////////
// invoke headers

// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <core/gmp_core.hpp>

#include <core/util/ds/data_ring_buffer.h>



//////////////////////////////////////////////////////////////////////////
// global variables here




//////////////////////////////////////////////////////////////////////////
// initialize routine here
void user_init(void)
{



}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
void user_loop(void)
{


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


