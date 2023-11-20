#include "dsp_c2000_general.h"

#ifndef _FILE_DSP_C28X_GENERAL_H_
#define _FILE_DSP_C28X_GENERAL_H_



#define GMP_IIC_HANDLE_T struct I2C_REGs*

void iica_init(void);
GMP_LENGTH_T gmp_iic_transmit(GMP_IIC_HANDLE_T hi2c, GMP_LENGTH_T iic_dev_addr, GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len);
GMP_LENGTH_T gmp_iic_receive(GMP_IIC_HANDLE_T hi2c, GMP_LENGTH_T iic_dev_addr, GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len);


#endif

