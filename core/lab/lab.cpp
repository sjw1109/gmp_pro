// This file is provided for developers to test their code firstly
// So the user code and test code may separate into two parts.

#include <core/gmp_core.hpp>

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
//gmp_size_t ret;

//pwm_dev_t pwm;
//sdspc_buck_ctrl_t sdspc;
//quadri_sect_ctl_wf_pack sdspc_wf;




// This function would be called when test mode.
// WEAK_FUNCTION
void gmp_test_init()
GMP_NO_OPT
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

//	gmp_data_t data[12] = { 'a','b','c','d','e','f','g' };

//	gmp_port_recv_int_response(
//		&gmp_uart_dbg,
//		data, 7,
//		0
//	);
//	gmp_data_t recv_data[8] = { 0 };
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
}

// This function would be called when test mode.
// WEAK_FUNCTION
void gmp_test_loop()
GMP_NO_OPT
{
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

//	gmp_data_t recv_data[8] = { 0 };

//	if (gmp_dev_read_buf(
//		&gmp_uart_dbg,
//		recv_data, 7))
//		gmp_dbg_prt("[Receive]:%s", recv_data);

//	int command = 0x2e;

//	iic1.write(0b1110111, 0xF4, (gmp_data_t*)&command, 1);

//	HAL_Delay(10);

//	uint32_t result_msb;
//	iic1.read(0b1110111, 0xF6, (gmp_data_t*)&result_msb, 1);

//	uint32_t result_lsb;
//	iic1.read(0b1110111, 0xF7, (gmp_data_t*)&result_lsb, 1);

//	uint32_t result = (result_lsb & 0xFF) | ((result_msb & 0xFF) << 8);

//	char result_str[20];

//	memset(result_str, 0, 20);
//	sprintf(result_str, "Temperature:%d\r\n", result);
//	gmp_size_t length = strlen(result_str);

//	dbg_uart.write((unsigned char*)result_str, length);

//	//	sdspc.gmp_sdspc_ctrl_g();

//	HAL_Delay(1000);

//	if (sdspc.flag_run)
//	{
//		enable_pwm(&htim1);
//	}

}

#endif // SPECIFY_ENABLE_TEST_ENVIRONMENT
