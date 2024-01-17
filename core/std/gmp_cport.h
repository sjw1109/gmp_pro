// This file should ensure that the user may compile it as a C source file.
// User may include this file and access the GMP library.



#ifndef _FILE_GLOBAL_H
#define _FILE_GLOBAL_H

#ifdef __cplusplus
extern "C"
{
#endif

	//////////////////////////////////////////////////////////////////////////
	// The following functions should be implemented by user

	// Key this function must do implement, otherwise the time-based part may invalid.
	// This function would return the system tick point right now.
	gmp_time_t gmp_port_system_tick(
		void
	);

	// This function should be implement by user, when fatal error occurred, the function would be called.
	// So the function must own the ability of stop the program.
	void gmp_port_system_stuck(
		void
	);

#if defined SPECIFY_ENABLE_FEED_WATCHDOG
	// This function should be implemented by user,
	// Every Loop routine, this function would be called.
	// And user should ensure that the function has only one thing is to feed the watchdog
	void gmp_port_feed_dog(
		void
	);
#endif // SPECIFY_ENABLE_FEED_WATCHDOG


	//////////////////////////////////////////////////////////////////////////
	// These function have implemented by GMP
	void gmp_entry(void);
	void gmp_init_peripheral_tree(void);
	void gmp_setup_peripheral(void);
	void gmp_loop(void);
	void gmp_init(void);
	void gmp_setup_label(void);

	// This function would be called when system meets error.
	void gmp_system_stuck(void);

	// User should implement this two functions 
	void user_init(void);
	void user_loop(void);

	// For test user should enable and implement the following function
	void gmp_test_init(void);
	void gmp_test_loop(void);

	// The memory controller functions
	void* gmp_malloc(gmp_size_t size);
	void gmp_free(void* ptr);

	// When a function is unimplemented, the function would be invoke.
	void gmp_not_impl(const char* file, uint32_t line);


#ifdef SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

	// default ext buffer size for default debug print function
#ifndef _GMP_CHAR_EXT
#define _GMP_CHAR_EXT ((48))
#endif // _GMP_CHAR_EXT

// debug print function prototype
	gmp_size_t gmp_dbg_prt_fn(const char* p_fmt, ...);

	// debug output mode
#define gmp_dbg_prt gmp_dbg_prt_fn
#endif // SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

#ifndef gmp_dbg_prt
#define gmp_dbg_prt(x, ...)
#endif// debug output mode disabled


	//////////////////////////////////////////////////////////////////////////
	// global variables
	// The following variables will be defined in <core/std/global.c>

#ifndef TIMEOUT_CNT
#define TIMEOUT_CNT 0X3FFF
#endif // TIMEOUT_CNT

#ifndef TIMEOUT_MS
#define TIMEOUT_MS 0x000A
#endif // TIMEOUT_MS

	extern gmp_size_t g_delay;
	extern gmp_size_t g_delay_ms;

	//////////////////////////////////////////////////////////////////////////
	// Default heap memory set

	// Default heap memory space
#ifndef GMP_DEFAULT_HEAP_SIZE
#define GMP_DEFAULT_HEAP_SIZE ((1536))
#endif

	// Default heap size, byte(s)
#ifndef GMP_DEFAULT_BUFFER_SIZE
#define GMP_DEFAULT_BUFFER_SIZE ((32))
#endif

	// default heap memory space
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
	GMP_MEM_ALIGN
		extern gmp_data_t default_heap[GMP_DEFAULT_HEAP_SIZE];
#endif

	//////////////////////////////////////////////////////////////////////////
	// Error Handle function

	//////////////////////////////////////////////////////////////////////////
	// big-endian to little-endian
// 	uint16_t gmp_l2b12(uint16_t data);
	uint16_t gmp_l2b16(uint16_t data);
	uint32_t gmp_l2b32(uint32_t data);
#ifdef SPECIFY_ENABLE_INTEGER64
	uint64_t gmp_l2b64(uint64_t data);
#endif // SPECIFY_ENABLE_INTEGER64


	// Give Big-endian and little-endian an alias
	// User may config these function by <user_config.h>
// #ifndef L2B12
// #define L2B12 gmp_l2b12
// #endif // L2B12

#ifndef L2B16
#define L2B16 gmp_l2b16
#endif // L2B16

#ifndef L2B32
#define L2B32 gmp_l2b32
#endif // L2B32

#ifdef SPECIFY_ENABLE_INTEGER64
#ifndef L2B64
#define L2B64 gmp_l2b64
#endif // L2B64
#endif // SPECIFY_ENABLE_INTEGER64

#ifdef BIG_ENDIAN
#define BE12(x) ((x))
#define BE16(x) ((x))
#define BE32(x) ((x))
#ifdef SPECIFY_ENABLE_INTEGER64
#define BE64(x) ((x))
#endif // SPECIFY_ENABLE_INTEGER64
#define LE12 L2B12
#define LE16 L2B16
#define LE32 L2B32
#ifdef SPECIFY_ENABLE_INTEGER64
#define LE64 L2B64
#endif // SPECIFY_ENABLE_INTEGER64
#endif // BIG_ENDIAN

#ifdef LITTLE_ENDIAN
#define BE12 L2B12
#define BE16 L2B16
#define BE32 L2B32
#ifdef SPECIFY_ENABLE_INTEGER64
#define BE64 L2B64
#endif // SPECIFY_ENABLE_INTEGER64
#define LE12(x) ((x))
#define LE16(x) ((x))
#define LE32(x) ((x))
#ifdef SPECIFY_ENABLE_INTEGER64
#define LE64(x) ((x))
#endif // SPECIFY_ENABLE_INTEGER64
#endif // LITTLE_ENDIAN



#ifdef __cplusplus
}
#endif


#endif // _FILE_GLOBAL_H
