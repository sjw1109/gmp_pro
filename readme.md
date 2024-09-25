# General Motor Platform (GMP) library manual summary

GMP is a easy-going library which may help you implement a controller easily.
GMP contains a set of tools for user, you may find then in specified folders.

| Folders | Summary |
| ------- | :------- |
| core/   | This folder provide a set of basic components for the whole library and user. Such as, workflow and it's scheduling, memory management, and abstract of IO devices and etc.. |
| csp/    | This folder contains all the chip support informations. This predefinitions may help you use the library more easily |
| ctl/    | The controller template library. This folder provides not only basic components but also a whole host of controller and it's workflow agent. |
| ext/    | The extension module of the library. This folder contains a lot of devices based on the GMP core. These extensions may help you deploy application easily and rapidly. |
| user/   | The folder is for user to implement their applications. Generally, this folder provide a template for user, and user may copy them to their own project src folder, and change them. |

## Quick Start

Start a project with GMP library is quite a easy routine. You may follow the four steps.
Notice, this section of quick start may help you start your project based on GMP framework.

### prepare all the GMP related files

All the GMP file contains three parts, shown as follow:

+ User Files (these files would be modified)

All the user related files are saved in `<root>/user` folder. Some application examples may provide some user files. If you have the same running environment you can use them directly.

User files including the following files:

| File name                    | Summary                                                      |
| ---------------------------- | :----------------------------------------------------------- |
| user_main.cpp                | User may implement his application in this file. This file provide two key functions, `user_init`, and `user_loop`.  When GMP framework take over the whole program, the `user_init` function may be called once, after all the necessary GMP initialization; `user_loop` function may be invoked over and over till the program is killed. |
| user_main.hpp                | User may add your own extern variables or extern functions in this file. This file may only visible for `user_main.cpp` and `ctl_main.cpp`. |
| ctl_main.cpp<br />(optional) | Controller implement source file. If you will implement a Controller based on Controller Template Library, we strongly suggest you to implement your necessary top level controller code in this file. |
| user.config.h                | This header file may be the first one include file for the whole GMP framework. This files only contains a set of configuration macros, in order to enable or disable some modules. |
| peripheral.cpp               | This folder provide a set of functions which would be called by GMP framework. These functions may help you implement your peripheral setup routine on time. |

Based on the previous content, you may copy a user file template to your own project. Then you may add a folder (or file filter, or file  groups) in your own project and add these files in your project manager.

+ GMP Framework

GMP Framework contains two folders in GMP library. One is `core` and another is `csp`.

>  Stage I: Invoke all GMP body code.

In `core` folder, all the GMP framework and basic function is declared and defined. Here're some details

| Sub-folder                     | Full Name                 | Summary                                                      |
| ------------------------------ | ------------------------- | :----------------------------------------------------------- |
| std<br />(necessary)           | Standardization           | This part of code is the basement of the GMP framework.      |
| mm<br />(optional)             | Memory Management         | This part of code is Memory management for GMP code. When any GMP related submodule need to allocate or free some heap memory this submodule may be invoked. <br />This module may be disabled in your config file. |
| dev<br />(provided by headers) | Device Abstract           | This folder provide a set of standard interface for peripheral communication based on bus. Chip Support Package or User defines a set of functions to compatible these standards may helps user use GMP- Extension and GMP-FPGA module easier. Because these extension and FPGA interface comply with these abstract interfaces. |
| pm<br />(optional)             | Process Management        | This folder provide a set of tool kits for user to manage their process. Including three kernel parts, state machine standard, workflow standard, message mapping standard. |
| fpga<br />(optional)           | FPGA support relate       | This folder provides a set of FPGA peripherals. These peripheral may be add to your heterogeneous designs. |
| util<br />(optional)           | utilities provided by GMP | Some data structure and algorithm and some other utilities including in this folder. |

Generally, and at least you have to add two files in your project, which is `error_code.c` and `gmp_main.cpp` which are stored in `<gmp root>/core/std`. If memory management is necessary you should add the `block_mem.c` in `core/mm` folder. These files are stored in `\core\src` folder.

The  folder `<gmp root>/core/std`, provides all the necessary supports which is need by GMP, including four parts, that is, compiler standardization in `compiler_sup.h`, type name standardization in `default.types.h`, error code standardization in `error_code.h`, GMP basic port functions in `gmp_cport.h`.  

> Stage II include all the Chip Support Files

Another part of GMP core is Chip Support Package files. There're several functions and necessary function are defined in CSP submodule.

You may find which chip you use. For instance my chip is STM32G431RB from  STMicroelectronics. I need to add `csp/stm32/stm32_chip_common.c` to my projects.

+ GMP CTL (Controller Project Only) 

If you need to create a GMP controller project, the GMP Controller Template Library is necessary for your projects.

CTL module has five kernel folders, that is:

