// This file should ensure that the user may compile it as a C source file.
// User may include this file and access the GMP library.


#ifndef _FILE_GLOBAL_H
#define _FILE_GLOBAL_H

#ifdef __cplusplus
extern "C"
{
#endif

	//////////////////////////////////////////////////////////////////////////
	// Step I: User function prototypes
	// 
	// The following functions should be implemented by user
	//

	// Key this function must do implement, otherwise the time-based part may invalid.
	// This function would return the system tick point right now.
	//
	time_gt gmp_port_system_tick(
		void
	);

	// This function should be implement by user, when fatal error occurred, the function would be called.
	// So the function must own the ability of stop the program.
	//
	void gmp_port_system_stuck(
		void
	);

//#if defined SPECIFY_ENABLE_FEED_WATCHDOG
	// This function should be implemented by user,
	// Every Loop routine, this function would be called.
	// And user should ensure that the function has only one thing is to feed the watchdog
	//
	void gmp_port_feed_dog(
		void
	);
//#endif // SPECIFY_ENABLE_FEED_WATCHDOG
	
	// ....................................................................//
	// GPIO action functions.
	//

	gmp_stat_t gmp_gpio_set_mode(
		hgpio_gt *hgpio,		     // handle of GPIO port
		gmp_gpio_mode_t mode     // input mode or output mode
	);

	void gmp_gpio_write(
		hgpio_gt *hgpio,	         // handle of GPIO port
		fast_gt level            // output level 
	);

	void gmp_gpio_set(
		hgpio_gt *hgpio	    // handle of GPIO port
	);

	void gmp_gpio_clear(
		hgpio_gt *hgpio	    // handle of GPIO port
	);

	fast_gt gmp_gpio_read(
		hgpio_gt *hgpio		     // handle of GPIO port
	);


	// ....................................................................//
	// User should implement this two functions 
	//

	// This function would execute only once.
	// User should implement all the initialization code in this function. 
	//
	void user_init(void);

	// This function would execute only once.
	// User should implement all the controller related initialization code in this function.
	// That means user init process may isolate with the controller init process. 
	void ctl_init(void);

	// This function would be the endless loop.
	// User should implement all the loop tasks and round-robin tasks.
	//
	void user_loop(void);

	// This function will be called by gmp_ctl_dispatch.
	void ctl_dispatch(void);

	// This function would be called by main ISR function.
	// User should call this function, in your ctl_main.cpp or just ignore it.
	// When you need to simulate your controller, this function would be invoked.
	// return 0 is normal, and any non-zero value means error.
	int gmp_ctl_dispatch(void);


	//////////////////////////////////////////////////////////////////////////
	// Step II: These function have implemented by GMP
	//

	// This function would be called by user in entry function.
	// And this function would not return.
	//
	void gmp_entry(void);

	// This function would be implemented by CSP (Chip support package).
	// This function would be called firstly, by `gmp_init`.
	//
	void gmp_csp_startup(void);

	// This function would create a peripheral tree.
	// Which will determine which device is attached to which peripheral.
	//
	void gmp_init_peripheral_tree(void);

	// This function should init all the peripherals.
	//
	void gmp_setup_peripheral(void);

	// Show GMP setup label
	// This function contain a label show of GMP.
	// 
	void gmp_setup_label(void);

	// This function is the last function before mainloop.
	// This function is implemented by CSP
	void csp_post_process(void);

	// GMP Exit routine
	// This function should be implemented by CSP submodule.
	// And generally, this function is a null function.
	// 
	void gmp_exit_routine(void);


	// ....................................................................//
	// Error Handle function
	//

	// This function would be called when system meets error.
	// In order to referenced by error handling,
	// the function prototype is redefined in `error_code.h`
	// 
	void gmp_system_stuck(void);

	// When a function is unimplemented, the function would be invoke.
	//
	void gmp_not_impl(const char* file, uint32_t line);

	// ....................................................................//
	// GMP default initialization function and loop function 
	//

	// This function is the GMP loop function,
	// `user_loop()` would be called in this function,
	// and other GMP deposit would be invoked in this function.
	//
	void gmp_loop(void);

	// This function is the GMP initialization function,
	// `user_loop()` would be called in the function,
	// and other GMP deposit would be invoked in this function.
	//
	void gmp_init(void);


	//////////////////////////////////////////////////////////////////////////
	// Step III: Lab functions
	//

	// For test, user should enable and implement the following function.

	// This function would implement all the initialization test routines.
	//
	void gmp_test_init(void);

	// This function would implement all the loop test routines.
	//
	void gmp_test_loop(void);


	//////////////////////////////////////////////////////////////////////////
	// Step IV: Default Memory Management functions
	//

	// The memory controller functions
	//
	void* gmp_malloc(size_gt size);
	void gmp_free(void* ptr);

	// Default heap memory set
	//
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
	GMP_MEM_ALIGN
		extern data_gt default_heap[GMP_DEFAULT_HEAP_SIZE];
#endif



#ifdef SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

	// debug print function prototype
	//
	size_gt gmp_dbg_prt_fn(const char* p_fmt, ...);

#endif // SPECIFY_ENABLE_DEFUALT_DEBUG_PRINT_FUNC

	// debug print function alias
	// user should invoke this function instead of any direct function name.
	//
#ifndef gmp_dbg_prt
#define gmp_dbg_prt(x, ...)
#endif// debug output mode disabled


	//////////////////////////////////////////////////////////////////////////
	//Step V: global variables
	// The following variables will be defined in <core/std/global.c>
	//

	// ....................................................................//
	// delay parameters
	//

	// Timeout counter
	//
	extern size_gt g_delay;

	// Timeout timer
	//
	extern size_gt g_delay_ms;


	//////////////////////////////////////////////////////////////////////////
	// Step VI: big-endian to little-endian
	//
	 
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


	// Specify the alias of Little endian and little endian data.
	//
#ifdef _MSC_VER // This feature is supported by Visual Studio.
#pragma region BIG_LITTLE_ENDIAN_ALIAS
#endif // _MSC_VER

		// Specify the chip is Big-endian 
	//
#ifdef BIG_ENDIAN

	// Specify the data is Big-endian 12-bit data
	//
#define BE12(x) ((x))

	// Specify the data is Big-endian 16-bit data
	//
#define BE16(x) ((x))

	// Specify the data is Big-endian 32-bit data
	//
#define BE32(x) ((x))

#ifdef SPECIFY_ENABLE_INTEGER64
	// Specify the data is Big-endian 64-bit data
	//
#define BE64(x) ((x))
#endif // SPECIFY_ENABLE_INTEGER64

	// Specify the data is Little-endian 12-bit data
	//
#define LE12 L2B12

	// Specify the data is Little-endian 16-bit data
	//
#define LE16 L2B16

	// Specify the data is Little-endian 32-bit data
	//
#define LE32 L2B32

#ifdef SPECIFY_ENABLE_INTEGER64
	// Specify the data is Little-endian 64-bit data
	//
#define LE64 L2B64
#endif // SPECIFY_ENABLE_INTEGER64

#endif // BIG_ENDIAN

	// Specify the chip is Little-endian
	//
#ifdef LITTLE_ENDIAN

	// Specify the data is Big-endian 12-bit data
	//
#define BE12 L2B12

	// Specify the data is Big-endian 16-bit data
	//
#define BE16 L2B16

	// Specify the data is Big-endian 32-bit data
	//
#define BE32 L2B32

#ifdef SPECIFY_ENABLE_INTEGER64
	// Specify the data is Big-endian 64-bit data
	//
#define BE64 L2B64
#endif // SPECIFY_ENABLE_INTEGER64

	// Specify the data is Little-endian 12-bit data
	//
#define LE12(x) ((x))

	// Specify the data is Little-endian 16-bit data
	//
#define LE16(x) ((x))

	// Specify the data is Little-endian 32-bit data
	//
#define LE32(x) ((x))

#ifdef SPECIFY_ENABLE_INTEGER64
	// Specify the data is Little-endian 64-bit data
	//
#define LE64(x) ((x))
#endif // SPECIFY_ENABLE_INTEGER64

#endif // LITTLE_ENDIAN

#ifdef _MSC_VER
#pragma endregion BIG_LITTLE_ENDIAN_ALIAS
#endif // _MSC_VER






#ifdef __cplusplus
}
#endif


#endif // _FILE_GLOBAL_H
