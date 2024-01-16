// 

// TODO BUG REPORT: local variables is not inited.
// must use `data_type result = 0;` instead `data_type result;`.

#include <stdint.h>
#include <assert.h>
#include <string.h>


#ifndef _FILE_DEVICE_PROTOTYPE_HPP_
#define _FILE_DEVICE_PROTOTYPE_HPP_

class iic_register_device_t
	:public concept_wr_single_addr<gmp_addr_t, uint32_t>
{
public:
	// ctor & dtor
	iic_register_device_t(gmp_iic_entity* iic,
		gmp_addr_t dev_addr,
		uint8_t reg_size = 1
	)
		:iic(iic), dev_addr(dev_addr), reg_size(reg_size)
	{
		assert(iic);
		assert(reg_size <= 4);
	}

public:
	typedef uint32_t data_type;
	typedef gmp_addr_t addr_type;

public:
	data_type read(addr_type addr) override
	{
		data_type result;

		iic->read(dev_addr, addr, (gmp_data_t*)&result, reg_size);
		return result;
	}

	gmp_size_t write(addr_type addr, const data_type data) override
	{
		iic->write(dev_addr, addr, (const gmp_data_t*)&data, reg_size);
		return reg_size;
	}

public:
	gmp_iic_entity* iic;
	gmp_addr_t dev_addr;
	gmp_addr_t reg_size;
};


class iic_command_device_t
	:public concept_rw_single<uint32_t>
{
public:
	// ctor & dtor
	iic_command_device_t(gmp_iic_entity* iic,
		gmp_addr_t dev_addr,
		uint8_t command_size = 1)
		: iic(iic), dev_addr(dev_addr), cmd_size(command_size)
	{
		assert(cmd_size <= 4);
	}

public:
	typedef uint32_t data_type;
	typedef gmp_addr_t addr_type;

public:
	data_type read() override
	{
		data_type result;

		iic->read(dev_addr, (gmp_data_t*)&result, cmd_size);
		return result;
	}

	gmp_size_t write(const data_type data) override
	{
		iic->write(dev_addr, (const gmp_data_t*)&data, cmd_size);
		return cmd_size;
	}

public:
	gmp_iic_entity* iic;
	gmp_addr_t dev_addr;
	gmp_addr_t cmd_size;


};

class iic_storage_device_t
	:virtual public concept_wr_single_addr<gmp_addr_t, uint32_t>,
	virtual public gmp_concept_rw_with_addr
{
public:
	// ctor & dtor
	iic_storage_device_t(gmp_iic_entity* iic,
		uint16_t dev_base_addr,
		uint8_t reg_size = 1)
		:iic(iic), dev_base_addr(dev_base_addr), dev_addr(dev_base_addr), reg_size(reg_size)
	{
		assert(iic);
		assert(reg_size <= 4);
	}

public:
	typedef gmp_addr_t addr_type;
	typedef uint32_t cell_type;
	typedef gmp_data_t data_type;

public:
	cell_type read(addr_type addr) override
	{
		data_type result;

		iic->read(dev_addr, addr, (gmp_data_t*)&result, reg_size);
		return result;
	}

	gmp_size_t write(addr_type addr, const cell_type data) override
	{
		return iic->write(dev_addr, addr, (const gmp_data_t*)&data, reg_size);
	}

	gmp_size_t read(addr_type addr, data_type* data, gmp_size_t length) override
	{
		return iic->read(dev_addr, addr, (gmp_data_t*)data, length);
	}

	gmp_size_t write(addr_type addr, const data_type* data, gmp_size_t length) override
	{
		return iic->write(dev_addr, addr, (gmp_data_t*)data, length);
	}



public:
	gmp_iic_entity* iic;
	gmp_addr_t dev_base_addr;
	gmp_addr_t dev_addr;
	gmp_addr_t reg_size;

};

// NOTE this class is now for frame_size = 8;
class spi_register_device_t
	:public concept_rw_with_addr<uint32_t, gmp_data_t>
{
public:
	// ctor & dtor
	spi_register_device_t(gmp_spi_entity* spi,
		uint8_t reg_addr_size = 1)
		:spi(spi), reg_addr_size(reg_addr_size)
	{
		assert(reg_addr_size <= 4);
	}

public:
	typedef gmp_data_t data_type;
	typedef uint32_t addr_type;

public:
	gmp_size_t read(addr_type addr, data_type* data, gmp_size_t length)	override
	{
		uint8_t addr_size = reg_addr_size;

		while (addr_size--)
		{
			spi->write((addr >> addr_size) & 0xFF);
		}

		if (last_error == GMP_STAT_OK)
			return spi->read((gmp_spi_entity::data_type*)data, length);
		else
			return 0;
	}

	gmp_size_t write(addr_type addr, const data_type* data, gmp_size_t length) override
	{
		uint8_t addr_size = reg_addr_size;
		uint8_t data_buf[16];
		uint8_t buf_pos = 0;

		memset(data_buf, 0, 16);

		while (addr_size--)
		{
			data_buf[buf_pos++] = (addr >> addr_size) & 0xFF;
			//			spi->write((addr >> addr_size) & 0xFF);
		}

		if (length < 16 - buf_pos)
		{
			memcpy(&data_buf[buf_pos], data, length);
			buf_pos += length;
			return spi->write((const gmp_spi_entity::data_type*)data_buf, buf_pos);
		}
		else
		{
			spi->write((const gmp_spi_entity::data_type*)data_buf, buf_pos);
		}


		if (last_error == GMP_STAT_OK)
			return spi->write((const gmp_spi_entity::data_type*)data, length);
		else
			return 0;
	}

public:
	gmp_spi_entity* spi;
	uint8_t reg_addr_size;
};


#endif

