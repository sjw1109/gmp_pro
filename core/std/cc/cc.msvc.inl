// + MSVC doesn't support __cplusplus macro.
// Must use MSVC CLANG Compiler


//////////////////////////////////////////////////////////////////////////
// Step I language patch
// 
// Disable safe function for higher version MSVC compiler
#if (_MSC_VER >= 1600)  
// Disable the _s(safe) functions.
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static inline
#endif //__STATIC_INLINE

//////////////////////////////////////////////////////////////////////////
// Step II system library

//////////////////////////////////////////////////////////////////////////
// Step III library support macro

// + weak function Modifier
#define GMP_WEAK_FUNC_PREFIX __attribute__((weak))
#define GMP_WEAK_FUNC_SUFFIX 

// + disable optimization
#define GMP_NO_OPT_PREFIX
#define GMP_NO_OPT_SUFFIX

// + variables aligned
#define GMP_MEM_ALIGN  __declspec(align(8))

