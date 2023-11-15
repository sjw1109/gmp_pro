// invoke stm32f4 series standard headers 
#include "stm32f4xx_hal.h"

// invoke standard error code
#include "./service/std/error_code.h"

// invoke global macros
#include "./service/std/global.h"

#ifndef _FILE_STM32L1_GENERAL_H_
#define _FILE_STM32L1_GENERAL_H_

#ifdef __cplusplus
extern "C"
{
#endif


	// Type Selection
#define GMP_IIC_HANDLE_T I2C_HandleTypeDef*
#define GMP_DATA_ELEMENT_T uint8_t
#define GMP_LENGTH_T uint16_t

	// iic port
	inline GMP_LENGTH_T gmp_iic_transmit(GMP_IIC_HANDLE_T hi2c, GMP_LENGTH_T iic_dev_addr, GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len)
	{
		if (HAL_I2C_Master_Transmit(hi2c, iic_dev_addr, data, len, TIMEOUT_CNT) == HAL_OK)
			return len;


	}



#ifdef __cplusplus
}
#endif

#endif