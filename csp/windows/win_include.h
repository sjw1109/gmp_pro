#include <csp/chip_default_type.h>

#ifndef _FILE_WIN_INCLUDE_H_
#define _FILE_WIN_INCLUDE_H_


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	// Port Functions Prototype
	GMP_LENGTH_T gmp_iic_transmit
	(
		GMP_PORT_IIC_HANDLE_T hi2c,
		GMP_LENGTH_T iic_dev_addr,
		GMP_DATA_ELEMENT_T* data, 
		GMP_LENGTH_T len
	);

	void gmp_iic_request_read_mem
	(
		GMP_PORT_IIC_HANDLE_T hi2c,
		GMP_CMD_T cmd,
		GMP_LENGTH_T length
	);

	GMP_LENGTH_T gmp_iic_receive(
		GMP_PORT_IIC_HANDLE_T hi2c,
		GMP_LENGTH_T iic_dev_addr,
		GMP_CMD_T cmd, GMP_LENGTH_T cmd_len,
		GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len
	);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //  _FILE_WIN_INCLUDE_H_

