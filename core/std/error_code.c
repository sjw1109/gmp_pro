#include <stdint.h>
#include "error_code.h"

// This variable may save the last error (not fatal) code.
// These code may help developer locate faults rapidly.
gmp_stat_t last_error;

// This variable may save the last fatal (not error) code.
// These code may help developer locate faults rapidly.
gmp_stat_t last_fatal;

