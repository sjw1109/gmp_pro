
// This file provide all GMP basic standard types

#ifndef _FILE_GMP_STD_H_
#define _FILE_GMP_STD_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


// ....................................................................//
// invoke all the configurations

// invoke user config file
#include "user.config.h"

// invoke system default config
#include <.config.h>

// invoke CSP default config
#include <csp.config.h>


// ....................................................................//
// invoke all the GMP STD (standard support)

// Compiler support
#include <core/std/compiler_sup.h>

// GMP error code support
#include <core/std/error_code.h>

// CSP basic type
#include <csp.typedef.h>

// Invoke default type
#include <core/std/default.types.h>

// peripheral structure definitions
#include <core/std/gmp_peripheral_definition.h>

// Invoke peripheral headers
#include <core/dev/devif.h>







#ifdef __cplusplus
}
#endif // __cplusplus



#endif // _FILE_GMP_STD_H_
