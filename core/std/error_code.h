
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
// branch should not be reached
#define GMP_STAT_UNDEFINED_ACTION       ((GMP_STAT_FATAL_BEGIN + 3))
// Hardware damaged
#define GMP_STAT_HARD_ERROR             ((GMP_STAT_FATAL_BEGIN + 4))
// User should implement the function before use.
#define GMP_STAT_NOT_IMPL               ((GMP_STAT_ERRO_BEGIN  + 5))
// Device is busy
#define GMP_STAT_BUSY                   ((GMP_STAT_WARN_BEGIN  + 6))
// Device is not ready, maybe you haven't trans_init the peripheral
#define GMP_STAT_NOT_READY              ((GMP_STAT_ERRO_BEGIN  + 7))
// MM module to less memory too small
#define GMP_STAT_MM_NOT_ENOUGH_MEM      ((GMP_STAT_ERRO_BEGIN  + 8))
// MM module cannot write these memory, or memory check wfr_fault
#define GMP_STAT_MM_WRITE_REFUSE        ((GMP_STAT_ERRO_BEGIN  + 8))
// MM module pass an invalid parameter into the function
#define GMP_STAT_INVALID_PARAM          ((GMP_STAT_ERRO_BEGIN  + 9))
// MM module unable to find the specified memory block
#define GMP_STAT_MM_NO_SPECIFIED_BLOCK  ((GMP_STAT_ERRO_BEGIN  + 10))
// Timeout
#define GMP_STAT_RESPONSE_TIMEOUT		((GMP_STAT_WARN_BEGIN  + 11))
// data struct overflow
#define GMP_STAT_OVERFLOW               ((GMP_STAT_WARN_BEGIN  + 12))
// This function is disabled
#define GMP_STAT_FUNCTION_DISABLE       ((GMP_STAT_ERRO_BEGIN  + 13))
// workflow disabled
#define GMP_STAT_WF_DISABLED            ((GMP_STAT_INFO_BEGIN  + 14))
// object reinit
#define GMP_STAT_REINIT                 ((GMP_STAT_INFO_BEGIN  + 15))
// A null pointer is treated as function parameters
#define GMP_STAT_ZERO_PTR               ((GMP_STAT_ERRO_BEGIN  + 16))
// The module has been launched, so trans_init function should not be called!
#define GMP_STAT_MODULE_LAUNCHED        ((GMP_STAT_ERRO_BEGIN  + 17))
// The Workflow module is running, so user cannot modify the config of the class
#define GMP_STAT_WF_CANNOT_MODIFY       ((GMP_STAT_ERRO_BEGIN  + 18))
// If the workflow module is in reachable verify mode, and the verify is said non-reachable.
#define GMP_STAT_WF_NONREACHABLE        ((GMP_STAT_ERRO_BEGIN  + 19))
// the workflow object has start
#define GMP_STAT_WF_HAS_START           ((GMP_STAT_WARN_BEGIN  + 20))
// the workflow is not initialize complete, user should reset the class
#define GMP_STAT_WF_NO_RESET            ((GMP_STAT_ERRO_BEGIN  + 21))
// Operation was refused
#define GMP_STAT_REFUSED                ((GMP_STAT_ERRO_BEGIN  + 22))

// GMP library standard error return function
inline gmp_stat_t gmp_is_error(gmp_stat_t error_code)
{
    return error_code > GMP_STAT_ERRO_BEGIN;
}

inline gmp_stat_t gmp_is_warning(gmp_stat_t error_code)
{
    return error_code > GMP_STAT_WARN_BEGIN;
}

inline gmp_stat_t gmp_is_fine(gmp_stat_t error_code)
{
    return error_code < GMP_STAT_WARN_BEGIN;
}

#if defined SPECIFY_STUCK_WHEN_ERROR
inline gmp_stat_t gmp_error(gmp_stat_t error_code)
{
    if (error_code > GMP_STAT_FATAL_BEGIN)
        gmp_system_stuck();

    return error_code;
}
#else
gmp_stat_t gmp_error(gmp_stat_t error_code);
#endif // SPECIFY_STUCK_WHEN_ERROR




#ifdef __cplusplus
}
#endif

#endif // _FILE_ERROR_CODE_H_
