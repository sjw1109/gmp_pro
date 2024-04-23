//// This file provide a set of peripheral prototypes.
//// All the peripherals which are supported by GMP is listed in this file.
//
////#include <core/dev/io_concept.hpp>
//
//#ifndef _FILE_PERIPHERAL_PROTOTYPE_HPP_
//#define _FILE_PERIPHERAL_PROTOTYPE_HPP_
//
//// GMP device prototype type
//typedef enum
//{
//	PROTO_PERIPHERAL_NUL = 0,
//	PROTO_PERIPHERAL_IIC,
//	PROTO_PERIPHERAL_UART,
//	PROTO_PERIPHERAL_SPI,
//	PROTO_PERIPHERAL_IIC_SLAVE,
//	PROTO_PERIPHERAL_IIS,
//	PROTO_PERIPHERAL_ADC,
//	PROTO_PERIPHERAL_FINAL
//}gmp_prototype_t;
//
//
////////////////////////////////////////////////////////////////////////////
//void gmp_port_gpio_write();
//
//// entity of the GPIO device
//class gmp_gpio_entity
//	:public virtual gmp_concept_rw_single
//{
//public:
//	//ctor & dtor
//	gmp_gpio_entity()
//	{}
//
//public:
//	typedef data_gt data_type;
//
//public:
//	// utility function
//	virtual void set() = 0;
//	virtual void clear() = 0;
//	virtual void toggle() = 0;
//	virtual void set_mode(fast_gt mode) = 0;
//
//	virtual size_gt write(data_type data) = 0;
//	virtual data_type read() = 0;
//
//
//public:
//	// The following parameters are for write function and read function.
//	static constexpr fast_gt pin_reset = 0;
//	static constexpr fast_gt pin_set = 1;
//
//	// The following parameters are for set_mode function
//	static constexpr fast_gt pin_input = 0;
//	static constexpr fast_gt pin_output = 1;
//};
//
////////////////////////////////////////////////////////////////////////////
//// 
//// entity of the UART device
//class gmp_uart_entity
//	:public virtual gmp_concept_rw_direct,
//	public virtual gmp_concept_write_direct_const,
//	public virtual gmp_concept_rw_single
//{
//public:
//	// ctor & dtor
//	gmp_uart_entity()
//		: last_error(GMP_STAT_OK)
//	{}
//
//public:
//	typedef data_gt data_type;
//
//public:
//	// utility function
//	virtual void set_baudrate(uint32_t BaudRate) = 0;
//
//	virtual void set_stop_bit(fast_gt stopbit) = 0;
//
//	virtual void set_data_bit(fast_gt databit) = 0;
//
//	virtual void set_parity(fast_gt parity_check) = 0;
//
//	virtual size_gt read(data_type* data, size_gt length) = 0;
//
//	virtual size_gt write(const data_type* data, size_gt length) = 0;
//
//	virtual size_gt write(data_type* data, size_gt length) = 0;
//
//	virtual  size_gt write(data_type data) = 0;
//
//	virtual data_type read() = 0;
//
//
//public:
//	// last error code
//	gmp_stat_t last_error;
//
//public:
//	// constant
//
//	// The following parameters are to set UART stop bits 
//	static constexpr fast_gt stop_bit_1 = 0;
//	static constexpr fast_gt stop_bit_1_5 = 1;
//	static constexpr fast_gt stop_bit_2 = 2;
//
//	// The following parameters are to set UART data bits
//	static constexpr fast_gt data_bit_5 = 5;
//	static constexpr fast_gt data_bit_6 = 6;
//	static constexpr fast_gt data_bit_7 = 7;
//	static constexpr fast_gt data_bit_8 = 8;
//
//	// The following parameters are to set UART parity check
//	static constexpr fast_gt parity_none = 0;
//	static constexpr fast_gt parity_even = 1;
//	static constexpr fast_gt parity_odd = 2;
//};
//
////////////////////////////////////////////////////////////////////////////
//// 
//// entity of the IIC device
//class gmp_iic_entity
//	:public virtual gmp_concept_rw_with_dualaddr,  // This is for memory operation
//	public virtual gmp_concept_rw_with_addr		  // This is for direct operation
//{
//public:
//	// ctor & dtor
//	gmp_iic_entity()
//		:reg_addr_len(reg_addr_len_1), last_error(GMP_STAT_OK)
//	{}
//
//public:
//	typedef data_gt data_type;
//	typedef addr_gt addr_type;
//	typedef addr_type addr1_type;
//	typedef addr_type addr2_type;
//
//public:
//	// utility function
//	virtual void set_speed(uint32_t speed) = 0;
//
//	// Select 7-bit or 11-bit
//	virtual void set_device_addr_length(uint32_t addr_length) = 0;
//
//	virtual size_gt write(addr1_type addr1, addr2_type addr2, data_type* data, size_gt length) = 0;
//
//	virtual size_gt read(addr1_type addr1, addr2_type addr2, data_type* data, size_gt length) = 0;
//
//	virtual size_gt write(addr_type addr, data_type* data, size_gt length) = 0;
//
//	virtual size_gt read(addr_type addr, data_type* data, size_gt length) = 0;
//
//public:
//	// Set register memory address length, unit Byte
//	inline void set_reg_addr_length(fast_gt length)
//	{
//		reg_addr_len = length;
//	}
//
//	// get register memory address length, unit Byte
//	inline fast_gt get_reg_addr_length()
//	{
//		return reg_addr_len;
//	}
//
//protected:
//	// reg or memory address length
//	fast_gt reg_addr_len;
//
//public:
//	// last error
//	gmp_stat_t last_error;
//
//public:
//	// The following parameters are to set IIC register or memory address length.
//	// unit: Byte
//	static constexpr fast_gt reg_addr_len_1 = 1; // (default)
//	static constexpr fast_gt reg_addr_len_2 = 2;
//	static constexpr fast_gt reg_addr_len_3 = 3;
//	static constexpr fast_gt reg_addr_len_4 = 4;
//
//	// The following parameters are to set IIC slave address length.
//	// unit: Bit
//	static constexpr fast_gt dev_addr_length_7 = 1; // (default)
//	static constexpr fast_gt dev_addr_length_11 = 2;
//};
//
////////////////////////////////////////////////////////////////////////////
//// 
//// entity of the SPI device
//class gmp_spi_entity
//	: public virtual concept_rwd_single<uint16_t>,
//	public virtual concept_rwd_direct<uint16_t>
//{
//public:
//	// ctor & dtor
//	gmp_spi_entity()
//		:frame_size(8), last_error(GMP_STAT_OK)
//	{}
//
//public:
//	typedef uint16_t data_type;
//
//public:
//	virtual void set_clk_div(uint32_t div) = 0;
//
//	virtual void set_mode(fast_gt polarity) = 0;
//
//	virtual void set_bit_order(fast_gt order) = 0;
//
//	// from 4 bits to 16 bits
//	virtual void set_frame_size(fast_gt size)
//	{
//		frame_size = size;
//		// The rest config of SPI peripheral need to be complete by user.
//	}
//
//	virtual data_type readwrite(data_type data) = 0;
//	virtual size_gt write(data_type data) = 0;
//	virtual data_type read() = 0;
//	virtual size_gt readwrite(data_type* data_in, data_type* data_out, size_gt length) = 0;
//	virtual size_gt read(data_type* data, size_gt length) = 0;
//	virtual size_gt write(data_type* data, size_gt length) = 0;
//
//
//public:
//	// last error
//	gmp_stat_t last_error;
//	gmp_stat_t frame_size;
//
//public:
//	// The following parameters are used to set SPI mode.
//	// The SPI mode is decided by CPOL & CPHA
//	static constexpr fast_gt mode_0 = 0;
//	static constexpr fast_gt mode_1 = 1;
//	static constexpr fast_gt mode_2 = 2;
//	static constexpr fast_gt mode_3 = 3;
//
//	// The following parameters are used to set SPI bit order
//	static constexpr fast_gt order_lsb = 0;
//	static constexpr fast_gt order_msb = 1;
//};
//
////////////////////////////////////////////////////////////////////////////
//// 
//// Provide a abstract class of Timer
//
//class gmp_timer_entity
//{
//public:
//	// ctor & dtor
//
//	// Use default timer base frequency
//	gmp_timer_entity()
//		: base_freq(GMP_TIMER_BASE_FREQ)
//	{}
//
//	// specify the timer base frequency
//	gmp_timer_entity(time_gt base_freq)
//		: base_freq(base_freq)
//	{}
//
//public:
//	// configure
//
//	// Set frequency of timer
//	virtual void set_freq(time_gt freq_tick);
//
//	// Get frequency of timer, unit tick
//	virtual time_gt get_freq();
//
//	// Enable modulator output
//	virtual void enable();
//
//	// Disable modulator output
//	virtual void disable();
//
//	// utilities
//	// transfer frequency to tick
//	inline time_gt freq2tick(time_gt freq)
//	{
//		return base_freq / freq;
//	}
//
//public:
//	// The base frequency of timer.
//	time_gt base_freq;
//
//};
//
//// entity of the modulator device
//// It's a PWM modulator class prototype.
//// The following 3 classes, is different number of channels.
//
//// This class has only one channel
//class gmp_modulator_entity_1ch
//{
//public:
//	// ctor & dtor
//	gmp_modulator_entity_1ch()
//		: base_freq(GMP_TIMER_BASE_FREQ)
//	{}
//
//	gmp_modulator_entity_1ch(time_gt base_freq)
//		: base_freq(base_freq)
//	{}
//
//public:
//	// utilities function 
//
//	// Set modulator compare value
//	virtual void set_modulator_cmp(time_gt cmp_tick);
//
//	// Set frequency of timer
//	virtual void set_modulator_freq(time_gt freq_tick);
//
//	// Get frequency of timer, unit tick
//	virtual time_gt get_modulator_freq();
//
//	// Enable modulator output
//	virtual void enable();
//
//	// Disable modulator output
//	virtual void disable();
//
//public:
//	// The following parameter(s) is/are for controller
//
//	// The frequency of the timer or PWM peripheral
//	time_gt base_freq;
//
//	// How many ticks do the timer or PWM peripheral has.
////	time_gt freq_tick;
//
//};
//
//// This class has 2 channels
//class gmp_modulator_entity_2ch
//{
//public:
//	// ctor & dtor
//	gmp_modulator_entity_2ch()
//	{}
//
//public:
//	// utilities function 
//
//	// Set modulator compare value
//	virtual void set_modulator_cmp(time_gt cmp_tick1,
//		time_gt gmp_tick2);
//
//	// Set frequency of timer
//	// Default implement
//	virtual void set_modulator_freq(time_gt freq_tick);
//
//	// Enable modulator output
//	// This is a default implement
//	virtual void enable();
//
//	// Disable modulator output
//	// This is a default implement
//	virtual void disable();
//
//};
//
//// This class has 3 channels
//class gmp_modulator_entity_3ch
//{
//public:
//	// ctor & dtor
//	gmp_modulator_entity_3ch()
//	{}
//
//public:
//	// utilities function 
//
//	// Set modulator compare value
//	virtual void set_modulator_cmp(time_gt cmp_tick1,
//		time_gt gmp_tick2, time_gt gmp_tick3);
//
//	// Set frequency of timer
//	// Default implement
//	virtual void set_modulator_freq(time_gt freq_tick);
//
//	// Enable modulator output
//	// This is a default implement
//	virtual void enable();
//
//	// Disable modulator output
//	// This is a default implement
//	virtual void disable();
//
//};
//
//
////////////////////////////////////////////////////////////////////////////
//// 
//// entity of the CAN controller device
//class gmp_can_entity
//	:public virtual concept_rw_with_addr<uint16_t, data_gt>,   // For standard frame
//	public virtual concept_rw_with_dualaddr<uint16_t, uint16_t, data_gt> // For each frame type
//{
//public:
//	// ctor & dtor
//	typedef uint16_t addr1_type;
//	typedef uint16_t addr2_type;
//	typedef addr2_type addr_type;
//	typedef data_gt data_type;
//
//public:
//	// utilities function
//
//	// In general, this function only send a standard data frame.
//	virtual size_gt write(addr_type addr, data_type* data, size_gt length) = 0;
//	virtual size_gt read(addr_type addr, data_type* data, size_gt length) = 0;
//
//	// The first address is the frame type:
//	// standard frame or extension frame; data frame or remote frame
//	virtual size_gt write(addr1_type frame_type, addr2_type addr, data_type* data, size_gt length) = 0;
//	virtual size_gt read(addr1_type frame_type, addr2_type addr, data_type* data, size_gt length) = 0;
//
//
//public:
//
//};
//
//#endif // _FILE_PERIPHERAL_PROTOTYPE_HPP_
