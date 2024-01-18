
// invoke GMP header
#include <core/gmp_core.hpp>

//////////////////////////////////////////////////////////////////////////
// Timer function 

// This function get the system tick.
GMP_PORT_TIME_T gmp_port_system_tick(
	void
)
{
	return 0;
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
void gmp_port_feed_dog()
{
	// This function is defined in TI C2000ware.
	// you may find the function definition in 
	// <ti/c2000/C2000Ware_<version>/device_support/<device>/common/source/<device>_SysCtrl.c>
	// prototype is void ServiceDog(void);
	ServiceDog();
}

//////////////////////////////////////////////////////////////////////////
// IO peripheral function implement


