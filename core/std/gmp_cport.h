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



#ifdef __cplusplus
}
#endif


#endif // _FILE_GLOBAL_H
