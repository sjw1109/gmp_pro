#include "DSP28x_Project.h" 

// I2CA_Init - From C2000Ware
void iica_init(void)
{
	//
	// Initialize I2C
	//
	I2caRegs.I2CSAR = 0x0050;		// Slave address - EEPROM control code

	I2caRegs.I2CPSC.all = 6;		// Prescaler - need 7-12 Mhz on module clk
	I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
	I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
	I2caRegs.I2CIER.all = 0x24;		// Enable SCD & ARDY interrupts

	//
	// Take I2C out of reset. Stop I2C when suspended
	//
	I2caRegs.I2CMDR.all = 0x0020;

	I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
	I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

	return;
}

GMP_LENGTH_T gmp_iic_transmit(GMP_IIC_HANDLE_T hi2c, GMP_LENGTH_T iic_dev_addr, GMP_DATA_ELEMENT_T* data, GMP_LENGTH_T len)
{
	uint16_t i;

	// I2C is still not ready
	if (I2caRegs.I2CMDR.bit.STP == 1)
	{
		gmp_io_last_error = GMP_STAT_NOT_READY;
		return 0; // nothing was written
	}

	// Setup slave address
	I2caRegs.I2CSAR = iic_dev_addr;

	// Check if bus busy
	if (I2caRegs.I2CSTR.bit.BB == 1)
	{
		gmp_io_last_error = GMP_STAT_BUSY;
		return 0; // nothing was written
	}

	// Setup number of bytes to send MsgBuffer + Address
	I2caRegs.I2CCNT = len;

	// Write whole data to IIC buffer
	for (i = 0; i < len; i++)
		I2caRegs.I2CDXR = *(msg->MsgBuffer + i);

	// Send start as master transmitter
	I2caRegs.I2CMDR.all = 0x6E20;

	return len;


}
