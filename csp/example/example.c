// This file provide a set of function that CSP must defined.

#include <core/gmp_core.h>

// User should invoke this function to get time (system tick).
time_gt gmp_port_system_tick(void)
{}

void gmp_port_feed_dog(void)
{}

// This function may be called and used to initilize all the peripheral. 
void gmp_csp_startup(void)
{}

// This function would be called when fatal error occorred.
void gmp_port_system_stuck(void)
{}

// This function would be called when all the initilization process happened.
void csp_post_process(void)
{}
	
// This function is unreachable.
void gmp_exit_routine(void)
{}

// This function may invoke when main loop occurred.
void gmp_csp_loop(void)
{}

