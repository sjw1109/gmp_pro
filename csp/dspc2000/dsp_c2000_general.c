
#include <core/std/typedef.h>
#include <core/std/error_code.h>
// I2CA_Init - 
//
void
I2CA_Init(void)
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


// transmit via I2C port
gmp_stat_t gmp_port_i2c_tx(
	gmp_phy_mapping_t* handle,
	GMP_PORT_DATA_T* data,		// data to be transmitted
	GMP_PORT_SIZE_T size,		// size of data
	GMP_PORT_SIZE_T* ret_data	// return effective length
)
{
	int i;
	gmp_iic_mapping_t* hi2c = (gmp_iic_mapping_t*)handle;
	GMP_PORT_HIIC_T iic_reg = handle->peripheral->handle;

	// validate pointer parameters
	assert(handle);
	assert(data);
	//	ASSERT(ret_data);

	// default return
	if (ret_data) (*ret_data) = 0;


	// Wait until the STP bit is cleared from any previous master communication.
	// Clearing of this bit by the module is delayed until after the SCD bit is
	// set. If this bit is not checked prior to initiating a new message, the
	// I2C could get confused.
	if (iic_reg->I2CMDR.bit.STP == 1)
	{
		return GMP_STAT_NOT_READY;
	}

	// Setup slave address
	iic_reg->I2CSAR = hi2c->device_id;

	// Check if bus busy
	if (iic_reg->I2CSTR.bit.BB == 1)
	{
		return GMP_STAT_BUSY;
	}

	// Setup number of bytes to send MsgBuffer + Address
	iic_reg->I2CCNT = size;

	// Send all messages
	for (i = 0; i < size; i++)
		iic_reg->I2CDXR = *(data + i);

	if (ret_data) (*ret_data) = size;

	// Send start as master transmitter
	iic_reg->I2CMDR.all = 0x6E20;

	return GMP_STAT_OK;

}

gmp_stat_t gmp_port_i2c_rx(
	gmp_phy_mapping_t* handle,
	GMP_PORT_DATA_T* token,		// token of I2C device
	GMP_PORT_SIZE_T size,		// size of token
	GMP_PORT_DATA_T* data,		// data buffer for receiving
	GMP_PORT_SIZE_T cap,		// capacity of receiving buffer
	GMP_PORT_SIZE_T* ret_data	// return effective length of `data`
)
{
	gmp_iic_mapping_t* hi2c = (gmp_iic_mapping_t*)handle;
	GMP_PORT_HIIC_T iic_reg = handle->peripheral->handle;
	int i = 0;

	// parameters check
	assert(data);
	assert(handle);


	// Wait until the STP bit is cleared from any previous master communication.
	// Clearing of this bit by the module is delayed until after the SCD bit is
	// set. If this bit is not checked prior to initiating a new message, the
	// I2C could get confused.
	if (I2caRegs.I2CMDR.bit.STP == 1)
	{
		return GMP_STAT_NOT_READY;
	}

	// Setup slave address
	iic_reg->I2CSAR = hi2c->device_id;


	// Check if bus busy
	if (I2caRegs.I2CSTR.bit.BB == 1)
	{
		return GMP_STAT_BUSY;
	}

	// send address
	I2caRegs.I2CCNT = size;

	for (i = 0; i < size; i++)
		iic_reg->I2CDXR = *(token + i);

	// Send data to trans_setup EEPROM address
	I2caRegs.I2CMDR.all = 0x2620;

	// Wait until data has sent


	// receive messages
//	I2caRegs.I2CCNT = msg->NumOfBytes;	// Setup how many bytes to expect
	I2caRegs.I2CMDR.all = 0x2C20;       // Send restart as master receiver


	return I2C_SUCCESS;


}


__interrupt void
i2c_int1a_isr(void)
{

	uint16_t IntSource, i;

	// Read interrupt source
	IntSource = I2caRegs.I2CISRC.all;

	// Interrupt source = Register Access Ready
	// This interrupt is used to determine when the EEPROM address trans_setup 
	// portion of the read data communication is complete. Since no stop bit is 
	// commanded, this flag tells us when the message has been sent instead of 
	// the SCD flag. If a NACK is received, clear the NACK bit and command a 
	// stop. Otherwise, move on to the read data portion of the communication.

//	if (IntSource == I2C_ARDY_ISRC)
//	{
//		if (I2caRegs.I2CSTR.bit.NACK == 1)
//		{
//			I2caRegs.I2CMDR.bit.STP = 1;
//			I2caRegs.I2CSTR.all = I2C_CLR_NACK_BIT;
//		}
//		else if (CurrentMsgPtr->MsgStatus == I2C_MSGSTAT_SEND_NOSTOP_BUSY)
//		{
//			CurrentMsgPtr->MsgStatus = I2C_MSGSTAT_RESTART;
//		}
//	}

	// Enable future I2C (PIE Group 8) interrupts
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}

#define CPU_FREQ    90E6
#define LSPCLK_FREQ (CPU_FREQ/4)
#define SCI_FREQ    100E3
#define SCI_PRD     ((LSPCLK_FREQ/(SCI_FREQ*8))-1)

__interrupt void sciaRxFifoIsr(void);

