#ifndef _FILE_PROTOTYPE_H_
#define _FILE_PROTOTYPE_H_



// GMP device prototype type
typedef enum 
{
	PROTO_PERIPHERAL_NUL = 0,
	PROTO_PERIPHERAL_UART,
	PROTO_PERIPHERAL_SPI,
	PROTO_PERIPHERAL_IIC,
	PROTO_PERIPHERAL_IIC_SLAVE,
	PROTO_PERIPHERAL_IIS,
	PROTO_PERIPHERAL_ADC,
}gmp_prototype_t;

// GMP device extension package config
typedef enum 
{
	// This pointer is reserved for user.
	EXT_PCK_NUL = 0x0000,
	// TX buffer
	EXT_PCK_TX_BUFFER = 0x0001,
	// RX buffer
	EXT_PCK_RX_BUFFER,
	// Transmit Callback
	EXT_PCK_TX_HOOK,
	// Receive Callback
	EXT_PCK_RX_HOOK,
	// Timing Callback
	EXT_PCK_TIMING_HOOK,
	// Routine Complete Callback
	EXT_PCK_CMPT_HOOK,
	// Error, Abort, Timeout
	EXT_PCK_EXCEPTION_ROUTINE,
	// This symbol only for counting
	EXT_PCK_COUNTING
}gmp_ext_config_t;


// Physical peripheral Bus
typedef struct _tag_gmp_peripheral_t
{
	// device type
	uint16_t type;

	// capacity of the whole struct
	uint32_t cap;

	// device handle
	gmp_phy_handle_t handle;

	// extension address
	void* ext[EXT_PCK_COUNTING];
}gmp_peripheral_t;


// describe a specific device on the peripheral 
typedef struct _tag_gmp_phy_mapping_t
{
	gmp_peripheral_t* peripheral;
}gmp_phy_mapping_t;

// IIC
typedef struct _tag_dev_iic_mapping_t
{
	gmp_phy_mapping_t base;
	gmp_length_t device_id;
	gmp_cmd_t cmd;
	gmp_length_t cmd_length;
}gmp_iic_mapping_t;

typedef struct _tag_dev_iic_mem_mapping_t
{
	gmp_phy_mapping_t base;
	gmp_length_t device_id;
	gmp_cmd_t cmd;
	gmp_length_t mem_length;
}gmp_iic_mem_mapping_t;

#endif // _FILE_PROTOTYPE_H_

