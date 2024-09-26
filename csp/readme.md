# Chip Support Package

describe chip features

Unified device function name

All the C source file should contain the `<core/gmp_core.h>` folder.
This file contains all the C interface of GMP library.
That will not destroy your C

``` C++
#include <core/gmp_core.h>
```

If your project is a C++ project, and has main.cpp, you may directly include the `<gmp_core.h>`

``` C++
#include <core/gmp_core.hpp>
```



You may support your own Chip Support Package, via example.

Each Chip support package should contains at least 4 header files. They are

| file              | summary                                                      |
| ----------------- | ------------------------------------------------------------ |
| `csp.config.h`    | This header file may config the GMP library to enable or disable some parts. And validate user configurations. |
| `csp.general.h`   | This header file may include the chip support related files or headers. |
| `csp.general.hpp` | This header file may include the chip support related files or headers for C++. |
| `csp.typedef.h`   | This header file provide chip related typedef.               |

