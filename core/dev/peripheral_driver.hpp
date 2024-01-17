// This file provide a set of peripheral prototypes.
// All the peripherals which are supported by GMP is listed in this file.

//#include <core/dev/io_concept.hpp>

#ifndef _FILE_PERIPHERAL_PROTOTYPE_HPP_
#define _FILE_PERIPHERAL_PROTOTYPE_HPP_

// GMP device prototype type
typedef enum
{
	PROTO_PERIPHERAL_NUL = 0,
	PROTO_PERIPHERAL_IIC,
	PROTO_PERIPHERAL_UART,
	PROTO_PERIPHERAL_SPI,
	PROTO_PERIPHERAL_IIC_SLAVE,
	PROTO_PERIPHERAL_IIS,
	PROTO_PERIPHERAL_ADC,
	PROTO_PERIPHERAL_FINAL
}gmp_prototype_t;



void gmp_port_gpio_write();

// entity of the GPIO device
class gmp_gpio_entity
	:public virtual gmp_concept_rw_single
{
public:
	//ctor & dtor
	gmp_gpio_entity()
	{}

public:
	typedef gmp_data_t data_type;

public:
	// utility function
	virtual void set() = 0;
	virtual void clear() = 0;
	virtual void toggle() = 0;
	virtual void set_mode(uint8_t mode) = 0;

	virtual gmp_size_t write(data_type data) = 0;
	virtual data_type read() = 0;


public:
	// The following parameters are for write function and read function.
	static constexpr uint8_t pin_reset = 0;
	static constexpr uint8_t pin_set = 1;

	// The following parameters are for set_mode function
	static constexpr uint8_t pin_input = 0;
	static constexpr uint8_t pin_output = 1;
};

// entity of the UART device
class gmp_uart_entity
	:public virtual gmp_concept_rw_direct,
	public virtual gmp_concept_write_direct_const,
	public virtual gmp_concept_rw_single
{
public:
	// ctor & dtor
	gmp_uart_entity()
		: last_error(GMP_STAT_OK)
	{}

public:
	typedef gmp_data_t data_type;

public:
	// utility function
	virtual void set_baudrate(uint32_t BaudRate) = 0;

	virtual void set_stop_bit(uint8_t stopbit) = 0;

	virtual void set_data_bit(uint8_t databit) = 0;

	virtual void set_parity(uint8_t parity_check) = 0;

	virtual gmp_size_t read(data_type* data, gmp_size_t length) = 0;

	virtual gmp_size_t write(const data_type* data, gmp_size_t length) = 0;

	virtual gmp_size_t write( data_type* data, gmp_size_t length) = 0;

	virtual  gmp_size_t write(data_type data) = 0;

	virtual data_type read() = 0;


public:
	// last error code
	gmp_stat_t last_error;

public:
	// constant

	// The following parameters are to set UART stop bits 
	static constexpr uint8_t stop_bit_1 = 0;
	static constexpr uint8_t stop_bit_1_5 = 1;
	static constexpr uint8_t stop_bit_2 = 2;

	// The following parameters are to set UART data bits
	static constexpr uint8_t data_bit_5 = 5;
	static constexpr uint8_t data_bit_6 = 6;
	static constexpr uint8_t data_bit_7 = 7;
	static constexpr uint8_t data_bit_8 = 8;

	// The following parameters are to set UART parity check
	static constexpr uint8_t parity_none = 0;
	static constexpr uint8_t parity_even = 1;
	static constexpr uint8_t parity_odd = 2;
};


// entity of the IIC device
class gmp_iic_entity
	:public virtual gmp_concept_rw_with_dualaddr,  // This is for memory operation
	public virtual gmp_concept_rw_with_addr		  // This is for direct operation
{
public:
	// ctor & dtor
	gmp_iic_entity()
		:reg_addr_len(reg_addr_len_1), last_error(GMP_STAT_OK)
	{}

public:
	typedef gmp_data_t data_type;
	typedef gmp_addr_t addr_type;
	typedef addr_type addr1_type;
	typedef addr_type addr2_type;

public:
	// utility function
	virtual void set_speed(uint32_t speed) = 0;

	// Select 7-bit or 11-bit
	virtual void set_device_addr_length(uint32_t addr_length) = 0;

	virtual gmp_size_t write(addr1_type addr1, addr2_type addr2, data_type* data, gmp_size_t length) = 0;

	virtual gmp_size_t read(addr1_type addr1, addr2_type addr2, data_type* data, gmp_size_t length) = 0;

	virtual gmp_size_t write(addr_type addr, data_type* data, gmp_size_t length) = 0;

	virtual gmp_size_t read(addr_type addr, data_type* data, gmp_size_t length) = 0;

public:
	// Set register memory address length, unit Byte
	inline void set_reg_addr_length(uint8_t length)
	{
		reg_addr_len = length;
	}

	// get register memory address length, unit Byte
	inline uint8_t get_reg_addr_length()
	{
		return reg_addr_len;
	}

protected:
	// reg or memory address length
	uint8_t reg_addr_len;

public:
	// last error
	gmp_stat_t last_error;

public:
	// The following parameters are to set IIC register or memory address length.
	// unit: Byte
	static constexpr uint8_t reg_addr_len_1 = 1; // (default)
	static constexpr uint8_t reg_addr_len_2 = 2;
	static constexpr uint8_t reg_addr_len_3 = 3;
	static constexpr uint8_t reg_addr_len_4 = 4;

	// The following parameters are to set IIC slave address length.
	// unit: Bit
	static constexpr uint8_t dev_addr_length_7 = 1; // (default)
	static constexpr uint8_t dev_addr_length_11 = 2;
};

