//////////////////////////////////////////////////////////////////////////
// Step I language patch
// 
// Wrong C++ version
#ifdef __cplusplus
#if __cplusplus < 199711L
#error This library needs at least a C++03 compliant compiler.
#endif // __cplusplus <= 199711L
#endif // __cplusplus

// Lower version compiler patch
#if __cplusplus < 201103L
#define constexpr const
#define override
#define final

#ifndef nullptr
#define nullptr (0x00000000)
#endif // nullptr alternative definition

#endif // __cplusplus <= 201103L

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static inline
#endif //__STATIC_INLINE

//////////////////////////////////////////////////////////////////////////
// Step II system library

//////////////////////////////////////////////////////////////////////////
// Step III library support macro

// + weak function Modifier
#define GMP_WEAK_FUNC_PREFIX __weak
#define GMP_WEAK_FUNC_SUFFIX 

// + disable optimization
#define GMP_NO_OPT_PREFIX
#define GMP_NO_OPT_SUFFIX __attribute__((optnone))

// + variables aligned
#define GMP_MEM_ALIGN __attribute__((aligned (4)))
