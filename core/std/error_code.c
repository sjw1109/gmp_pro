#include <core/gmp_core.h>

// This variable may save the last error (not fatal) code.
// These code may help developer locate faults rapidly.
gmp_stat_t last_error;

// This variable may save the last fatal (not error) code.
// These code may help developer locate faults rapidly.
gmp_stat_t last_fatal;


#if !defined SPECIFY_STUCK_WHEN_ERROR
// error
// -- error-info(auto generated)
// just when error code is error or even fatal, the system will stuck here.
gmp_stat_t gmp_error(gmp_stat_t error_code)
{
    if(error_code > GMP_STAT_FATAL_BEGIN)
        gmp_system_stuck();

    return error_code;
}

#endif // SPECIFY_STUCK_WHEN_ERROR
