//////////////////////////////////////////////////////////////////////////
// Step I language patch
// 
// Wrong C++ version
#if __cplusplus < 199711L
#error This library needs at least a C++03 compliant compiler.
#endif // __cplusplus <= 199711L

// Lower version compiler patch
#if __cplusplus < 201103L
#define constexpr const
#define override
#define final

#ifndef nullptr
#define nullptr (0x00000000)
#endif // nullptr alternative definition

#endif // __cplusplus <= 201103L


//////////////////////////////////////////////////////////////////////////
// Step II system library

//////////////////////////////////////////////////////////////////////////
// Step III library support macro

// + weak function Modifier
#define GMP_WEAK_FUNC_PREFIX 
#define GMP_WEAK_FUNC_SUFFIX __attribute__((weak))

// + disable optimization
#define GMP_NO_OPT_PREFIX _Pragma("FUNCTION_OPTIONS(\"--opt_level=0\")")
#define GMP_NO_OPT_SUFFIX

// + variables aligned
#define GMP_MEM_ALIGN __Pragma("DATA_ALIGN(4)")

// + inline modifier
#define GMP_INLINE _Pragma("FUNC_ALWAYS_INLINE")

// + static inline modifier
#define GMP_FORCE_INLINE _Pragma("FORCEINLINE")
