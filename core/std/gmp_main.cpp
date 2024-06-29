
#define _CRT_SECURE_NO_WARNINGS

// necessary System headers 
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// Module dependences
#include <core/gmp_core.hpp>


//////////////////////////////////////////////////////////////////////////
// Step I: Global variables

// global variables in GMP
size_gt g_delay;
size_gt g_delay_ms;

// block heap memory space
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
// default heap
GMP_MEM_ALIGN data_gt default_heap[GMP_DEFAULT_HEAP_SIZE];
// handle of default heap
gmp_mem_area_head* default_mem_heap;
#endif // SPECIFY_GMP_BLOCK_MEMORY_ENABLE



//////////////////////////////////////////////////////////////////////////
// This function should be called in `main` after initialize routine.
// This function should implement by user to trans_setup all the peripherals of the chip.
// NOTICE: This function will not return!
void gmp_entry(void)
{
	// user initialize section
	gmp_init();

#if defined SPECIFY_ENABLE_TEST_ENVIRONMENT
	// print warning informations
	gmp_dbg_prt("[NOTE] GMP DEBUG MODE IS ENABLE! Program functionality may be affected.\r\n");

	// GMP test bench trans_setup
	gmp_test_init();
#else
	// Call user initialize
	user_init();
#endif // SPECIFY_ENABLE_TEST_ENVIRONMENT



#ifndef SPECIFY_DISABLE_GMP_LOGO
	// Debug information print
	gmp_setup_label();
#endif // SPECIFY_DISABLE_GMP_LOGO



	// take over `main loop`
#ifdef SPECIFY_PC_TEST_ENV
	for (int test_loop_cnt = 0; test_loop_cnt < 1000; ++test_loop_cnt)
		gmp_loop();
#else
	while (1)
	{
		// Call GMP general loop routine
		// This function may call watch-dog feeding function and scheduling function.
		gmp_loop();

		// Call user general loop routine
		user_loop();
}
#endif
	// Unreachable region
	//return;
}


// Main function
// GMP library initialization function
// If you won't let GMP take over your whole program,
// you should invoke this function in your main() function.
// An the other things should be completed in your own main() process.
void gmp_init()
{
	// CSP start up function
	gmp_csp_startup();

	// initialize peripheral
	// This function was defined in <bsp/user/peripheral_mapping.c>
	gmp_setup_peripheral();

	// initialize peripheral tree
	// This function was defined in <bsp/user/peripheral_mapping.c>
	gmp_init_peripheral_tree();

	// Setup GMP library

#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
	// Setup default heap memory controller
	default_mem_heap = gmp_mem_setup(default_heap, GMP_DEFAULT_HEAP_SIZE, GMP_MEM_BLOCK_SIZE);
#endif

	// initialize global delay 
	g_delay = TIMEOUT_CNT;
	g_delay_ms = TIMEOUT_MS;

	// initialize global error code


}

// This function would invoke some GMP based loop routine.
// If your main function take over by GMP library,
// the following function would be invoked as a loop routine.
void gmp_loop()
{
#if defined SPECIFY_ENABLE_TEST_ENVIRONMENT
	// This function would happened when debug mode.
	gmp_test_loop();
#endif // SPECIFY_ENABLE_TEST_ENVIRONMENT


#if defined SPECIFY_ENABLE_FEED_WATCHDOG
	gmp_port_feed_dog();
#endif 


}

#ifndef SPECIFY_DISABLE_GMP_LOGO
// This function would print a GMP label
void gmp_setup_label()
{
	gmp_dbg_prt("................................................\r\n");
	gmp_dbg_prt(".....MMMMMMMM.........MM.....,MMM....=MMWWDMN,..\r\n");
	gmp_dbg_prt("...8MM.......M.......MMMM....N8MM ....:MM...MM8.\r\n");
	gmp_dbg_prt("...MMM............. MM..MM?.?N.8M=....:MM..:MM..\r\n");
	gmp_dbg_prt("..=MMD.............=M,..DMM.N..8M=....:MMMMZ,...\r\n");
	gmp_dbg_prt("...MMM.....MMM.....=M,...MMMN..8M=....:MM.......\r\n");
	gmp_dbg_prt("...=MM......M......=M,....MM...8M=....:MM.......\r\n");
	gmp_dbg_prt(".....$M$$$$MMM.....===..........===..=NNMM+.....\r\n");
	gmp_dbg_prt("................................................\r\n");
	gmp_dbg_prt("................................................\r\n");
	gmp_dbg_prt("....General.........Motor............Platform...\r\n");
	gmp_dbg_prt(".........for all Motor & all Platform...........\r\n");
	gmp_dbg_prt("................................................\r\n");
	gmp_dbg_prt("................................................\r\n");
	gmp_dbg_prt("[okay] General motor platform ready.\r\n");
}
#endif // SPECIFY_DISABLE_GMP_LOGO

//////////////////////////////////////////////////////////////////////////
// The prototype of the following two functions would be found in
// <core/std/public.h>

