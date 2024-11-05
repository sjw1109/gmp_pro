
#include <core/std/gmp.std.h>

// This function should be called when Chip setup is completed.
void gmp_base_entry(void)
{
#ifdef SPECIFY_PC_ENVIRONMENT
    uint32_t loop_tick;
#endif // SPECIFY_PC_ENVIRONMENT

#ifndef SPECIFY_DISABLE_CSP
    // CSP Startup function
    //
    gmp_csp_startup();
#endif // SPECIFY_DISABLE_CSP

    // platform related function, initialize peripheral
    // This function is defined by user.
    // if CSP is disabled, user should implement chip setup routine
    // in this function.
    //
    setup_peripheral();

    // TODO: setup GMP library memory management module
    //

#ifndef SPECIFY_DISABLE_GMP_LOGO
    // Debug information print
    //
    gmp_base_show_label();
#endif // SPECIFY_DISABLE_GMP_LOGO

#if defined SPECIFY_ENABLE_GMP_CTL
    // Call CTL(Controller template library) initialization function
    //
    ctl_init();

#endif // SPECIFY_ENABLE_GMP_CTL

    // Call user initialization function
    //
    init();

#ifndef SPECIFY_DISABLE_CSP
    // latest function before Main loop, CSP may use this function to enable interrupt.
    //
    gmp_csp_post_process();
#endif // SPECIFY_DISABLE_CSP

#if defined SPECIFY_ENABLE_GMP_CTL
#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
    ctl_fm_controller_inspection(ctl_nano_handle);
#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
#endif // SPECIFY_ENABLE_GMP_CTL

#ifdef SPECIFY_PC_ENVIRONMENT
    // PC simulate environment, finite iteration
    for (loop_tick = 0; loop_tick < PC_ENV_MAX_ITERATION; ++loop_tick)
#else  // SPECIFY_PC_ENVIRONMENT
    // real processor routine
    for (;;)
#endif // SPECIFY_PC_ENVIRONMENT

    {

#ifndef SPECIFY_DISABLE_CSP
        // Call GMP CSP module loop routine
        //
        gmp_csp_loop();

#endif // SPECIFY_DISABLE_CSP

#if defined SPECIFY_ENABLE_GMP_CTL
#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
        ctl_fm_state_dispatch(ctl_nano_handle);
#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
#endif // SPECIFY_ENABLE_GMP_CTL

        // Call user general loop routine
        //
        mainloop();

#if defined SPECIFY_ENABLE_GMP_CTL
        // Call controller loop routine
        //
        ctl_mainloop();
#endif // SPECIFY_ENABLE_GMP_CTL
    }

    // This function is unreachable.
    gmp_csp_exit();
}

// This function should be called when System fatal error happened.
void gmp_base_system_stuck(void)
{
}

GMP_BASE_PRINT_DEFAULT_HANDLE_TYPE *default_debug_dev = NULL;

// implement the gmp_debug_print routine.
size_gt gmp_base_print(const char *p_fmt, ...)
{
    size_gt ret = 0;

#if defined SPECIFY_BASE_PRINT_NOT_IMPL
    return ret;
#else

    half_duplex_ift ptr;

    // if no one was specified to output, just ignore the request.
    if (default_debug_dev == NULL)
    {
        return ret;
    }

    size_gt size = (size_gt)strlen(p_fmt);

    static data_gt str[GMP_BASE_PRINT_CHAR_EXT];
    memset(str, 0, GMP_BASE_PRINT_CHAR_EXT);

    va_list vArgs;
    va_start(vArgs, p_fmt);
    vsprintf((char *)str, (char const *)p_fmt, vArgs);
    va_end(vArgs);

    ptr.buf = str;
    ptr.length = (size_gt)strlen((char *)str);
    ptr.capacity = GMP_BASE_PRINT_CHAR_EXT;

    GMP_BASE_PRINT_FUNCTION(default_debug_dev, &ptr);

    ret = ptr.length;

    return ret;

#endif // SPECIFY_BASE_PRINT_NOT_IMPL
}

#ifndef SPECIFY_DISABLE_GMP_LOGO
// This function would print a GMP label
void gmp_base_show_label(void)
{
    gmp_base_print("................................................\r\n");
    gmp_base_print(".....MMMMMMMM.........MM.....,MMM....=MMWWDMN,..\r\n");
    gmp_base_print("...8MM.......M.......MMMM....N8MM ....:MM...MM8.\r\n");
    gmp_base_print("...MMM............. MM..MM?.?N.8M=....:MM..:MM..\r\n");
    gmp_base_print("..=MMD.............=M,..DMM.N..8M=....:MMMMZ,...\r\n");
    gmp_base_print("...MMM.....MMM.....=M,...MMMN..8M=....:MM.......\r\n");
    gmp_base_print("...=MM......M......=M,....MM...8M=....:MM.......\r\n");
    gmp_base_print(".....$M$$$$MMM.....===..........===..=NNMM+.....\r\n");
    gmp_base_print("................................................\r\n");
    gmp_base_print("................................................\r\n");
    gmp_base_print("....General.........Motor............Platform...\r\n");
    gmp_base_print(".........for all Motor & all Platform...........\r\n");
    gmp_base_print("................................................\r\n");
    gmp_base_print("................................................\r\n");
    gmp_base_print("[okay] General motor platform ready.\r\n");
}
#endif // SPECIFY_DISABLE_GMP_LOGO

