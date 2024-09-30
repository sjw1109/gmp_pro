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

#include <core/gmp_core.hpp>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

#include <math.h>



// Model Interface: transmit package
// Controller Interface: transmit package
typedef struct _tag_transmit_package_t
{
	double sin_result;
	double cos_result;
}transmit_package_t;

// Model Interface: receive package
// Controller Interface: transmit package
typedef struct _tag_receive_package_t
{

	double time;


}receive_package_t;




transmit_package_t tx_pak;
receive_package_t rx_pak;

// CTL initialize routine
void ctl_init()
{
	tx_pak.cos_result = 0;
	tx_pak.sin_result = 0;
	
#ifdef USING_SIMULINK_UDP_SIMULATE

	udp_svr_obj.recv_buf = &rx_pak;
	udp_svr_obj.send_buf = &tx_pak;
	udp_svr_obj.recv_buf_len = sizeof(rx_pak);
	udp_svr_obj.send_buf_len = sizeof(tx_pak);
#endif // USING_SIMULINK_UDP_SIMULATE
}

// CTL loop routine
void ctl_dispatch(void)
{
	
	// User Controller logic here.

	tx_pak.sin_result = sin(rx_pak.time * 10);
	tx_pak.cos_result = cos(rx_pak.time * 10);

	

}

void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
{

}

void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
{

}


void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
{
	// Noting to do
}
