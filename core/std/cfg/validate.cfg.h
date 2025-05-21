
// This file validate configurations

#ifndef _FILE_GMP_CORE_VALIDATE_CFG_H_
#define _FILE_GMP_CORE_VALIDATE_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// validate parameters
#if defined BIG_ENDIAN && defined LITTLE_ENDIAN
#error "You should chose big-endian or little-endian."
#endif // BIG_ENDIAN && LITTLE_ENDIAN

#if SPECIFY_GMP_DEFAULT_ALLOC == USING_MANUAL_SPECIFY_FUNCTION
#if !((defined SPECIFY_GMP_USER_ALLOC) && (defined SPECIFY_GMP_USER_FREE))
#error "You should specify user_alloc and user_free function, via config files."
#endif
#endif // USING_MANUAL_SPECIFY_FUNCTION

	// To mark a unused param
#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(x) ((void)(x))
#endif // UNUSED_PARAMETER

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GMP_CORE_VALIDATE_CFG_H_
