#include <bsp_header.h>

// This variable store the last error code for gmp_dev_R/W functions.
gmp_stat_t gmp_io_last_error;

uint32_t gmp_dev_write
(
	gmp_phy_mapping_t* mapping,
	gmp_data_element_t* content,
	uint32_t cap
)
{
	// get device type
	gmp_prototype_t dev_type = mapping->peripheral->type;

	switch (dev_type)
	{
	case PROTO_PERIPHERAL_NUL:
		return cap;

	case PROTO_PERIPHERAL_UART:

		break;
	case PROTO_PERIPHERAL_SPI:

		break;
	case PROTO_PERIPHERAL_IIC:
		gmp_iic_handle_t iic_handle = mapping->peripheral->handle;
		uint16_t iic_dev_addr = ((dev_iic_mapping_t*)mapping)->device_id;
		return gmp_iic_transmit(iic_handle, iic_dev_addr, content, cap);
	case PROTO_PERIPHERAL_IIC_SLAVE:

		break;

	case PROTO_PERIPHERAL_IIS:
		break;

	case PROTO_PERIPHERAL_ADC:
		break;
	}

}