| CTL folder | Summary                                                      |
| ---------- | ------------------------------------------------------------ |
| component  | This folder contains a set of components for user and CTL suite module to construct controllers. In this folder, all the components are stored based on it's application. |
| framework  | This folder contains some standard framework which help user construct their own controller easier. |
| objects    | This folder contains a set of controlled plants model.       |
| suite      | This folder contains several complete suite of controller. Such as PMSM controller,  switching regulator controller. |
| FPGA       | This folder contains a list of FPGA peripherals for controller. |

All the source files are stored in `src` folders. User should add all the necessary components and framework source to your projects. In general, the controller framework is necessary, `ctl_nano.c`, stored in `ctl/framework/src`, and common component and interface_utils is necesaary, `ctl_common_init.c` and `peripheral_if_util_init.c`.

If you will use GMP CTL suite controller, for  instance, PMSM controller. you may add `ctl_motor_init` for motor related components, and `pmsm.ctl.c` as PMSM controller basic implement, and `pmsm_speed_closeloop.c` as PMSM speed controller.

+ Third parties and GMP Extensions

If the chip you use is not support the IQ math, you may use the TI IQMATH open source version. which is provided in `third_party`.

You should add all the sources files of IQ math library, and add correct include path.

If some extension is needed, you may search some more help in the extension folder.

### Add Correct Include searching path to your projects

All the GMP files are organized and use relative path based on GMP library root path. So what user need to do is tell  IDE where your own config files are, and where GMP root path are.

For instance, if  I've install GMP in my `E:\lib\gmp` folder. Then I create a project in `E:\project_based_on_GMP`, and all the user files are copied to `E:\project_based_on_GMP\user`. For now, I should add the following path into Include Path.

+ user file path, so that the GMP library may find out the user config file `user.config.h`.

`E:\project_based_on_GMP\user`

+ GMP library, so that the GMP library modules may find all the header files based on this path.

`E:\lib\gmp`

+ Select correct GMP CSP (Chip Support Package) 

`E:\lib\gmp\csp\stm32`

### Customize your own GMP via user.config.h

Generally, you may add only one line to `user.config.h` all the configure may automatically added. 

`#define MASTERCHIP GMP_AUTO_STM32`

For now, the project with GMP can compile. You may try it.

### Let GMP library running in your projects

Now invoke GMP, and let GMP framework running.

If your main source file is a C source file, please add the `core/gmp_core.h` just like the following code.

``` C
#include <core/gmp_core.h>
```

If your main source file is a C++ source file, please add the `core/gmp_core.hpp` just like the following code.

``` C++
#include <cire/gmp_core.hpp>
```

For now, you may invoke the `gmp_entry()` function in your main function. just like the following code does.

For now, you may invoke the `gmp_entry()` function in your main function. just like the following code does.

``` C++
void main (void)
{
	// Do your preparing code

	// ...

	// Ready to entry the GMP
	gmp_entry(); // invoke the GMP library

	// Some other things.
	// But code NEVER reach here.
}
```

> Attention! when `gmp_entry()` is called, the function may not returned. Generally this function should be called after all the initialization code is completed, and 



### Enjoy!

Now you can implement your application based on GMP framework. These code may added in `ctl_main.cpp` and `user_main.cpp`.





### compliment your GMP library configure

This file is located in `<.config.h>`, user may open this file, and change it based on what you need.
Worth mentioning, you should firstly pay attention to the first section of the file.
That is, you should tell the correct `MASTERCHIP`, `MASTER_CHIP_SET` and `COMPILER_TYPE`.

Some example for chip set or board may provided in user folder.
You may just copy and cover the `<.config.h>` file.

And then change your compiler settings, let your compiler support C++11 standard.

> NOTICE: the C++ compiler should support at least C++03 version.
> Using compiler supporting C++11 will greatly good at performance and compiler comments.
> The compatible definition of C++11 is defined here `<core/std/compiler_sup.h>`.
>

### Add the GMP source to your projects

You should add all the source file (*.c and *.cpp) to compiler candidate.
By the way, you need to add the location of GMP library root PATH to your include path.
All the include command is write using relative path, based on the GMP library root path.


### Call the `gmp_entry()` function and entry the GMP world

You need to call the `gmp_entry()` function, in your main function, and sometimes you may need to complete a set of preparing firstly, for instance the code generated by CubeMX.
Aiming to let program know the prototype of `gmp_entry()`, firstly,  you should add a include command in your main source file.

If your main source file is a C source file, please add the `core/gmp_core.h` just like the following code.

``` C
#include <core/gmp_core.h>
```

If your main source file is a C++ source file, please add the `core/gmp_core.hpp` just like the following code.

``` C++
#include <cire/gmp_core.hpp>
```

For now, you may invoke the `gmp_entry()` function in your main function. just like the following code does.

``` C++
void main (void)
{
	// Do your preparing code

	// ...

	// Ready to entry the GMP
	gmp_entry(); // invoke the GMP library

	// Some other things.
	// But code NEVER reach here.
}
```

### Final step: just enjoy!

You may start your own work in the `<user/>` folder.



