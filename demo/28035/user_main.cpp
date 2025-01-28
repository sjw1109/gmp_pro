/**
 * @file user_main.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
//////////////////////////////////////////////////////////////////////////
// invoke headers

// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <gmp_core.hpp>
#include <core/util/ds/data_ring_buffer.h>

// extensions

// Controller Template Library


//////////////////////////////////////////////////////////////////////////
// global variables here



//////////////////////////////////////////////////////////////////////////
// initialize routine here



GMP_NO_OPT_PREFIX
void user_init(void)
GMP_NO_OPT_SUFFIX
{


}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
GMP_NO_OPT_PREFIX
void user_loop(void)
GMP_NO_OPT_SUFFIX
{
for (int i =0; i< 10000; ++i);

gmp_dbg_prt("Hello World!\r\n");
}


//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here


