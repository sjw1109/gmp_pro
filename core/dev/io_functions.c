
#include <core/std/typedef.h>


#include <bsp/bsp_header.h>


// This variable store the last error code for gmp_dev_R/W functions.
gmp_stat_t gmp_io_last_error;

gmp_length_t gmp_dev_write
(
	gmp_phy_mapping_t* mapping,
	gmp_data_element_t* content,
	gmp_length_t cap
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
		return gmp_iic_transmit(mapping->peripheral->handle, 
			((gmp_iic_mapping_t*)mapping)->device_id,
			content, cap);
	case PROTO_PERIPHERAL_IIC_SLAVE:

		break;

	case PROTO_PERIPHERAL_IIS:
		break;

	case PROTO_PERIPHERAL_ADC:
		break;
	}
	// unknown method
	return 0;

}

gmp_length_t gmp_dev_read
(
	gmp_phy_mapping_t* mapping,
	gmp_data_element_t* content,
	gmp_length_t cap
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
		return gmp_iic_receive(
			mapping->peripheral->handle,
			((gmp_iic_mapping_t*)mapping)->device_id,
			((gmp_iic_mapping_t*)mapping)->cmd,
			((gmp_iic_mapping_t*)mapping)->cmd_length,
			content, cap);
	case PROTO_PERIPHERAL_IIC_SLAVE:

		break;

	case PROTO_PERIPHERAL_IIS:
		break;

	case PROTO_PERIPHERAL_ADC:
		break;
	}

	// unknown method
	return 0;

}


gmp_length_t gmp_dev_request_read_mem
(
	gmp_phy_mapping_t* mapping,
	gmp_cmd_t cmd,
	gmp_length_t length
)
{
	// get device type
	gmp_prototype_t dev_type = mapping->peripheral->type;

	switch (dev_type)
	{
	case PROTO_PERIPHERAL_IIC:
		((gmp_iic_mapping_t*)mapping)->cmd = cmd;
		((gmp_iic_mapping_t*)mapping)->cmd_length = length;
		gmp_iic_request_read_mem(mapping->peripheral->handle,
			cmd, length);
		break;
	default:
		break;
	}

	// unknown method
	return 0;
}

