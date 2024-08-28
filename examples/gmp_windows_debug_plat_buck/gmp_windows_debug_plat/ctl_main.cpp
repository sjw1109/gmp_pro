#include <core/gmp_core.hpp>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

#include <math.h>



// Model Interface: transmit package
// Controller Interface: transmit package
#pragma pack(1)
typedef struct _tag_transmit_package_t
{
	double sin_result;
	int32_t duty;
}transmit_package_t;

// Model Interface: receive package
// Controller Interface: transmit package
#pragma pack(1)
typedef struct _tag_receive_package_t
{

	double time;

	uint32_t adc_current;
	uint32_t adc_voltage;

}receive_package_t;




transmit_package_t tx_pak;
receive_package_t rx_pak;

// CTL initialize routine
void ctl_init()
{
	tx_pak.sin_result = 0;
	tx_pak.duty = 0;
	
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

	tx_pak.sin_result = rx_pak.time;
	tx_pak.duty = 0.5f * 2000;

	

}

//void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
//{
//
//}
//
//void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
//{
//
//}
//
//
//void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
//{
//	// Noting to do
//}
