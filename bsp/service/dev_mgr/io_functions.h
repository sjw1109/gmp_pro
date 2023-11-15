#ifndef _FILE_IO_FUNCTIONS_H_
#define _FILE_IO_FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

	// This variable store the last error code for gmp_dev_R/W functions.
	extern gmp_stat_t gmp_io_last_error;


	uint32_t gmp_dev_write
	(
		gmp_phy_mapping_t* mapping, 
		gmp_data_element_t *content,
		uint32_t cap
	);

	uint32_t gmp_dev_write_without_block
	(
		gmp_phy_mapping_t* handle,
		gmp_data_element_t* content,
		uint32_t cap);

	uint32_t gmp_dev_read
	(
		gmp_phy_mapping_t* handle,
		gmp_data_element_t* content,
		uint32_t cap
	);

	uint32_t gmp_dev_read_without_block
	(
		gmp_phy_mapping_t* handle,
		gmp_data_element_t* content,
		uint32_t cap
	);




#ifdef __cplusplus
}
#endif


#endif

