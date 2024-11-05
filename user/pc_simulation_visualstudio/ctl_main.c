/**
 * @file ctl_main.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

#include <gmp_core.h>
#include <ctl/ctl_core.h>
#include <math.h>


// User may set (get handle) TX content via `gmp_csp_sl_get_tx_buffer`
// User may get (get handle) RX content via `gmp_csp_sl_get_rx_buffer`


// CTL initialize routine
void ctl_init()
{

	
#ifdef USING_SIMULINK_UDP_SIMULATE

#endif // USING_SIMULINK_UDP_SIMULATE
}

// CTL loop routine
void ctl_dispatch(void)
{
	
	// User Controller logic here.


	

}

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
{

}

void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
{

}


void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
{
	
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