//////////////////////////////////////////////////////////////////////////
// A function is not implement, this is just a place holder.

void gmp_base_not_impl(const char *file, uint32_t line)
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
// Memory management

// The following function would be called by libraries in the default case.

// unit byte
void *gmp_base_malloc(size_gt size)
{
#if SPECIFY_GMP_DEFAULT_ALLOC == USING_DEFAULT_SYSTEM_DEFAULT_FUNCTION
    return malloc(size);
#elif SPECIFY_GMP_DEFAULT_ALLOC == USING_GMP_BLOCK_DEFAULT_FUNCTION
    return gmp_mm_block_alloc(default_gmp_area_mem_handle, size);
#elif SPECIFY_GMP_DEFAULT_ALLOC == USING_MANUAL_SPECIFY_FUNCTION
    return SPECIFY_GMP_USER_ALLOC(size);
#else //  not implement

#endif // SPECIFY_GMP_DEFAULT_ALLOC
}

void gmp_base_free(void *ptr)
{
#if SPECIFY_GMP_DEFAULT_ALLOC == USING_DEFAULT_SYSTEM_DEFAULT_FUNCTION
    free(ptr);
#elif SPECIFY_GMP_DEFAULT_ALLOC == USING_GMP_BLOCK_DEFAULT_FUNCTION
    gmp_mm_block_free(default_gmp_area_mem_handle, ptr);
#elif SPECIFY_GMP_DEFAULT_ALLOC == USING_MANUAL_SPECIFY_FUNCTION
    return SPECIFY_GMP_USER_FREE(ptr);
#else //  not implement

#endif // SPECIFY_GMP_DEFAULT_ALLOC
}

//////////////////////////////////////////////////////////////////////////
// Weak function definition

#ifdef _MSC_VER
// This function would execute only once.
// User should implement all the initialization code in this function.
//
#pragma comment(linker, "/alternatename:init=gmp_defualt_msvc_init")
void gmp_defualt_msvc_init(void)
{
    // not implement
}

// This function would be the endless loop.
// User should implement all the loop tasks and round-robin tasks.
//
#pragma comment(linker, "/alternatename:mainloop=gmp_defualt_msvc_mainloop")
void gmp_defualt_msvc_mainloop(void)
{
    // not implement
}

// This function should setup all the peripherals.
// In this function the code could be platform related.
//
#pragma comment(linker, "/alternatename:setup_peripheral=gmp_defualt_msvc_setup_peripheral")
void gmp_defualt_msvc_setup_peripheral(void)
{
    // not implement
}

// This function would be implemented in ctl_main.c
// This function would execute only once.
// User should implement all the controller related initialization code in this function.
// That means user init process may isolate with the controller init process.
//
#pragma comment(linker, "/alternatename:ctl_init=gmp_defualt_msvc_ctl_init")
void gmp_defualt_msvc_ctl_init(void)
{
}

// This function would be implemented in ctl_main.c
// This function would be called by main ISR function.
// User should call this function, in your ctl_main.cpp or just ignore it.
// When you need to simulate your controller, this function would be invoked.
// return 0 is normal, and any non-zero value means error.
//
#pragma comment(linker, "/alternatename:ctl_mainloop=gmp_defualt_msvc_ctl_mainloop")
void gmp_defualt_msvc_ctl_mainloop(void)
{
    // not implement
}

// This function would be implemented in ctl_main.c
// This function would be called in every controller loop
// This function would be called by `gmp_base_ctl_step`
//
#pragma comment(linker, "/alternatename:ctl_dispatch=gmp_defualt_msvc_ctl_dispatch")
void gmp_defualt_msvc_ctl_dispatch(void)
{
    // not implement
}

#else // other compiler support weak symbol

// This function would execute only once.
// User should implement all the initialization code in this function.
//
GMP_WEAK_FUNC_PREFIX
void init(void) GMP_WEAK_FUNC_SUFFIX
{
    // not implement
}

// This function would be the endless loop.
// User should implement all the loop tasks and round-robin tasks.
//
GMP_WEAK_FUNC_PREFIX
void mainloop(void) GMP_WEAK_FUNC_SUFFIX
{
    // not implement
}

// This function should setup all the peripherals.
// In this function the code could be platform related.
//
GMP_WEAK_FUNC_PREFIX
void setup_peripheral(void) GMP_WEAK_FUNC_SUFFIX
{
    // not implement
}

// This function would be implemented in ctl_main.c
// This function would execute only once.
// User should implement all the controller related initialization code in this function.
// That means user init process may isolate with the controller init process.
//
GMP_WEAK_FUNC_PREFIX
void ctl_init(void) GMP_WEAK_FUNC_SUFFIX
{
}

// This function would be implemented in ctl_main.c
// This function would be called by main ISR function.
// User should call this function, in your ctl_main.cpp or just ignore it.
// When you need to simulate your controller, this function would be invoked.
// return 0 is normal, and any non-zero value means error.
//
GMP_WEAK_FUNC_PREFIX
void ctl_mainloop(void) GMP_WEAK_FUNC_SUFFIX
{
    // not implement
}

// This function would be implemented in ctl_main.c
// This function would be called in every controller loop
// This function would be called by `gmp_base_ctl_step`
//
GMP_WEAK_FUNC_PREFIX
void ctl_dispatch(void) GMP_WEAK_FUNC_SUFFIX
{
    // not implement
}

#endif // other compiler support weak symbol