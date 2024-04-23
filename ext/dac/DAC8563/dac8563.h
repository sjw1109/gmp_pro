// This file is for DAC8563 DAC chip

#include <core/dev/device_prototype.hpp>

#ifndef _FILE_DAC8563_H_
#define _FILE_DAC8563_H_

// Please disable the NSS port of the 
class dac8563
	:spi_register_device_t
{
public:
	// ctor & dtor
	dac8563(gmp_spi_entity* spi,
		gmp_gpio_entity* clr,
		gmp_gpio_entity* load,
		gmp_gpio_entity* sync)
		:spi_register_device_t(spi),
		clr(clr), sync(sync), load(load)
	{}


public:
	void write_channel_1(uint16_t data)
	{
		//		sync->set();
				// 		ch1_data = data;
		ch1_data = ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);


		//		for (int i = 0; i < 10000; ++i);
		sync->clear();

		write(reg_addr_ch1_data, (data_gt*)&ch1_data, 2);

		load->clear();
		for (int i = 0; i < 10000; ++i);
		load->set();

// 		uint16_t command = 0;
// 		write(0x0F, (data_gt*)&command, 2);

		sync->set();
	}

	void write_channel_2(uint16_t data)
	{
		//		sync->set();
		// 		ch2_data = data;
		ch2_data = ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);


		//		for (int i = 0; i < 10000; ++i);
		sync->clear();

		write(reg_addr_ch2_data, (data_gt*)&ch2_data, 2);

		load->clear();
		for (int i = 0; i < 10000; ++i);
		load->set();

// 		uint16_t command = 0;
// 		write(0x0F, (data_gt*)&command, 2);


		sync->set();
	}

	void write_dual_channel(uint16_t data1, uint16_t data2)
	{
		//		ch1_data = data1;
		ch1_data = ((data1 & 0xFF) << 8) | ((data1 & 0xFF00) >> 8);
		// 		ch2_data = data2;
		ch2_data = ((data2 & 0xFF) << 8) | ((data2 & 0xFF00) >> 8);


		for (int i = 0; i < 10000; ++i);
		sync->clear();

		write(reg_addr_ch1_data, (data_gt*)&ch1_data, 2);
		write(reg_addr_ch2_data, (data_gt*)&ch2_data, 2);

		load->clear();
		for (int i = 0; i < 10000; ++i);
		load->set();

// 		uint16_t command = 0;
// 		write(0x0F, (data_gt*)&command, 2);

		sync->set();
	}

	void clear()
	{
		clr->clear();
		for (int i = 0; i < 10000; ++i);
		clr->set();
		for (int i = 0; i < 10000; ++i);
		clr->clear();

		sync->set();

	}

	void setup()
	{
		load->set();

		sync->clear();

		uint16_t command = 0x0100;
		write(0x28, (data_gt*)&command, 2);
		command = 0x0300;
		write(0x20, (data_gt*)&command, 2);
		command = 0x0000;
		write(0x30, (data_gt*)&command, 2); // Enable LD
		command = 0x0100;
		write(0x38, (data_gt*)&command, 2);


		// 		uint16_t command = 0x0100;
		// 		write(0x28, (data_gt*)&command, 2);
		// 		command = 0x0300;
		// 		write(0x20, (data_gt*)&command, 2);
		// 		command = 0x0300;
		// 		write(0x30, (data_gt*)&command, 2); // Enable LD
		// 		command = 0x0100;
		// 		write(0x38, (data_gt*)&command, 2);

		//		sync->set();

		// 		load->clear();
		// 
		// 
		// 		ch1_data = 0xFFFF;
		// 
		// 		write(reg_addr_ch1_data, (data_gt*)&ch1_data, 2);
		// 		
		// 		for (int i = 0; i < 10000; ++i);
		//		load->set();

		// 		command = 0;
		// 		write(0x0F, (data_gt*)&command, 2);


		sync->set();

	}

protected:
	gmp_gpio_entity* clr;
	gmp_gpio_entity* load;
	gmp_gpio_entity* sync;  // Chip Select

public:
	uint32_t ch1_data;
	uint32_t ch2_data;

public:
	static constexpr addr_type reg_addr_ch1_data = 0x18;
	static constexpr addr_type reg_addr_ch2_data = 0x19;
};

#endif // _FILE_DAC8563_H_
