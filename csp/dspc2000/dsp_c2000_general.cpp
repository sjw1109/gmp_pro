
// invoke GMP header
#include <core/gmp_core.hpp>

//////////////////////////////////////////////////////////////////////////
// Timer function 

// This function get the system tick.
GMP_PORT_TIME_T gmp_port_system_tick(
	void
)
{
	return HAL_GetTick();
}

// This function would invoke when system meets some fatal errors.
void gmp_port_system_stuck(
	void
)
{
	// invoke the Error handler
	for(;;){}

}


// The following function is a feed a dog routine
// For STM32, the watchdog is IWDG.
void gmp_port_feed_dog()
{

}
