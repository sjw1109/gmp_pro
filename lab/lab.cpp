// This file is provided for developers to test their code firstly
// So the user code and test code may separate into two parts.


// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <core/gmp_core.hpp>
#include <core/util/ds/data_ring_buffer.h>

// extensions

// Controller Template Library
#include <ctl/ctl_core.h>

// include the platform
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

#include <ctl/component/common/filter.h>

// include the simulate function 
#include "Buck_constant_param.h"

#include <fstream>

#include <math.h>


#if defined SPECIFY_ENABLE_TEST_ENVIRONMENT

//#include <string.h>
//#include <stdio.h>

//#include <core/gmp_core.hpp>

//#include <bsp/bsp_header.h>
//#include <core/util/ds/data_ring_buffer.h>
//#include <core/dev/io_functions.h>
//#include <ctrl/ctrl_config.h>
//#include <ctrl/ctrl_plant/sdspc/sdspc_general.hpp>
//#include <core/dev/pwm_device.h>
//#include <ext/dac/DAC8563/dac8563.h>

//// global variables

//uint8_t result180[2] = { 0, 0 };
//uint8_t pData[2] = { 0 };
//size_gt ret;

//pwm_dev_t pwm;
//sdspc_buck_ctrl_t sdspc;
//quadri_sect_ctl_wf_pack sdspc_wf;

uint16_t adc_data[6] = { 0 };
uint16_t pin_stat = 0;

buck_boost_2ch_ctl_object_t buck_boost;

std::ofstream buck_out("buck_out.csv");
std::ofstream filter_out("filter_out.csv");


// This function would be called when test mode.
// WEAK_FUNCTION
GMP_NO_OPT_PREFIX
void gmp_test_init()
GMP_NO_OPT_SUFFIX
{
	//	int* pint = (int*)gmp_malloc(10 * sizeof(int));
	//	float* pf = (float*)gmp_malloc(100 * sizeof(float));

	//	*pint = 10;
	//	*pf = 2.34;

	//	gmp_free(pint);
	//	gmp_free(pf);

	//	gmp_ring_buffer_t* buf;

	//	init_ring_buffer(&buf, 24);

	//	for (int i = 0; i < 20; ++i)
	//		rb_push(buf, '1' + i);


	//	rb_push(buf, '1');
	//	rb_push(buf, '2');
	//	rb_push(buf, '3');
	//	rb_push(buf, '4');
	//	rb_push(buf, '5');
	//	rb_push(buf, '6');

	//	release_ring_buffer(buf);

	//	data_gt data[12] = { 'a','b','c','d','e','f','g' };

	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 7,
	//		0
	//	);
	//	data_gt recv_data[8] = { 0 };
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 7);


	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 4,
	//		0
	//	);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 7);


	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 5,
	//		0
	//	);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 2);


	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 7,
	//		0
	//	);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);

	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 7,
	//		0
	//	);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 4);

	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 7,
	//		0
	//	);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 5);

	//	gmp_port_recv_int_response(
	//		&gmp_uart_dbg,
	//		data, 7,
	//		0
	//	);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);
	//	gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 3);



	//	// TEST pwm OUTPUT 
	//	//HAL_TIM_Base_Start(&htim1);
	////	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	////	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_1);
	////	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	////	HAL_TIMEx_PWMN_Start(&htim1,TIM_CHANNEL_2);

	//	// DAC output
	//	da.clear();

	//	da.setup();

	//	// 	da.write_channel_1(0x9000);
	//	// 	HAL_Delay(100);
	//	//	da.write_channel_1(60000);

	//	//	HAL_Delay(1000);



	//	init_pwm_device(&pwm, &htim1, 64e6, 10e3);

	//	gmp_get_dead_time(&pwm, 200);

	//	enable_pwm(&htim1);

	//	// ADC output
	//	HAL_ADCEx_Calibration_Start(&hadc1);
	//	HAL_StatusTypeDef stat = HAL_ADC_Start_DMA(&hadc1, adc_data, 4);


	//	for (int i = 0; i < 10; i++)
	//	{
	//		gmp_write_pwm_cmp(pwm.handle, GMP_PWM_CHN_1, pwm.pwm_full_scale * 0.1 * i);
	//		//		da.write_dual_channel(6500 * i, 6500 * i);
	//		HAL_Delay(1000);
	//	}

	//	gmp_dbg_prt("Controller memory size: %d byte(s)\r\n", sizeof(sdspc_buck_ctrl_t));
	//	gmp_dbg_prt("Node memory size: %d byte(s)\r\n", sizeof(gmp_wf_node_gt));
	//	gmp_dbg_prt("Workflow memory size: %d byte(s)\r\n", sizeof(gmp_workflow_t));
	//	gmp_dbg_prt("Controller workflow package size: %d byte(s)\r\n", sizeof(quadri_sect_ctl_wf_pack));
	//	gmp_dbg_prt("Workflow section function type: %d byte(s)\r\n", sizeof(routine_fn_t));

	//	// trans_setup a controller one by one

	//	// bind a controller output object with a PWM device.
	//	sdspc.bind(&pwm, GMP_PWM_CHN_1);

	//	// bind a controller with a workflow object
	//	sdspc_wf.wf_init(&sdspc);

	//	sdspc.close_loop_level = sdspc_buck_ctrl_t::OPENLOOP;

	//	sdspc.user_if.duty = 0.15;
	//	sdspc.flag_run = 1;
	//	// And then start the workflow of the controller

	//	HAL_Delay(500);

	//	char* happy_str = "Happy New Year\r\n";

	//	dbg_uart.write((unsigned char*)happy_str, 17);


	//	// wfr_ready to launch the SDSPC object

