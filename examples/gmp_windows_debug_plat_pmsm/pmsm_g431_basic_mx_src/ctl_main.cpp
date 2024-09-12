#include <core/gmp_core.hpp>
#include <ctl/ctl_core.h>

#include <user_main.hpp>

#include <ctl/suite/pmsm/pmsm_speed_closeloop/pmsm_speed_closeloop.h>
#include <ctl/component/common/stimulate.h>

#include <ctl/component/motor/pmsm.smo.h>

#include <ctl/component/motion/basic_pos_loop_p.h>

#include <math.h>

#include <ext/encoder/TLE5012/TLE5012.h>


#define SPI3_CS_Pin GPIO_PIN_2
#define SPI3_CS_GPIO_Port GPIOD

// Model Interface: transmit package
// Controller Interface: transmit package
//#pragma pack(1)
typedef struct _tag_transmit_package_t
{
	double scope[4];
	uint32_t duty[3];

	uint32_t enable;
}transmit_package_t;

// Model Interface: receive package
// Controller Interface: transmit package
//#pragma pack(1)
typedef struct _tag_receive_package_t
{

	double time;

	uint32_t adc_current[3];
	uint32_t adc_voltage[3];

	uint32_t adc_dc_current;
	uint32_t adc_dc_voltage;

	uint32_t motor_encoder;

}receive_package_t;


uint32_t adc_data1[3];

transmit_package_t tx_pak;
receive_package_t rx_pak;



#define BUILD_LEVEL 4

// extern variables
extern uint16_t adc_data[7];
extern TIM_HandleTypeDef htim1;
extern SPI_HandleTypeDef hspi3;

pmsm_ctl_object_t pmsm;

ctl_motor_driver_consultant_t drv = MOTOR_DRIVER_CONSULTANT_WRAPPER;
ctl_pmsm_nameplate_consultant_t np = MOTOR_PMSM_NAMEPLATE_CONSULTANT_WRAPPER;
ctl_pmsm_dsn_consultant_t dsn = MOTOR_PMSM_DESIGN_CONSULTANT_WRAPPER;

ctl_pmsm_smo_observer_t pmsm_smo;
ctl_pos_loop_p_ctrl_t pos_ctrl;

// open loop angle source
ctl_src_rg_t rg;


// Encoder buffer
uint32_t encoder_src;
ctrl_gt encoder_smo;

// Initialize function for GMP-CTL
// CTL initialize routine
void ctl_init()
{

#ifdef USING_SIMULINK_UDP_SIMULATE

	udp_svr_obj.recv_buf = &rx_pak;
	udp_svr_obj.send_buf = &tx_pak;
	udp_svr_obj.recv_buf_len = sizeof(rx_pak);
	udp_svr_obj.send_buf_len = sizeof(tx_pak);
	
		std::cout << "ctl_motor_driver_consultant_t: " << sizeof(ctl_motor_driver_consultant_t) << std::endl
		<< "ctl_pmsm_nameplate_consultant_t:" << sizeof(ctl_pmsm_nameplate_consultant_t) << std::endl
		<< "ctl_pmsm_dsn_consultant_t: " << sizeof(ctl_pmsm_dsn_consultant_t) << std::endl
		<< "ctl_src_rg_t:" << sizeof(ctl_src_rg_t) << std::endl
		<< "pmsm_ctl_object_t:" << sizeof(pmsm_ctl_object_t) << std::endl
		<< "pmsm_ctl_entity_t:" << sizeof(pmsm_ctl_entity_t) << std::endl
		<< "ctl_object_nano_t" << sizeof(ctl_object_nano_t) << std::endl;

	for (int i = 0; i < 4; ++i)
		tx_pak.scope[i] = 0;
	
#endif // USING_SIMULINK_UDP_SIMULATE




	


	ctl_init_pmsm_ctl_entity(&pmsm.ctrl);
	ctl_setup_pmsm_ctl_entity(&pmsm.ctrl, &drv, &np);
	ctl_tuning_pmsm_pid_via_consultant(&pmsm.ctrl, &dsn, &drv, &np);

	// Init ctrl nano framework
	ctl_init_nano_header(&pmsm.base);
	ctl_setup_nano_header(&pmsm.base, (uint32_t)drv.control_law_freq);

//	ctl_force_nona_header_online(&pmsm.base);

	// Open loop voltage & angle generator
	ctl_init_ramp_gen(&rg);
	ctl_setup_ramp_gen_via_amp_freq(&rg, 10e3, 21, 2 * PI, 0);
	
	// init and setup SMO
	ctl_init_pmsm_smo(&pmsm_smo);
	ctl_setup_pmsm_smo_via_consultant(&pmsm_smo,
		&dsn, &drv, &np,
		CTRL_T(1.0), CTRL_T(0.5),CTRL_T(0), 
		CTRL_T(1.0)
		);
		
	//  pos loop
	ctl_init_pos_loop_p_ctrl(&pos_ctrl);
	ctl_setup_pos_loop_p_ctrl(&pos_ctrl, CTRL_T(0.25), CTRL_T(0.2), 10);

#if BUILD_LEVEL == 1
	// OPEN LOOP
	ctl_set_pmsm_ctl_entity_as_openloop(&pmsm.ctrl);

	// sync Vdq
	ctl_set_pmsm_ctl_Vdq(&pmsm.ctrl, CTRL_T(0.08), CTRL_T(0.0));
#elif BUILD_LEVEL >= 2
	// Current Open Loop

	// Config Current PID
	ctl_set_pid_parameter(&pmsm.ctrl.Idq_ctl[phase_D].pid,
		CTRL_T(0.25), CTRL_T(0.05), CTRL_T(0.0)
	);

	ctl_set_pid_parameter(&pmsm.ctrl.Idq_ctl[phase_Q].pid,
		CTRL_T(0.25), CTRL_T(0.05), CTRL_T(0.0)
	);

	pmsm.ctrl.pos_encoder.offset = 2099199;

	//ctl_set_pid_limit(&pmsm.ctrl.Idq_ctl[phase_D].pid, CTRL_T(-0.25), CTRL_T(0.25));
	//ctl_set_pid_limit(&pmsm.ctrl.Idq_ctl[phase_Q].pid, CTRL_T(-0.25), CTRL_T(0.25));

	// OPEN LOOP
	ctl_set_pmsm_ctl_entity_as_currentloop(&pmsm.ctrl);

	// sync Idq
	ctl_set_pmsm_ctl_Idq(&pmsm.ctrl, CTRL_T(0.15), CTRL_T(0.0));

#endif

#if BUILD_LEVEL >= 3
	// Speed Loop

	// Config Speed PID
	ctl_set_pid_parameter(&pmsm.ctrl.spd_ctl.pid,
		CTRL_T(1.0), CTRL_T(0.05), CTRL_T(0.0)
	);

	ctl_set_pid_limit(&pmsm.ctrl.spd_ctl.pid, -CTRL_T(0.5), CTRL_T(0.5));


	// speed close loop
	ctl_set_pmsm_ctl_entity_as_speedloop(&pmsm.ctrl);

	// sync spd
	ctl_set_pmsm_ctl_spd(&pmsm.ctrl, CTRL_T(0.2));
	

#endif

#if BUILD_LEVEL >= 4
	// position loop
	
#endif 


#ifndef USING_SIMULINK_UDP_SIMULATE
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
#endif
}


