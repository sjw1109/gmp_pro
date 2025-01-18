# Start a DSP C2000 Series projects based on GMP library

## Create a project

New CCS Project

Choose a target, and finally get the chip model name, for instance TMS320F28069.

Give a project name, and select a "Empty Project with a mian.c".

Click "Finish" Button and you will get the project.

## Link some necessary Resources to the project

Right click the project item in the left "Project Explorer" and get into the properties of the project.
And select "Resource" - "Linked Resource".
You will see a path Variables page here.

Now please add the following two alias of path.

| Name | Location | Appendix | 
| ---- | -------- | -------- |
| C2000_Library_LOC | C:\ti\c2000\C2000Ware_3_04_00_00\device_support\f2806x | Please choose where you install the C2000Ware |
| GMP_Library_LOC | E:\lib\gmp_pro | Please choose your location of GMP Library |

## invoke C2000Ware

Come back to the "Project Explorer". We will complete the following steps to invoke the DSP28069 C2000Ware library.

+ Add the "Include Option" of the device

Select "Build" - "C2000 Compiler" - "Include Options" - "Add dir to #include search path" .
And add the following two path.

``` Text
${C2000_Library_LOC}/common/include
${C2000_Library_LOC}/headers/include
```

+ Add the necessary source files of the device.

Close the "Project Explorer".

Right click the project item again, and select "Add files".

You should select all the files (or at least what you really need) in
"C:\ti\c2000\C2000Ware_3_04_00_00\device_support\f2806x\common\source".
And the folder may be decided by your installation location of your C2000Ware.

And then a tiny window will pop up.
You should choose "Link to files" and create link location relative to "C2000_Library_LOC".

![](./img/Select A correct relative locations.jpg)

And add again for the following folder:
"C:\ti\c2000\C2000Ware_3_04_00_00\device_support\f2806x\headers\source", 
owning the same configuration of the last items.

There exist two duplicate files, you should exclude them.
This two files is shown in the following tables.

| The file should be excluded | The file should keep | 
| --------------------------- | -------------------- |
| F2806x_SWPrioritizedDefaultIsr.c | F2806x_DefaultIsr.c |
| F2806x_SWPrioritizedPieVect.c | F2806x_PieVect.c |

+ Add link script of the projects

The whole project need two link scripts.
The project must have one of `F28069.cmd` and `F28069_RAM_lnk.cmd`. 

| Script folders | Script | Description | 
| -------------- | ------ | ----------- | 
| device_support\f2806x\headers\cmd | F2806x_Headers_nonBIOS.cmd | Provide all the definitions of peripheral registers. | 
| device_support\f2806x\common\cmd  | F28069.cmd | Provide all the definitions of FLASH and RAM, and program will stored in FLASH |
| device_support\f2806x\common\cmd  | F28069_RAM_lnk.cmd | Provide all the definitions of FLASH and RAM, and program will stored in RAM |

If you have include two of them, just select one of them and "Right click" the item and select "exclude from the project".

+ Try to compile the project

Now you may try to compile the whole project, and the project should not have any warning or error reports.
And now you get a null project template.

## invoke GMP library

Based on the null project template, we will invoke GMP library.

+ Add the include path
 
Go to the project properties page, and select include options.
Add the GMP library path: `${GMP_Library_LOC}`

+ change language options
 
Select the "Properties" - "Build" - "C2000 Compiler" - "Advanced Options" - "Language Options".
Choose C Dialect and C++ Dialect.

| Options | Details |
| ------- | ------- |
| C Dialect | compile program in C99 mode. |
| C++ Dialect | compile program in C++03 mode. |

+ Add necessary source files


+ call the GMP library


+ try to compile the project

## Preparing to load the program

For now, you still cannot program the board. You should create a new target configuration file first.
You should find the "File" Menu and expand "New" items, and select "New Target Configuration"

And you should select which chip you will use to program.

