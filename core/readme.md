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
