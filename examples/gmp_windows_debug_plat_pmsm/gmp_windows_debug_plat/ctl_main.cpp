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

#include <ctl/suite/pmsm/pmsm_speed_closeloop/pmsm_speed_closeloop.h>
#include <ctl/component/common/stimulate.h>

#include <ctl/component/motor/pmsm.smo.h>

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

ctl_pmsm_smo_observer_t pmsm_smo;

// open loop angle source
ctl_src_rg_t rg;

#define BUILD_LEVEL 3


// CTL initialize routine
void ctl_init()
{

#ifdef USING_SIMULINK_UDP_SIMULATE

	udp_svr_obj.recv_buf = &rx_pak;
	udp_svr_obj.send_buf = &tx_pak;
	udp_svr_obj.recv_buf_len = sizeof(rx_pak);
	udp_svr_obj.send_buf_len = sizeof(tx_pak);
#endif // USING_SIMULINK_UDP_SIMULATE


	std::cout << "ctl_motor_driver_consultant_t: " << sizeof(ctl_motor_driver_consultant_t) << std::endl
		<< "ctl_pmsm_nameplate_consultant_t:" << sizeof(ctl_pmsm_nameplate_consultant_t) << std::endl
		<< "ctl_pmsm_dsn_consultant_t: " << sizeof(ctl_pmsm_dsn_consultant_t) << std::endl
		<< "ctl_src_rg_t:" << sizeof(ctl_src_rg_t) << std::endl
		<< "pmsm_ctl_object_t:" << sizeof(pmsm_ctl_object_t) << std::endl
		<< "pmsm_ctl_entity_t:" << sizeof(pmsm_ctl_entity_t) << std::endl
		<< "ctl_object_nano_t" << sizeof(ctl_object_nano_t) << std::endl;


	for (int i = 0; i < 4; ++i)
		tx_pak.scope[i] = 0;

	ctl_init_pmsm_ctl_entity(&pmsm.ctrl);
	ctl_setup_pmsm_ctl_entity(&pmsm.ctrl, &drv, &np);
	ctl_tuning_pmsm_pid_via_consultant(&pmsm.ctrl, &dsn, &drv, &np);

	// Init ctrl nano framework
	ctl_init_nano_header(&pmsm.base);
	ctl_setup_nano_header(&pmsm.base, (uint32_t)drv.control_law_freq);

	ctl_force_nona_header_online(&pmsm.base);

	// Open loop voltage & angle generator
	ctl_init_ramp_gen(&rg);
	ctl_setup_ramp_gen_via_amp_freq(&rg, 10e3, 5, 2 * PI, 0);

#if BUILD_LEVEL == 1
	// OPEN LOOP
	ctl_set_pmsm_ctl_entity_as_openloop(&pmsm.ctrl);

	// sync Vdq
	ctl_set_pmsm_ctl_Vdq(&pmsm.ctrl, CTRL_T(0.1), CTRL_T(0));
#elif BUILD_LEVEL >= 2
	// Current Open Loop

	// Config Current PID
	ctl_set_pid_parameter(&pmsm.ctrl.Idq_ctl[phase_D].pid,
		CTRL_T(5), CTRL_T(0.05), CTRL_T(0.0)
	);

	ctl_set_pid_parameter(&pmsm.ctrl.Idq_ctl[phase_Q].pid,
		CTRL_T(5), CTRL_T(0.05), CTRL_T(0.0)
	);


	//ctl_set_pid_limit(&pmsm.ctrl.Idq_ctl[phase_D].pid, CTRL_T(-0.25), CTRL_T(0.25));
	//ctl_set_pid_limit(&pmsm.ctrl.Idq_ctl[phase_Q].pid, CTRL_T(-0.25), CTRL_T(0.25));

	// OPEN LOOP
	ctl_set_pmsm_ctl_entity_as_currentloop(&pmsm.ctrl);

	// sync Idq
	ctl_set_pmsm_ctl_Idq(&pmsm.ctrl, CTRL_T(0.0), CTRL_T(0.2));

#endif

#if BUILD_LEVEL >= 3
	// Speed Loop

	// Config Speed PID
	ctl_set_pid_parameter(&pmsm.ctrl.spd_ctl.pid,
		CTRL_T(17.0), CTRL_T(0.08), CTRL_T(0.0)
	);

	ctl_set_pid_limit(&pmsm.ctrl.spd_ctl.pid, CTRL_T(-0.25), CTRL_T(0.25));


	// speed close loop
	ctl_set_pmsm_ctl_entity_as_speedloop(&pmsm.ctrl);

	// sync spd
	ctl_set_pmsm_ctl_spd(&pmsm.ctrl, -CTRL_T(0.1));
	

#endif
}

void ctl_loop(void)
{
    controller_state_dispatch((ctl_object_nano_t *)&pmsm);
}

// CTL loop routine
void ctl_dispatch(void)
{


	// User Controller logic here.

#if (BUILD_LEVEL == 1)
	// force angle
	//	ctl_set_pmsm_force_angle(&pmsm.ctrl, 0);

	ctl_set_pmsm_force_angle(&pmsm.ctrl, ctl_get_ramp_gen_output(&rg));
	ctl_step_ramp_gen(&rg);
#endif

#if (BUILD_LEVEL == 2)
	//ctl_set_pmsm_force_angle(&pmsm.ctrl, 0);

	ctl_set_pmsm_force_angle(&pmsm.ctrl, ctl_get_ramp_gen_output(&rg));

#endif 
	// Call CTL nano framework
	controller_dispatch((ctl_object_nano_t*)&pmsm);



}

void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
{
	pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

	// input motor controller
	ctl_input_with_inv_current_pmsm_ctl(&obj->ctrl,
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
	tx_pak.scope[0] = ctl_get_elec_angle_via_pos_encoder(&obj->ctrl.pos_encoder);


	//tx_pak.scope[1] = ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_U);
	//tx_pak.scope[2] = ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_V);
	//tx_pak.scope[3] = ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_W);

	//
	//tx_pak.scope[1] = obj->ctrl.Iabc.value[0];
	//tx_pak.scope[2] = obj->ctrl.Iabc.value[1];
	//tx_pak.scope[3] = obj->ctrl.Iabc.value[2];

	//tx_pak.scope[1] = obj->ctrl.Idq0.dat[0];
	//tx_pak.scope[2] = obj->ctrl.Idq0.dat[1];
	//tx_pak.scope[3] = obj->ctrl.Idq0.dat[2];

	//tx_pak.scope[1] = obj->ctrl.Vabc.value[0];
	//tx_pak.scope[2] = obj->ctrl.Vabc.value[1];
	//tx_pak.scope[3] = obj->ctrl.Vabc.value[2];

	//tx_pak.scope[1] = obj->ctrl.Vdq0.dat[0];
	//tx_pak.scope[2] = obj->ctrl.Vdq0.dat[1];
	//tx_pak.scope[3] = obj->ctrl.Vdq0.dat[2];

	tx_pak.scope[0] = obj->ctrl.spd_ctl.traj.out; // spd_set
	tx_pak.scope[1] = ctl_get_pmsm_ctl_spd(&obj->ctrl);
	tx_pak.scope[2] = obj->ctrl.Idq_set[1];
	tx_pak.scope[3] = obj->ctrl.Idq0.dat[1]; // q
}


