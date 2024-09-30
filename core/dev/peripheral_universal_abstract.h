/**
 * @file peripheral_universal_abstract.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */

#ifndef _FILE_PERIPHERAL_UNIVERSAL_ABSTRACT_H_
#define _FILE_PERIPHERAL_UNIVERSAL_ABSTRACT_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	//////////////////////////////////////////////////////////////////////////

#ifndef GMP_DUPLEX_DEV_DEFAULT_BUF_LEN
#define GMP_DUPLEX_DEV_DEFAULT_BUF_LEN ((16))
#endif // GMP_DEPLEX_DEV_DEFAULT_BUF_LEN

	// This device provide a duplex device buffer
	typedef struct _tag_duplex_dev_t
	{
		data_gt rx_buf[GMP_DUPLEX_DEV_DEFAULT_BUF_LEN];
		data_gt tx_buf[GMP_DUPLEX_DEV_DEFAULT_BUF_LEN];
	}duplex_dev_t;

	//////////////////////////////////////////////////////////////////////////

#ifndef GMP_HALF_DUPLEX_DEV_DEFAULT_BUF_LEN
#define GMP_HALF_DUPLEX_DEV_DEFAULT_BUF_LEN ((16))
#endif // GMP_DEPLEX_DEV_DEFAULT_BUF_LEN


	// This device provide a half-duplex device buffer
	typedef struct _tag_half_duplex_dev_t
	{
		data_gt buf[GMP_DUPLEX_DEV_DEFAULT_BUF_LEN];
	}half_duplex_dev_t;


	//////////////////////////////////////////////////////////////////////////
	// 
	// This is a IIC command device model
	typedef struct _tag_iic_dev_t
	{
		
	}iic_dev_t;


	//////////////////////////////////////////////////////////////////////////
	// 
	// This is a IIC memeory device model
	typedef struct _tag_iic_mem_dev_t
	{

	}iic_mem_dev_t;


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PERIPHERAL_UNIVERSAL_ABSTRACT_H_


