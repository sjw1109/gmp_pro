# Core of GMP

This folder contain all core functions of GMP.



## Headers of GMP

GMP core provide two parts of headers. 
One is the c general header. If you need to complete a C programing file, please use the `<core/gmp_core.h>`. 
Another is the C++ style header. If you need to complete a C++ programing file, please use the `<core/gmp_core.hpp>`.

If your program framework is a C framework, you may include `<core/gmp_core.h>`, and call the `gmp_entry();`. 
This function is C style function and the C header will not disturb your project framework.

On the contrary, if your program framework is a C++ framework, you may include `<core/gmp_core.hpp>`, and call the `gmp_entry()` function. This function is `extern "C"`.

GMP library will take over the whole program. You may start your program in `<user/user_main.cpp>`.

## Components of GMP core

GMP core has the following 5 key parts, which are shown in the follow.

| Part Name | Folder  | Comments |
| --------- | ------  | -------  |
| Standardize | `std` | This folder provides all the cross platform support. Such as standard error code, standard compiler macros, and standard GMP typedef. |
| memory management | mm | Internal Memory Management module. |
| Process management | pm | This folder provide a set of tool kits for user to manage their processes. |
| device peripheral unify abstract | dev | This folder provides the standard device peripheral abstract. |
| FGPA support | fpga | This folder provides the GMP related FPGA support Verilog code and HLS code. |
| utilities tools | util | This folder contains a set of utilities. |
