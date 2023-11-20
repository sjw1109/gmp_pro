#include <core/dev/prototypes.h>

#ifndef _FILE_IO_FUNCTIONS_H_
#define _FILE_IO_FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

	// This variable store the last error code for gmp_dev_R/W functions.
	extern gmp_stat_t gmp_io_last_error;


	gmp_length_t gmp_dev_write
	(
		gmp_phy_mapping_t* mapping, 
		gmp_data_element_t *content,
		gmp_length_t cap
	);

	gmp_length_t gmp_dev_write_without_block
	(
		gmp_phy_mapping_t* mapping,
		gmp_data_element_t* content,
		gmp_length_t cap
	);

	gmp_length_t gmp_dev_read
	(
		gmp_phy_mapping_t* mapping,
		gmp_data_element_t* content,
		gmp_length_t cap
	);

	gmp_length_t gmp_dev_read_with_block
	(
		gmp_phy_mapping_t* handle,
		gmp_data_element_t* content,
		gmp_length_t cap
	);

	void gmp_dev_request_read
	(
		gmp_phy_mapping_t* handle,
		gmp_cmd_t cmd
	);

	gmp_length_t gmp_dev_request_read_mem
	(
		gmp_phy_mapping_t* mapping,
		gmp_cmd_t cmd,
		gmp_length_t length
	);

	
	void gmp_dev_request_write
	(
		gmp_phy_mapping_t* handle,
		gmp_cmd_t cmd
	);

	void gmp_dev_request_write_mem
	(
		gmp_phy_mapping_t* handle,
		gmp_cmd_t cmd,
		gmp_length_t length
	);

#ifdef __cplusplus
}
#endif


#endif

