
#include <string.h>
#include <bsp/bsp_header.h>
#include <core/util/ds/data_ring_buffer.h>
#include <core/dev/io_functions.h>

// global variables here
uint8_t pData[2] = { 0 };
gmp_size_t ret;


// initialize routine here.
void user_init(void)
GMP_NO_OPT
{


	int* pint = gmp_malloc(10 * sizeof(int));
	float* pf = gmp_malloc(100 * sizeof(float));

	*pint = 10;
	*pf = 2.34;

	gmp_free(pint);
	gmp_free(pf);

	gmp_ring_buffer_t* buf;

	init_ring_buffer(&buf, 24);

	for (int i = 0; i < 20; ++i)
		rb_push(buf, '1' + i);


	rb_push(buf, '1');
	rb_push(buf, '2');
	rb_push(buf, '3');
	rb_push(buf, '4');
	rb_push(buf, '5');
	rb_push(buf, '6');

	release_ring_buffer(buf);

	gmp_data_t data[12] = { 'a','b','c','d','e','f','g' };

	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 7,
		0
	);
	gmp_data_t recv_data[8] = { 0 };
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 7);


	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 4,
		0
	);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 7);


	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 5,
		0
	);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 2);


	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 7,
		0
	);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);

	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 7,
		0
	);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 4);

	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 7,
		0
	);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 5);

	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		data, 7,
		0
	);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
	gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 3);
}

uint8_t result180[2] = { 0, 0 };

// endless loop function here
void user_loop(void)
GMP_NO_OPT
{
	uint8_t cmd[2] = { 0xF4, 0x2E };
	uint32_t ret = gmp_dev_write((gmp_phy_mapping_t*)&gmp_iic_bmp180, cmd, 2);
	HAL_Delay(5);


	uint8_t cmd2 = 0xF6;
	ret = gmp_dev_read((gmp_phy_mapping_t*)&gmp_iic_bmp180, &cmd2, 1, result180, 2);

	char* string = "\r\n\r\nGMP is running!\r\n";
	ret = gmp_dev_write(&gmp_uart_dbg_port, (unsigned char*)string, strlen(string));

	gmp_dbg_prt("Hello World!\r\n");
	gmp_dbg_prt("Pressure Source data:%d\r\n", *(uint16_t*)result180);
	uint8_t a = 1;

	gmp_data_t recv_data[8] = { 0 };

	if (gmp_dev_read_buf(
		&gmp_uart_dbg,
		recv_data, 7))
		gmp_dbg_prt("[Receive]:%s", recv_data);

	HAL_Delay(1000);


}

