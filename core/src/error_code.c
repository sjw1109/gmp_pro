

#include <core/gmp_core.h>


//////////////////////////////////////////////////////////////////////////
// Step I: implement the global variables
// 

// This variable may save the last error code (any return value will include).
// These code may help developer locate faults rapidly.
gmp_stat_t g_gmp_last_ret;


// This variable may save the last error (fatal and error) code.
// These code may help developer locate faults rapidly.
gmp_stat_t g_gmp_last_error;


// This variable may save the last fatal (only fatal) code.
// These code may help developer locate faults rapidly.
gmp_stat_t g_gmp_last_fatal;


//////////////////////////////////////////////////////////////////////////
// Step II: include the error code show function
//

#include <core/std/ec/erro_code.show.inl>