// The following function would be called by libraries in the default case.

// unit byte
void* gmp_malloc(size_gt size)
{
	// This function was specified by <user_config.h>
	return GMP_BLOCK_ALLOC_FUNC(size);
}


void gmp_free(void* ptr)
{
	// This function was specified by <user_config.h>
	GMP_BLOCK_FREE_FUNC(ptr);
}


//////////////////////////////////////////////////////////////////////////
// implement of new and delete operator
// TODO impl


//////////////////////////////////////////////////////////////////////////
// default debug device handle
// This pointer should be redirect by <user/peripheral_mapping.c>
#if defined SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC
gmp_concept_write_direct* default_debug_dev = nullptr;

// implement the gmp_debug_print routine.
size_gt gmp_dbg_prt_fn(const char* p_fmt, ...)
{
	// if no one was specified to output, just ignore the request.
	if (default_debug_dev == nullptr)
	{
		return 0;
	}

	size_gt size = strlen(p_fmt);
#if defined SPECIFY_DISABLE_DYNAMIC_ALLOC_OF_DBGPTR
	char str[48 + _GMP_CHAR_EXT];
	memset(str, 0, 48 + _GMP_CHAR_EXT);
#else
	char* str = (char*)gmp_malloc(size + _GMP_CHAR_EXT);
	memset(str, 0, size + _GMP_CHAR_EXT);
#endif // SPECIFY_DISABLE_DYNAMIC_ALLOC_OF_DBGPTR

	va_list vArgs;
	va_start(vArgs, p_fmt);
	vsprintf(str, (char const*)p_fmt, vArgs);
	va_end(vArgs);

	size_gt len = strlen(str);

	size_gt ret = default_debug_dev->write((data_gt*)str, len);

#if defined SPECIFY_DISABLE_DYNAMIC_ALLOC_OF_DBGPTR
#else 
	gmp_free(str);
#endif //SPECIFY_DISABLE_DYNAMIC_ALLOC_OF_DBGPTR

	return ret;
}


#endif // SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

//////////////////////////////////////////////////////////////////////////
// GMP Library error handler
void gmp_system_stuck(void)
{
	// disable the GMP library and freeze the scene
	// TODO impl


	// invoke user stuck function 
	gmp_port_system_stuck();
}

//////////////////////////////////////////////////////////////////////////
// A function is not implement, this is just a place holder.

void gmp_not_impl(const char* file, uint32_t line)
{

#if defined SPECIFY_ENABLE_UNIMPL_FUNC_WARNING
	// print the error information
	gmp_dbg_prt("[Erro] invoke unimplemented function: [%s, %d].\r\n", file, line);
#endif // SPECIFY_ENABLE_UNIMPL_FUNC_WARNING

#if defined SPECIFY_STUCK_WHEN_UNIMPL_FUNC
#if defined SPECIFY_ENABLE_UNIMPL_FUNC_WARNING
	gmp_dbg_prt("[INFO] Program has stuck!\r\n");
#endif // SPECIFY_ENABLE_UNIMPL_FUNC_WARNING

	// Stop program right now
	gmp_system_stuck();

#endif // SPECIFY_STUCK_WHEN_UNIMPL_FUNC


}

//////////////////////////////////////////////////////////////////////////
// default little-endian to big-endian

// uint16_t gmp_l2b12(uint16_t data)
// {
// 	return ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);
// }

uint16_t gmp_l2b16(uint16_t data)
{
	return ((data & 0xFF) << 8) | ((data & 0xFF00) >> 8);
}

uint32_t gmp_l2b32(uint32_t data)
{
	return ((data & 0xFF000000) >> 24) | ((data & 0xFF0000) >> 8)
		| ((data & 0xFF00) << 8) | ((data & 0xFF) << 24);
}

#ifdef SPECIFY_ENABLE_INTEGER64
uint64_t gmp_l2b64(uint64_t data)
{
	return ((data & 0xFF00000000000000) >> 56)
		| ((data & 0xFF000000000000) >> 40)
		| ((data & 0xFF0000000000) >> 24)
		| ((data & 0xFF00000000) >> 8)
		| ((data & 0xFF000000) << 8)
		| ((data & 0xFF0000) << 24)
		| ((data & 0xFF00) << 40)
		| ((data & 0xFF) << 56);
}
#endif // SPECIFY_ENABLE_INTEGER64

//////////////////////////////////////////////////////////////////////////
// weak functions

GMP_WEAK_FUNC_PREFIX
void gmp_setup_peripheral()
GMP_WEAK_FUNC_SUFFIX
{}

GMP_WEAK_FUNC_PREFIX
void gmp_init_peripheral_tree()
GMP_WEAK_FUNC_SUFFIX
{}

GMP_WEAK_FUNC_PREFIX
void user_init(void)
GMP_WEAK_FUNC_SUFFIX
{}

GMP_WEAK_FUNC_PREFIX
void user_loop(void)
GMP_WEAK_FUNC_SUFFIX
{}