void SCI_init(
	gmp_phy_mapping_t* handle
)
{
	assert(handle);

//	uint32_t timeout;
//	gmp_size_t i;					// loop variable

	GMP_PORT_HUART_T sci = handle->peripheral->handle;

	// 1 stop bit, No loopback, No parity, 8 char bits, idle-line protocol
	sci->SCICCR.all = 0x0007;


	// enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
	sci->SCICTL1.all = 0x0003;
	sci->SCICTL2.all = 0x0003;
	sci->SCICTL2.bit.TXINTENA = 0;
	sci->SCICTL2.bit.RXBKINTENA = 1;

	// baudrate config
	sci->SCIHBAUD = ((Uint16)SCI_PRD) >> 8;
	sci->SCILBAUD = SCI_PRD;

//	sci->SCIFFTX.all = 0xC022;
	sci->SCIFFRX.all = 0x0022;
	sci->SCIFFCT.all = 0x00;

	// Relinquish SCI from Reset
	sci->SCICTL1.all = 0x0023;     
	sci->SCIFFRX.bit.RXFIFORESET = 1;

	// For SCI-A
	EALLOW;  
	PieVectTable.SCIRXINTA = &sciaRxFifoIsr; // //SCIRXINTA
	EDIS;

	PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;     // PIE Group 9, INT1, SCIRXINTA
	IER = 0x100;                         // Enable CPU INT
	EINT;

	// For SCI-B
	//EALLOW;
	//PieVectTable.SCIRXINTB = &scibRxFifoIsr; //SCIRXINTB
	//EDIS;

	//PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
	//PieCtrlRegs.PIEIER9.bit.INTx3 = 1;    // PIE Group 9, INT3, SCIRXINTB
	//IER = 0x100;                          // Enable CPU INT
	//EINT;
}

// transmit via UART port
gmp_stat_t gmp_port_uart_tx(
	gmp_phy_mapping_t* handle,
	GMP_PORT_DATA_T* data,		// data to be transmitted
	GMP_PORT_SIZE_T size,		// size of data
	GMP_PORT_SIZE_T* ret_data	// return effective length
)
{
	// parameters check
	assert(data);
	assert(handle);

	uint32_t timeout;
	gmp_size_t i;					// loop variable

	GMP_PORT_HUART_T sci = handle->peripheral->handle;
	
	for (i = 0; i < size; ++i)
	{
		timeout = 0;
		while (sci->SCIFFTX.bit.TXFFST != 0)
		{
			timeout += 1;
			if (timeout > TIMEOUT_CNT)
			{
				if (ret_data) *ret_data = i;
				return GMP_STAT_BUSY;
			}
		}
		sci->SCITXBUF = data[i];
	}

	if (ret_data) *ret_data = size;
	return GMP_STAT_OK;

}

// receive via UART port
gmp_stat_t gmp_port_uart_rx(
	gmp_phy_mapping_t* handle,
	GMP_PORT_DATA_T* token,		// token of I2C device
	GMP_PORT_SIZE_T size,		// size of token
	GMP_PORT_DATA_T* data,		// data buffer for receiving
	GMP_PORT_SIZE_T cap,		// capacity of receiving buffer
	GMP_PORT_SIZE_T* ret_data	// return effective length
)
{
	// parameters check
	assert(data);
	assert(handle);

	uint32_t timeout;
	gmp_size_t i;					// loop variable

	GMP_PORT_HUART_T sci = handle->peripheral->handle;

	for (i = 0; i < cap; ++i)
	{
		// wait for RRDY/RXFFST =1 for 1 data available in FIFO
		while (sci->SCIFFRX.bit.RXFFST != 1)
		{
			timeout += 1;
			if (timeout > TIMEOUT_CNT)
			{
				if (ret_data) *ret_data = i;
				return GMP_STAT_RESPONSE_TIMEOUT;
			}
		}

		// Check received character
		data[i] = sci->SCIRXBUF.all;
	}

	if (ret_data) *ret_data = cap;
	return GMP_STAT_OK;
}

extern gmp_peripheral_t gmp_uart_dbg;

uint16_t rdataA[2];

__interrupt void
sciaRxFifoIsr(void)
{
	Uint16 i; // loop variables

	
	for (i = 0; i < 2; i++)
	{
		rdataA[i] = SciaRegs.SCIRXBUF.all;  // Read data
	}

	// call GMP interrupt response routine
	gmp_port_recv_int_response(
		&gmp_uart_dbg,
		rdataA, 2, 0
	);

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;   // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag

	PieCtrlRegs.PIEACK.all |= 0x100;       // Issue PIE ack
}


//transmit via SPI port
gmp_stat_t gmp_port_spi_tx(
	gmp_phy_mapping_t* handle,
	GMP_PORT_DATA_T* data,		// data to be transmitted
	GMP_PORT_SIZE_T size,		// size of data
	GMP_PORT_SIZE_T* ret_data	// return effective length
);

// receive via SPI port
gmp_stat_t gmp_port_spi_rx(
	gmp_phy_mapping_t* handle,
	GMP_PORT_DATA_T* token,		// token of I2C device
	GMP_PORT_SIZE_T size,		// size of token
	GMP_PORT_DATA_T* data,		// data buffer for receiving
	GMP_PORT_SIZE_T cap,		// capacity of receiving buffer
	GMP_PORT_SIZE_T* ret_data	// return effective length of `data`
);

//////////////////////////////////////////////////////////////////////////
// TIMER function

// get system default tick
GMP_PORT_TIME_T gmp_port_system_tick(
	void
)
{
	return 0;
}