uint32_t ticks = 100000;


	_iq* data = new _iq[ticks];

	for (int i = 0; i < ticks; ++i)
		data[i] = _IQ(1.0f + sinf(2.0 * 3.1415926f / 100 * i) + sinf(2.0 * 3.1415926f / 100 * i * 10));

	filter_IIR2_setup_t filter_setup;

	filter_setup.fc = 1;
	filter_setup.filter_type = FILTER_IIR2_TYPE_LOWPASS;
	filter_setup.fs = 500;
	filter_setup.gain = 1.0f;
	filter_setup.q = 1.0f;

	filter_IIR2_t filter;

	ctl_init_filter_iir2(&filter);
	
	ctl_setup_filter_iir2(&filter, &filter_setup);

	for (int i = 0; i < ticks; ++i)
	{
		ctl_filter_iir2_calc(&filter, data[i]);

		filter_out << _IQ24toF(data[i]) << ", " << _IQ24toF(filter.out) << ", " << std::endl;
	}

	delete[] data;


	init_buck_boost_2ch_ctl(&buck_boost);
	buck_boost.base.state_machine = CTL_SM_ONLINE;
	buck_boost.ctrl.V_target = _IQ(0.5);

}

fpga_math_fix_t i_L = 0;		// state variables current L
fpga_math_fix_t u_C = 0; 		// state variables voltage C
fpga_ctrl_bit sw;
fpga_math_fix_t i_R;		// output variables iR
fpga_math_fix_t u_R;	// output variables uR

fpga_math_fix_t i_L_iter;
fpga_math_fix_t u_C_iter;


// This function would be called when test mode.
// WEAK_FUNCTION
GMP_NO_OPT_PREFIX
void gmp_test_loop()
GMP_NO_OPT_SUFFIX
{

	// Buck Boost ADC result
	int i;


	// Buck Circuit Simulate

	uint32_t buck_total_step = 1.0f / step / ISR_FREQUENCY;
	uint32_t sw_pos = _IQ24toF(buck_boost.pwm[0].raw) * buck_total_step;

	for (i = 0; i < 1 / step / ISR_FREQUENCY; ++i)
	{
		if (i < sw_pos)
			sw = 1;
		else
			sw = 0;

		buck_trans(
			i_L, u_C, sw,
			&i_L_iter, &u_C_iter,
			&i_R, &u_R);

		i_L = i_L_iter;
		u_C = u_C_iter;

		//buck_out << _IQ24toF(i_L) << ", " << _IQ24toF(u_C) << ", " << (uint32_t)sw << ", " << _IQ24toF(i_R) << ", " << _IQ24toF(u_R) << ", " << std::endl;
		buck_out << i_L * I_base << ", " << u_C * E_base << ", " << (uint32_t)sw << ", " << i_R * I_base << ", " << u_R * E_base << ", " << std::endl;
	}

	buck_boost.adc_results[U_out].raw = _IQ24toIQ12(buck_boost.adc_results[U_out].bias + _IQ(u_R));
	buck_boost.adc_results[I_out].raw = _IQ24toIQ12(buck_boost.adc_results[I_out].bias + _IQ(i_R));

	controller_dispatch((ctl_object_nano_t*)&buck_boost);

	for (int i = 0; i < 10; ++i)
	{
		// Enable Controller state machine
		controller_state_dispatch((ctl_object_nano_t*)&buck_boost);
	}


	//	uint8_t cmd[2] = { 0xF4, 0x2E };
	//	uint32_t ret = gmp_dev_write((gmp_phy_mapping_t*)&gmp_iic_bmp180, cmd, 2);
	//	HAL_Delay(5);


	//	uint8_t cmd2 = 0xF6;
	//	ret = gmp_dev_read((gmp_phy_mapping_t*)&gmp_iic_bmp180, &cmd2, 1, result180, 2);

	//	char* string = "\r\n\r\nGMP is running!\r\n";
	//	ret = gmp_dev_write(&gmp_uart_dbg_port, (unsigned char*)string, strlen(string));

	//	gmp_dbg_prt("Hello World!\r\n");
	//	gmp_dbg_prt("Pressure Source data:%d\r\n", *(uint16_t*)result180);
	//	uint8_t a = 1;

	//	data_gt recv_data[8] = { 0 };

	//	if (gmp_dev_read_buf(
	//		&gmp_uart_dbg,
	//		recv_data, 7))
	//		gmp_dbg_prt("[Receive]:%s", recv_data);

	//	int command = 0x2e;

	//	iic1.write(0b1110111, 0xF4, (data_gt*)&command, 1);

	//	HAL_Delay(10);

	//	uint32_t result_msb;
	//	iic1.read(0b1110111, 0xF6, (data_gt*)&result_msb, 1);

	//	uint32_t result_lsb;
	//	iic1.read(0b1110111, 0xF7, (data_gt*)&result_lsb, 1);

	//	uint32_t result = (result_lsb & 0xFF) | ((result_msb & 0xFF) << 8);

	//	char result_str[20];

	//	memset(result_str, 0, 20);
	//	sprintf(result_str, "Temperature:%d\r\n", result);
	//	size_gt length = strlen(result_str);

	//	dbg_uart.write((unsigned char*)result_str, length);

	//	//	sdspc.gmp_sdspc_ctrl_g();

	//	HAL_Delay(1000);

	//	if (sdspc.flag_run)
	//	{
	//		enable_pwm(&htim1);
	//	}

}

#endif // SPECIFY_ENABLE_TEST_ENVIRONMENT
