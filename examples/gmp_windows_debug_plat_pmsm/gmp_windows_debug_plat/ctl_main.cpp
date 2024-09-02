#include <core/gmp_core.hpp>
#include <ctl/ctl_core.h>

#include <ctl/suite/pmsm/pmsm_speed_closeloop/pmsm_speed_closeloop.h>
#include <ctl/component/common/stimulate.h>

#include <math.h>




// Model Interface: transmit package
// Controller Interface: transmit package
#pragma pack(1)
typedef struct _tag_transmit_package_t
{
	double scope[4];
	uint32_t duty[3];

	uint32_t enable;
}transmit_package_t;

// Model Interface: receive package
// Controller Interface: transmit package
#pragma pack(1)
typedef struct _tag_receive_package_t
{

	double time;

	uint32_t adc_current[3];
	uint32_t adc_voltage[3];

	uint32_t adc_dc_current;
	uint32_t adc_dc_voltage;

	uint32_t motor_encoder;

}receive_package_t;




transmit_package_t tx_pak;
receive_package_t rx_pak;


pmsm_ctl_object_t pmsm;

ctl_motor_driver_consultant_t drv = MOTOR_DRIVER_CONSULTANT_WRAPPER;
ctl_pmsm_nameplate_consultant_t np = MOTOR_PMSM_NAMEPLATE_CONSULTANT_WRAPPER;
ctl_pmsm_dsn_consultant_t dsn = MOTOR_PMSM_DESIGN_CONSULTANT_WRAPPER;

// open loop angle source
ctl_src_rg_t rg;


// CTL initialize routine
void ctl_init()
{

#ifdef USING_SIMULINK_UDP_SIMULATE

	udp_svr_obj.recv_buf = &rx_pak;
	udp_svr_obj.send_buf = &tx_pak;
	udp_svr_obj.recv_buf_len = sizeof(rx_pak);
	udp_svr_obj.send_buf_len = sizeof(tx_pak);
#endif // USING_SIMULINK_UDP_SIMULATE

	for (int i = 0; i < 4; ++i)
		tx_pak.scope[i] = 0;

	ctl_init_pmsm_ctl_entity(&pmsm.ctrl);
	ctl_setup_pmsm_ctl_entity(&pmsm.ctrl, &drv, &np);
	ctl_tuning_pmsm_pid_via_consultant(&pmsm.ctrl, &dsn, &drv);

	// Open loop voltage & angle generator
	ctl_init_ramp_gen(&rg);
	ctl_setup_ramp_gen_via_amp_freq(&rg, 10e3, 20, 2 * PI, 0);
}

// CTL loop routine
void ctl_dispatch(void)
{


	// User Controller logic here.

	// Call CTL nano framework
	controller_dispatch((ctl_object_nano_t*) & pmsm);
	



}

void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
{
	pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

	// input motor controller
	ctl_input_pmsm_ctl(&obj->ctrl, 
		rx_pak.adc_current, rx_pak.adc_voltage, 
		rx_pak.adc_dc_current, rx_pak.adc_dc_voltage, 
		rx_pak.motor_encoder);
}

void ctl_core_stage_routine(ctl_object_nano_t* pctl_obj)
{
	pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

	ctl_step_pmsm_ctl_entity(&obj->ctrl);
}



void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
{
	pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

	// enable signal
	tx_pak.enable = ctl_get_pmsm_ctl_output_enable(&obj->ctrl);


	// pwm signal
	tx_pak.duty[0] = ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_U);
	tx_pak.duty[1] = ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_V);
	tx_pak.duty[2] = ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_W);

	// scope signal

}


