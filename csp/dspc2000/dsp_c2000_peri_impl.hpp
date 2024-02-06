
// base classes headers
#include <core/dev/peripheral_driver.hpp>


// User must config the compiler to satisfy the C11 standard
// Properties for the project, you should follow the prompt
// - Build
// - - C2000 Compiler
// - - - Advanced Options
// - - - - Language Options
// And you should pay attention to 
// - C Dialect   : Compile program in C99 mode (--c99)
// - C++ Dialect : Compile program in C++03 mode (--c++03)


#ifndef _FILE_C2000_PERIPHERAL_IMPLEMENT_HPP_
#define _FILE_C2000_PERIPHERAL_IMPLEMENT_HPP_

typedef struct _tag_dsp_c2000_gpio_dat_t
{
	uint32_t data;
	uint32_t set;
	uint32_t clear;
	uint32_t toggle;
}gpio_dat_t;

#ifndef GPIO_MAX_NUMBER
#define GPIO_MAX_NUMBER 32
#endif // GPIO_MAX_NUMBER

// This is the only entry point of GPIO Data Register pointer.
extern gpio_dat_t* gpio_dat_handle;

// This is the GPIO implementation of C2000 series DSP
class gmp_gpio_dspc2000_impl_t
	:public gmp_gpio_entity
{
public:
	typedef gmp_data_t data_type;

public:
	// ctor & dtor
	gmp_gpio_dspc2000_impl_t()
		:gpio_mux(0), gpio_pin(0)
	{
	}

	gmp_gpio_dspc2000_impl_t(uint16_t gpio_index)
	{
		assert(gpio_index < GPIO_MAX_NUMBER);

		gpio_mux = gpio_pin / GPIO_MAX_NUMBER;
		gpio_pin = gpio_pin % GPIO_MAX_NUMBER;
	}

public:
	void set_gpio_pin(uint16_t gpio_index)
	{
		assert(gpio_index < GPIO_MAX_NUMBER);

		gpio_mux = gpio_pin / GPIO_MAX_NUMBER;
		gpio_pin = gpio_pin % GPIO_MAX_NUMBER;
	}

	// utility function
	void set() override
	{
	    EALLOW;
		gpio_dat_handle[gpio_mux].set |= (uint32_t)1 << gpio_pin;
		EDIS;
	}
	void clear() override
	{
	    EALLOW;
		gpio_dat_handle[gpio_mux].clear |= (uint32_t)1 << gpio_pin;
		EDIS;

	}
	void toggle() override
	{
	    EALLOW;
		gpio_dat_handle[gpio_mux].toggle |= (uint32_t)1 << gpio_pin;
		EDIS;
	}

	// Write and Read function
	gmp_size_t write(const data_type data) override
	{
	    EALLOW;
		gpio_dat_handle[gpio_mux].toggle |= (uint32_t)1 << gpio_pin;
		EDIS;
	}

	data_type read() override
	{
	    uint32_t gpio_mask = (uint32_t)1 << gpio_pin;

		data_type gpio_status = (gpio_dat_handle[gpio_mux].data) & gpio_mask;
		if (gpio_status == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}


	// The following function is determined by chips, not implement
	void set_mode(gmp_element_t mode)override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}


public:
	uint16_t gpio_mux;
	uint16_t gpio_pin;

};

//////////////////////////////////////////////////////////////////////////

#define GMP_PORT_HUART_T struct SCI_REGS*


// implement the uart entity
class gmp_uart_dspc2000_impl_t
	:public gmp_uart_entity
{
public:
	typedef gmp_addr_t addr_type;
	typedef gmp_data_t data_type;

public:
	// ctor & dtor
	gmp_uart_dspc2000_impl_t(volatile struct SCI_REGS* handle)
		:handle(handle)
	{}

public:
	void init()
	{
		//
		// Note: Clocks were turned on to the SCI peripheral
		// in the InitSysCtrl() function
		//

		//
		// Initalize the SCI FIFO
		//
		handle->SCIFFTX.all = 0xE040;
		handle->SCIFFRX.all = 0x2044;
		handle->SCIFFCT.all = 0x0;

		//
		// 1 stop bit, No parity, No loopback,8 char bits, async mode,
		// idle-line protocol
		//
		handle->SCICCR.all = 0x0007;

		//
		// enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
		//
		handle->SCICTL1.all = 0x0003;
		handle->SCICTL2.all = 0x0003;
		handle->SCICTL2.bit.TXINTENA = 0;
		handle->SCICTL2.bit.RXBKINTENA = 0;
		handle->SCIHBAUD = 0x0000;
		handle->SCILBAUD = 0x032E; // Baudrate 115200 bps
		handle->SCICCR.bit.LOOPBKENA = 0; // Disable loop back
		handle->SCICTL1.all = 0x0023;     // Relinquish SCI from Reset
	}

	void set_baudrate(uint32_t BaudRate) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_stop_bit(gmp_element_t stopbit) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_data_bit(gmp_element_t databit) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

	void set_parity(gmp_element_t parity_check) override
	{
		// This function is not implemented here
		gmp_not_impl(__FILE__, __LINE__);
	}

public:
	// Basic IO functions is defined here.
	gmp_size_t read(gmp_data_t* data, gmp_size_t length) override
	{
		return 0;
	}

	gmp_size_t write(data_type* data, gmp_size_t length) override
	{
		for (int i= 0;i<length;++i)
		{
			transmit(data[i]);
		}
		return length;
	}

	gmp_size_t write(const gmp_data_t* data, gmp_size_t length) override
	{
		for (int i = 0; i < length; ++i)
		{
			transmit(data[i]);
		}
		return length;
	}

	gmp_data_t read() override
	{
		return 0;
	}

	gmp_size_t write(gmp_data_t data) override
	{
		transmit(data);
		return 1;
	}

public:
	inline gmp_stat_t transmit(gmp_data_t a)
	{
		handle->SCITXBUF = a;

		//
		// wait for RRDY/RXFFST =1 for 1 data available in FIFO
		//
		while (handle->SCIFFTX.bit.TXFFST == 1)
		{}

		return GMP_STAT_OK;
	}

protected:
	volatile struct SCI_REGS* handle;

};




#endif // _FILE_C2000_PERIPHERAL_IMPLEMENT_HPP_

