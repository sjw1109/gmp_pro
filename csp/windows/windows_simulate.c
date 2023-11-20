
#include <core/std/typedef.h>

// iic port
GMP_LENGTH_T gmp_iic_transmit(
	GMP_PORT_IIC_HANDLE_T hi2c,
	GMP_LENGTH_T iic_dev_addr,
	GMP_DATA_ELEMENT_T* data,
	GMP_LENGTH_T len)
{

	return 0;
}

void gmp_iic_request_read_mem
(
	GMP_PORT_IIC_HANDLE_T hi2c,
	GMP_CMD_T cmd,
	GMP_LENGTH_T length
)
{
	return;

}

GMP_LENGTH_T gmp_iic_receive(
	GMP_PORT_IIC_HANDLE_T hi2c,
	GMP_LENGTH_T iic_dev_addr,
	GMP_CMD_T cmd, GMP_LENGTH_T cmd_len,
	GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len)
{
	return len;
}
