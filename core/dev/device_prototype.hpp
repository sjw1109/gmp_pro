//// This file provide a set of base classes.
//
//// TODO BUG REPORT: local variables is not inited.
//// must use `data_type result = 0;` instead `data_type result;`.
//
//#include <stdint.h>
//#include <assert.h>
//#include <string.h>
//
//
//#ifndef _FILE_DEVICE_PROTOTYPE_HPP_
//#define _FILE_DEVICE_PROTOTYPE_HPP_
//
//class iic_register_device_t
//	:public concept_wr_single_addr<addr_gt, uint32_t>
//{
//public:
//	// ctor & dtor
//	iic_register_device_t(gmp_iic_entity* iic,
//		addr_gt dev_addr,
//		fast_gt reg_size = 1
//	)
//		:iic(iic), dev_addr(dev_addr), reg_size(reg_size)
//	{
//		assert(iic);
//		assert(reg_size <= 4);
//	}
//
//public:
//	typedef uint32_t data_type;
//	typedef addr_gt addr_type;
//
//public:
//	data_type read(addr_type addr) override
//	{
//		data_type result = 0;
//
//		iic->read(dev_addr, addr, (data_gt*)&result, reg_size);
//		return result;
//	}
//
//	size_gt write(addr_type addr, data_type data) override
//	{
//		iic->write(dev_addr, addr, (data_gt*)&data, reg_size);
//		return reg_size;
//	}
//
//public:
//	gmp_iic_entity* iic;
//	addr_gt dev_addr;
//	addr_gt reg_size;
//};
//
//
//class iic_command_device_t
//	:public concept_rw_single<uint32_t>
//{
//public:
//	// ctor & dtor
//	iic_command_device_t(gmp_iic_entity* iic,
//		addr_gt dev_addr,
//		fast_gt command_size = 1)
//		: iic(iic), dev_addr(dev_addr), cmd_size(command_size)
//	{
//		assert(cmd_size <= 4);
//	}
//
//public:
//	typedef uint32_t data_type;
//	typedef addr_gt addr_type;
//
//public:
//	data_type read() override
//	{
//		data_type result = 0;
//
//		iic->read(dev_addr, (data_gt*)&result, cmd_size);
//		return result;
//	}
//
//	size_gt write( data_type data) override
//	{
//		iic->write(dev_addr, (data_gt*)&data, cmd_size);
//		return cmd_size;
//	}
//
//public:
//	gmp_iic_entity* iic;
//	addr_gt dev_addr;
//	addr_gt cmd_size;
//};
//
//class iic_storage_device_t
//	:virtual public concept_wr_single_addr<uint32_t, uint32_t>,
//	virtual public concept_rw_with_addr<uint32_t,data_gt>
//{
//public:
//	// ctor & dtor
//	iic_storage_device_t(gmp_iic_entity* iic,
//		uint16_t dev_base_addr,
//		fast_gt reg_size = 1)
//		:iic(iic), dev_base_addr(dev_base_addr), dev_addr(dev_base_addr), reg_size(reg_size)
//	{
//		assert(iic);
//		assert(reg_size <= 4);
//	}
//
//public:
//	typedef uint32_t addr_type;
//	typedef uint32_t cell_type;
//	typedef data_gt data_type;
//
//public:
//	cell_type read(addr_type addr) override
//	{
//		data_type result = 0;
//
//		iic->read(dev_addr, addr, (data_gt*)&result, reg_size);
//		return result;
//	}
//
//	size_gt write(addr_type addr, cell_type data) override
//	{
//		return iic->write(dev_addr, addr, (data_gt*)&data, reg_size);
//	}
//
//	size_gt read(addr_type addr, data_type* data, size_gt length) override
//	{
//		return iic->read(dev_addr, addr, (data_gt*)data, length);
//	}
//
//	size_gt write(addr_type addr, data_type* data, size_gt length) override
//	{
//		return iic->write(dev_addr, addr, (data_gt*)data, length);
//	}
//
//
//
//public:
//	gmp_iic_entity* iic;
//	addr_gt dev_base_addr;
//	addr_gt dev_addr;
//	addr_gt reg_size;
//
//};
//
//// NOTE this class is now for frame_size = 8;
//class spi_register_device_t
//	:public concept_rw_with_addr<uint32_t, data_gt>
//{
//public:
//	// ctor & dtor
//	spi_register_device_t(gmp_spi_entity* spi,
//		fast_gt reg_addr_size = 1)
//		:spi(spi), reg_addr_size(reg_addr_size)
//	{
//		assert(reg_addr_size <= 4);
//	}
//
//public:
//	typedef data_gt data_type;
//	typedef uint32_t addr_type;
//
//public:
//	size_gt read(addr_type addr, data_type* data, size_gt length)	override
//	{
//		fast_gt addr_size = reg_addr_size;
//
//		while (addr_size--)
//		{
//			spi->write((addr >> addr_size) & 0xFF);
//		}
//
//		if (last_error == GMP_STAT_OK)
//			return spi->read((gmp_spi_entity::data_type*)data, length);
//		else
//			return 0;
//	}
//
//	size_gt write(addr_type addr, data_type* data, size_gt length) override
//	{
//		fast_gt addr_size = reg_addr_size;
//		fast_gt data_buf[16];
//		size_gt buf_pos = 0;
//
//		memset(data_buf, 0, 16);
//
//		while (addr_size--)
//		{
//			data_buf[buf_pos++] = (addr >> addr_size) & 0xFF;
//			//			spi->write((addr >> addr_size) & 0xFF);
//		}
//
//		if (length < 16U - buf_pos)
//		{
//			memcpy(&data_buf[buf_pos], data, length);
//			buf_pos += length;
//			return spi->write((gmp_spi_entity::data_type*)data_buf, buf_pos);
//		}
//		else
//		{
//			spi->write((gmp_spi_entity::data_type*)data_buf, buf_pos);
//		}
//
//
//		if (last_error == GMP_STAT_OK)
//			return spi->write((gmp_spi_entity::data_type*)data, length);
//		else
//			return 0;
//	}
//
//public:
//	gmp_spi_entity* spi;
//	fast_gt reg_addr_size;
//};
//
//class rs485
//	: public virtual gmp_concept_rw_direct,
//	public virtual gmp_concept_write_direct_const,
//	public virtual gmp_concept_rw_single
//{
//public:
//	// ctor & dtor
//	rs485()
//	{}
//
//public:
//	gmp_uart_entity uart;
//	gmp_gpio_entity dir;
//};
//
//
//#endif
//
