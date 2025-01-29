/**
 * @file gmp_core.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#ifndef _FILE_GMP_CORE_HEADER_H_
#define _FILE_GMP_CORE_HEADER_H_

// This file is GMP library C source header

// Basic GMP standard type definition
#include <core/std/gmp.std.h>

// TI fixed library
#ifdef ENABLE_IQMATH_HEADER_DIREDCT
#ifndef __IQMATHLIB_H_INCLUDED__

#include <third_party\iqmath\IQmathLib.h>
// #include "sw/modules/iqmath/src/32b/IQmathLib.h"
#endif
#endif // USING_FIXED_LIBRARY

//////////////////////////////////////////////////////////////////////////
// Step III GMP basement
//

// <csp> default chip type definition
// #include <csp/chip_port.h>

// default peripheral types
// This header may be seized in `chip_port.h`
// #include <core/std/default_peripheral.config.h>

// public C ports function
// memory management support
// #include <core/std/gmp_cport.h>

// public C CSP ports functions
// peripheral functions
// #include <core/std/gmp_csp_cport.h>

//////////////////////////////////////////////////////////////////////////
// Step IV other C core modules
//

// (MM) Memory Management module
#if defined SPECIFY_GMP_BLOCK_MEMORY_ENABLE
#include <core/mm/block_mem.h>
#endif // SPECIFY_GMP_BLOCK_MEMORY_ENABLE

// + (WF) Workflow module

// + (SCH) Scheduling module

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // This function would be called by user in entry function.
    // And this function would not return.
    //
    // You may find definition of this function in `gmp_std_port.c`.
    //
    //extern void gmp_base_entry(void);

    // This function would be called by main ISR function, by user.
    // User should call this function, in your ctl_main.cpp or just ignore it.
    // When you need to simulate your controller, this function would be invoked.
    //
    // You may find definition of this function in `gmp_cport.h`.
    //
    //extern void gmp_base_ctl_step(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GMP_CORE_HEADER_H_




//                     ..';;;;,
//               .,ooKWMXxKMMN.
//            'lKMMW:cMNk,:ddl
//          cXMlckWk;   .',. ..
//        lWMMMW;  .:d0NN'X: ,d
//      .KOcxNd. cONNNNNN.0: ,d      ;o,
//     cWMM0:  lNk;:XNNNN.O: ,d    ;xkkkx;
//    lWxXKd ,XNNc..0NNNXKX: ,kdockkkkkko'
//   ,MMKO' :NNNNNNNNNX0l,.    .,okkkkl.
//   XWXWX .XXXNNNNNXK,  .,...,.  .dkx.                'cNdoodl
//  ,WxOc, 0occcccONK  .c.  .. .c.  oko.......       ..xdll:lll,
//  oMMMM' ,,,,,,,,,.  o         x   '''''''''      .'.,0WK,lMMk
//  xMMMM' ddclodddd,  o         x  ,dddddoood.    lXMx;OMWoKMMX'
//  cWd:,, WX''''lMMN.  c.     .c. .X0o;.'',dN  .xcxxMM0dol:llll.
//  .MMMMX lWllxKWMMMN:  .'''',.  lk.';,,;;;0o ,WMMXXd.
//   xMXo;, kMNx:..XMMMXd;.   .;ox'Ok;,';c:ok .o0XMX.
//    KWOKMd kK. :XMKoKMMMM: oNNo'd.c:;c;;ck lMX0X0
//    .NMM0O..;NWMNc  0Mo:M: oNd.x.x.d.;0Xc.'xKMMX.
//     .ONkKXWo.lX0'.oMo  M' oX 0.d.d;lKc.lNKxdWO
//       lWMMMM0.''o0WM:.;M. oO X'Kokl'..kMMMMN:
//        .oWMKdOWWk..;:ldk' :xol:;' dNMx;XMNo
//           ;OMMMMockMNx.:xxxx.cNMKooMMMWx:
//             .:xKXMMMMOdKMMMMxoMMMMXKx;
//                  ':oxOOXXXXXKxxo:'
// 
//               ____     __  __     ____  
//              / ___|   |  \/  |   |  _ \ 
//             | |  _    | |\/| |   | |_) |
//             | |_| |   | |  | |   |  __/ 
//              \____|   |_|  |_|   |_|