// entity of the SPI device
class gmp_spi_entity
	: public virtual concept_rwd_single<uint16_t>,
	public virtual concept_rwd_direct<uint16_t>
{
public:
	// ctor & dtor
	gmp_spi_entity()
		:frame_size(8), last_error(GMP_STAT_OK)
	{}

public:
	typedef uint16_t data_type;

public:
	virtual void set_clk_div(uint32_t div) = 0;

	virtual void set_mode(uint8_t polarity) = 0;

	virtual void set_bit_order(uint8_t order) = 0;

	// from 4 bits to 16 bits
	virtual void set_frame_size(uint8_t size)
	{
		frame_size = size;
		// The rest config of SPI peripheral need to be complete by user.
	}

	virtual data_type readwrite(data_type data) = 0;
	virtual gmp_size_t write(data_type data) = 0;
	virtual data_type read() = 0;
	virtual gmp_size_t readwrite(data_type* data_in, data_type* data_out, gmp_size_t length) = 0;
	virtual gmp_size_t read(data_type* data, gmp_size_t length) = 0;
	virtual gmp_size_t write(data_type* data, gmp_size_t length) = 0;


public:
	// last error
	gmp_stat_t last_error;
	gmp_stat_t frame_size;

public:
	// The following parameters are used to set SPI mode.
	// The SPI mode is decided by CPOL & CPHA
	static constexpr uint8_t mode_0 = 0;
	static constexpr uint8_t mode_1 = 1;
	static constexpr uint8_t mode_2 = 2;
	static constexpr uint8_t mode_3 = 3;

	// The following parameters are used to set SPI bit order
	static constexpr uint8_t order_lsb = 0;
	static constexpr uint8_t order_msb = 1;
};

// entity of the modulator device
// It's a PWM modulator class prototype.
// The following 4 classes, is different number of channels.

// This class has only one channel
class gmp_modulator_entity_1ch
{
public:
	// ctor & dtor
	gmp_modulator_entity_1ch()
	{}

public:
	// utilities function 

	// Set modulator compare value
	virtual void set_modulator_cmp(gmp_timer_cmp_t cmp_value);

	// Set frequency of timer
	virtual void set_modulator_freq(gmp_timer_cmp_t freq_value);

	// Enable modulator output
	virtual void enable();

	// Disable modulator output
	virtual void disable();

};

// This class has 2 channels
class gmp_modulator_entity_2ch
{
public:
	// ctor & dtor
	gmp_modulator_entity_2ch()
	{}

public:
	// utilities function

	// Set modulator compare value
	virtual void set_modulator_cmp(
		gmp_timer_cmp_t cmp1_value, gmp_timer_cmp_t cmp2_value
	);

	// Set frequency of timer
	virtual void set_modulator_freq(gmp_timer_cmp_t freq_value);

	// Enable modulator output
	virtual void enable();

	// Disable modulator output
	virtual void disable();
};

// This class has 3 channels
class gmp_modulator_entity_3ch
{
public:
	// ctor & dtor
	gmp_modulator_entity_3ch()
	{}

public:
	// utilities function

	// Set modulator compare value
	virtual void set_modulator_cmp(
		gmp_timer_cmp_t cmp1_value, gmp_timer_cmp_t cmp2_value,
		gmp_timer_cmp_t cmp3_value
	);

	// Set frequency of timer
	virtual void set_modulator_freq(gmp_timer_cmp_t freq_value);

	// Enable modulator output
	virtual void enable();

	// Disable modulator output
	virtual void disable();
};

// This class has 4 channels
class gmp_modulator_entity_4ch
{
public:
	// ctor & dtor
	gmp_modulator_entity_4ch()
	{}

public:
	// utilities function

	// Set modulator compare value
	virtual void set_modulator_cmp(
		gmp_timer_cmp_t cmp1_value, gmp_timer_cmp_t cmp2_value,
		gmp_timer_cmp_t cmp3_value, gmp_timer_cmp_t cmp4_value
	);

	// Set frequency of timer
	virtual void set_modulator_freq(gmp_timer_cmp_t freq_value);

	// Enable modulator output
	virtual void enable();

	// Disable modulator output
	virtual void disable();
};

// entity of the CAN controller device
class gmp_can_entity
	:public virtual concept_rw_with_addr<uint16_t, gmp_data_t>,   // For standard frame
	public virtual concept_rw_with_dualaddr<uint16_t, uint16_t, gmp_data_t> // For each frame type
{
public:
	// ctor & dtor
	typedef uint16_t addr1_type;
	typedef uint16_t addr2_type;
	typedef addr2_type addr_type;
	typedef gmp_data_t data_type;

public:
	// utilities function

	// In general, this function only send a standard data frame.
	virtual gmp_size_t write(addr_type addr, data_type* data, gmp_size_t length) = 0;
	virtual gmp_size_t read(addr_type addr, data_type* data, gmp_size_t length) = 0;

	// The first address is the frame type:
	// standard frame or extension frame; data frame or remote frame
	virtual gmp_size_t write(addr1_type frame_type, addr2_type addr, data_type* data, gmp_size_t length) = 0;
	virtual gmp_size_t read(addr1_type frame_type, addr2_type addr, data_type* data, gmp_size_t length) = 0;


public:

};

#endif // _FILE_PERIPHERAL_PROTOTYPE_HPP_
