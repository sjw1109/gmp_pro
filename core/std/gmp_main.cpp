
// necessary System headers 
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

// Module dependences
#include <core/gmp_core.hpp>


//////////////////////////////////////////////////////////////////////////
// Global variables

// global variables in GMP
gmp_size_t g_delay;
gmp_size_t g_delay_ms;

// block heap memory space
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
// default heap
GMP_MEM_ALIGN gmp_data_t default_heap[GMP_DEFAULT_HEAP_SIZE];
// handle of default heap
gmp_mem_area_head* default_mem_heap;
#endif // SPECIFY_GMP_BLOCK_MEMORY_ENABLE



//////////////////////////////////////////////////////////////////////////
// This function should be called in `main` after initialize routine.
// This function should implement by user to trans_setup all the peripherals of the chip.
// NOTICE: This function will not return!
void gmp_entry(void)
{
	// initialize peripheral
	// This function was defined in <bsp/user/peripheral_mapping.c>
	gmp_setup_peripheral();

	// initialize peripheral tree
	// This function was defined in <bsp/user/peripheral_mapping.c>
	gmp_init_peripheral_tree();

	// user initialize section
	gmp_init();

	// Debug information print
	gmp_setup_label();

#if defined SPECIFY_ENABLE_TEST_ENVIRONMENT
	// print warning informations
	gmp_dbg_prt("[NOTE] GMP DEBUG MODE IS ENABLE! Program functionality may be affected.\r\n");

	// GMP test bench trans_setup
	gmp_test_init();
#endif // SPECIFY_ENABLE_TEST_ENVIRONMENT

	// take over `main loop`
#ifdef GMP_WINDOWS
	gmp_loop();
#else
	while (1) gmp_loop();
#endif
	return;
}


// GMP library initialization function
void gmp_init()
{
	// Setup GMP library

#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
	// Setup default heap memory controller
	default_mem_heap = gmp_mem_setup(default_heap, GMP_DEFAULT_HEAP_SIZE, GMP_MEM_BLOCK_SIZE);
#endif

	// set global delay 
	g_delay = TIMEOUT_CNT;
	g_delay_ms = TIMEOUT_MS;

	// Call user initialize
	user_init();
}

// This function would 
void gmp_loop()
{
#if defined SPECIFY_ENABLE_TEST_ENVIRONMENT
	// This function would happened when debug mode.
	gmp_test_loop();
#endif // SPECIFY_ENABLE_TEST_ENVIRONMENT

	user_loop();
}

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

//////////////////////////////////////////////////////////////////////////
// The prototype of the following two functions would be found in
// <core/std/public.h>

// The following function would be called by libraries in the default case.

// unit byte
void* gmp_malloc(gmp_size_t size)
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
gmp_size_t gmp_dbg_prt_fn(const char* p_fmt, ...)
{
	// if no one was specified to output, just ignore the request.
	if (default_debug_dev == nullptr)
	{
		return 0;
	}

	gmp_size_t size = strlen(p_fmt);
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

	gmp_size_t len = strlen(str);

	gmp_size_t ret = default_debug_dev->write((gmp_data_t*)str, len);

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
	gmp_dbg_prt("[INFO] Program has stuck!");
#endif // SPECIFY_ENABLE_UNIMPL_FUNC_WARNING

	// Stop program right now
	gmp_system_stuck();

#endif // SPECIFY_STUCK_WHEN_UNIMPL_FUNC


}

