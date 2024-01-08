
// This file provided a set of macros to help user to config the code.

#ifndef _FILE_COMPILER_SUP_H_
#define _FILE_COMPILER_SUP_H_

// Check C++ compiler, and ensure it is fulfilling the requirement.
#if defined __cplusplus
#if __cplusplus <= 199711L
#error This library needs at least a C++11 compliant compiler.
#endif
#endif

// To mark a function that shouldn't be optimize by compiler
#ifndef GMP_NO_OPT
#if defined COMPILER_AC6
#define GMP_NO_OPT __attribute__((optnone))
#else
#define GMP_NO_OPT
#endif // GMP_NO_OPT
#endif // GMP_NO_OPT

// To mark a variable that should aligned
#ifndef GMP_MEM_ALIGN
#if defined COMPILER_AC6
#define GMP_MEM_ALIGN __attribute__((aligned (4)))
#else
#define GMP_MEM_ALIGN
#endif // GMP_MEM_ALIGN
#endif // GMP_MEM_ALIGN


// To mark a function that should be a weak function
#ifndef WEAK_FUNCTION
#if defined GMP_WINDOWS
#define WEAK_FUNCTION 
#elif defined COMPILER_CCS
#define WEAK_FUNCTION __attribute__((weak))
#else
#define WEAK_FUNCTION __weak
#endif
#endif // WEAK_FUNCTION

// To mark a function that should invoke as quick as possible 
#ifndef FAST_FUNCTION
#define FAST_FUNCTION 
#endif // FAST_FUNCTION

#endif // _FILE_COMPILER_SUP_H_