// CTL loop routine
// This function would be called every controller ISR
void ctl_dispatch(void)
{



	// Call CTL nano framework
	controller_dispatch((ctl_object_nano_t*)&pmsm);



}


void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
{

		pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

#ifdef USING_SIMULINK_UDP_SIMULATE


#else
	rx_pak.adc_current[0] = adc_data[0];
	asm("nop");
  rx_pak.adc_current[1] = adc_data[2];
	asm("nop");
  rx_pak.adc_current[2] = adc_data[4];	

	rx_pak.adc_dc_voltage = adc_data[3];
	
	rx_pak.motor_encoder = encoder_src;
			
#endif // USING_SIMULINK_UDP_SIMULATE

		// input motor controller
	ctl_input_pmsm_ctl(&obj->ctrl,
		rx_pak.adc_current, rx_pak.adc_voltage,
		rx_pak.adc_dc_current, rx_pak.adc_dc_voltage,
		rx_pak.motor_encoder);

	
	return;
}

void ctl_core_stage_routine(ctl_object_nano_t* pctl_obj)
{
	pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

	
	// User Controller logic here.

#if (BUILD_LEVEL == 1)
	// force angle
//		ctl_set_pmsm_force_angle(&pmsm.ctrl, 0);
	ctl_set_pmsm_force_angle(&pmsm.ctrl, CTRL_T(2*3.14/3));

	ctl_set_pmsm_force_angle(&pmsm.ctrl, ctl_get_ramp_gen_output(&rg));
	ctl_step_ramp_gen(&rg);
#endif

#if (BUILD_LEVEL == 2)
	//ctl_set_pmsm_force_angle(&pmsm.ctrl, CTRL_T(0));

	ctl_set_pmsm_force_angle(&pmsm.ctrl, ctl_get_ramp_gen_output(&rg));
	ctl_step_ramp_gen(&rg);

#endif 

	ctl_input_pos_loop_p_ctrl_via_only_ang(&pos_ctrl, obj->ctrl.pos_encoder.position);
	ctl_step_pos_loop_p_ctrl(&pos_ctrl);
	
#if(BUILD_LEVEL == 4)
	obj->ctrl.spd_set_user = pos_ctrl.speed_ref;
#endif 
	
	ctl_step_pmsm_ctl_entity(&obj->ctrl);
	
//	ctl_input_pmsm_smo(&pmsm_smo,
//		obj->ctrl.Vab_set.dat[0],obj->ctrl.Vab_set.dat[1],
//		obj->ctrl.Iab.dat[0],obj->ctrl.Iab.dat[1]);

//	encoder_smo = ctl_step_pmsm_smo(&pmsm_smo);
}

void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
{
	
		pmsm_ctl_object_t* obj = (pmsm_ctl_object_t*)pctl_obj;

#ifdef USING_SIMULINK_UDP_SIMULATE
	
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
#else
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_U));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_V));
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, ctl_pmsm_ctl_get_outout_cmp(&obj->ctrl, phase_W));
#endif

}


void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
{
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_RESET);
	
	uint16_t req_cmd = TLE5012_READ_ANGLE; 
	uint16_t rec_cmd = 0;
	
	HAL_SPI_Transmit(&hspi3, (uint8_t*)&req_cmd, 1, 1);
	for (int i=0;i<20;++i) asm("nop");
	
	req_cmd = 0xFFFF;
	HAL_SPI_Receive(&hspi3, (uint8_t*)&rec_cmd, 1, 1);
	
	HAL_GPIO_WritePin(SPI3_CS_GPIO_Port, SPI3_CS_Pin, GPIO_PIN_SET);
	
	if((rec_cmd & 0x8000) == 0x8000)
	{
		encoder_src = rec_cmd & 0x7FFF;
	}

}







// ADC Complete callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	uint32_t cpu_usage = __HAL_TIM_GET_COUNTER(&htim1);
	
    ctl_dispatch();
	
	uint32_t cpu_usage2 = __HAL_TIM_GET_COUNTER(&htim1);
	
	pmsm.base.control_law_CPU_usage_tick = cpu_usage2 - cpu_usage;
}
