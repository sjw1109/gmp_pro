
//////////////////////////////////////////////////////////////////////////
// Error Code

#ifndef _FILE_ERROR_CODE_H_
#define _FILE_ERROR_CODE_H_

#ifdef __cplusplus
extern "C"
{
#endif

	

typedef uint32_t gmp_stat_t;

// function to judge if error is fatal

// extern variables
// This variable may save the last error (not fatal) code.
// These code may help developer locate faults rapidly.
extern gmp_stat_t last_error;

// This variable may save the last fatal (not error) code.
// These code may help developer locate faults rapidly.
extern gmp_stat_t last_fatal;


// standard error code.
// Just an information
#define GMP_STAT_INFO_BEGIN				(0x00000000)
// an warning you may deal with it if it necessary
#define GMP_STAT_WARN_BEGIN				(0x20000000)
// an error you must deal with it before get out of hand
#define GMP_STAT_ERRO_BEGIN				(0x40000000)
// an fatal error happened and the system will shutdown right now
// And the system may only recover from reset manually.
#define GMP_STAT_FATAL_BEGIN			(0x80000000)

// This area to define all infos, warnings or errors.
#define GMP_STAT_OK				        ((0x00000000))
#define GMP_STAT_GENERAL_WARN           ((GMP_STAT_WARN_BEGIN  + 1))
#define GMP_STAT_GENERAL_ERROR	        ((GMP_STAT_ERRO_BEGIN  + 1))

#define GMP_STAT_WARN_PRINT             ((GMP_STAT_WARN_BEGIN  + 2))
// branch should not reached
#define GMP_STAT_UNDEFINED_ACTION       ((GMP_STAT_FATAL_BEGIN + 3))
// Hardware damaged
#define GMP_STAT_HARD_ERROR             ((GMP_STAT_FATAL_BEGIN + 4))
// User should implement the function before use.
#define GMP_STAT_NOT_IMPL               ((GMP_STAT_ERRO_BEGIN  + 5))
// Device is busy
#define GMP_STAT_BUSY                   ((GMP_STAT_WARN_BEGIN  + 6))
// Device is not ready, maybe you haven't init the peripheral
#define GMP_STAT_NOT_READY              ((GMP_STAT_ERRO_BEGIN  + 7))
// MM module to less memory too small
#define GMP_STAT_MM_NOT_ENOUGH_MEM      ((GMP_STAT_ERRO_BEGIN  + 8))
// MM module cannot write these memory, or memory check fault
#define GMP_STAT_MM_WRITE_REFUSE        ((GMP_STAT_ERRO_BEGIN  + 8))
// MM module pass an invalid parameter into the function
#define GMP_STAT_INVALID_PARAM          ((GMP_STAT_ERRO_BEGIN  + 9))
// MM module unable to find the specified memory block
#define GMP_STAT_MM_NO_SPECIFIED_BLOCK  ((GMP_STAT_ERRO_BEGIN  + 10))

#ifdef __cplusplus
}
#endif

#endif // _FILE_ERROR_CODE_H_