
// GMP Core header
#include <core/gmp_core.h>

// System model of STM32
// This file implement all the STM32 system core functions.
//


// ....................................................................//
// CSP start up function
//

// This function should undertake initialization of the chip.
//
void gmp_csp_startup(void)
{

}

// This function will translate STM32 HAL library error code to GMP std error code.
//
gmp_stat_t trans_hal_error_to_gmp_error(HAL_StatusTypeDef stat)
{
	if (stat == HAL_OK)
		return GMP_STAT_OK;
	
	if (stat == HAL_BUSY)
		return GMP_STAT_BUSY;

	if (stat == HAL_ERROR)
		return GMP_STAT_GENERAL_ERROR;

	return GMP_STAT_GENERAL_ERROR;
}



// Prototype of Error handle
// This function is defined in `main.c`
//
extern void Error_Handler(void);

// This function will be called when system meets fatal error.
//
void gmp_port_system_stuck(
	void
)
{
	// TODO add more code block to deal with system stuck
	//
	
	
	// invoke the Error handler
	//
	Error_Handler();
}



// ....................................................................//
// + Clock Management
// System Clock topology management.
//
#ifdef HAL_RCC_MODULE_ENABLED

GMP_PORT_TIME_T gmp_port_system_tick(
	void
)
{
	return HAL_GetTick();
}

#endif

// ....................................................................//
// + Power Management
// System has enable Power configurations.
//
#ifdef HAL_PWR_MODULE_ENABLED

#endif


// ....................................................................//
// Watch Gog
// System has enable Watch Dog module.
//

void gmp_port_feed_dog(
	void
)
{
#if defined HAL_IWDG_MODULE_ENABLED
	HAL_IWDG_Refresh(&hiwdg);
#endif // HAL_IWDG_MODULE_ENABLED, IWDG enabled
}


// ....................................................................//
// + CRC check
// System enable CRC coding check
//
#ifdef HAL_CRC_MODULE_ENABLED

#endif


