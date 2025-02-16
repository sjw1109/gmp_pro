# GMP CTL PMSM Motor Control Suite



## Abstract

BOOSTXL-3PHGANINV (48-V Three-Phase Inverter With Shunt-Based In-Line Motor Phase Current Sensing Evaluation Module) is used to implement a Motor Driver. You may find related notes here: https://www.ti.com/tool/BOOSTXL-3PHGANINV

### Current & Voltage Measurement 

1. Current Measurement

Current Measurement using a $0.005 \,\Omega$ resister, and INA240A1 provide 20 gain.

2. Voltage Measurement

Voltage Measurement gain is 
$$
A_v = \frac{4.22 \,k\Omega}{100 \,k\Omega + 4.22 \,k\Omega} = 0.04049127
$$


Meanwhile, Voltage filter is cut frequency is 
$$
f_{vc} = \frac{1}{2\pi RC}=\frac{1}{2\pi \times 4.22 \,k\Omega \times 0.033 \,\mu F}= 1.1434\,kHz
$$

3. Motor Parameters

The target motor is a PMSM motor, which parameter is stored in  `ctl/component/motor_control/preset_motor_param/GBM2804H_100T.h`



## Encoder Hardware

The system use [AS5048A](https://docs.rs-online.com/0657/A700000006921305.pdf) as absolute position encoder. This is a 14-bit rotary position sensor with SPI interface. 



## Software Structure

GMP CTL motor_control suite owns a PMSM controller with absolute position encoder based on CTL nano framework. Main parts is defined in `pmsm_servo.h`, some initialization code is defined in `src/pmsm_servo.c` .

A set of invoke examples is defined in `implement` folder. `impelement/user_common` owns basic usage of `pmsm_servo`, and these code are unrelated to MCU. Other code related with running platform is defined in each folder.

| Folder        | Platform                                                     |
| ------------- | ------------------------------------------------------------ |
| user_f28x     | [Launch Pad F280039](https://www.ti.com/tool/LAUNCHXL-F280039C) |
| user_simulink | GMP SIL                                                      |
| user_stm32    | [NUCLEO-G474RE](https://www.st.com/en/evaluation-tools/nucleo-g474re.html) |



 You may change motor config in `impelement/user_common/ctl_main.h`.

| MACROS               | Notes                                                    |
| -------------------- | -------------------------------------------------------- |
| BUILD_LEVEL          | Select Build Level. Open loop, Current loop, Speed loop. |
| CONTROLLER_FREQUENCY | Select Controller Frequency.                             |

You may change PID config in `impelement/user_common/ctl_main.c`, the time invoke function `ctl_setup_pmsm_servo_framework`.





## GMP SIL Simulation Environment

GMP SIL Project: `gmp_pro\ctl\suite\motor_control\pmsm\projects\motor_control_simulink`

You may open the `motor_control_simulink.sln` using Visual Studio 2022, and `gmp_pmsm_sil_mdl.slx` using MATLAB Simulink 2024 or later.

> NOTICE: 
>
>  If you have not install GMP CTL Simulink Library, you should install this library firstly, by running `gmp_pro\slib\install_gmp_simulink_lib.m`

You should run Visual Studio Solution firstly, and then run the simulation model.

 You may change `ctl_fmif_output_stage_routine` function in `implement\user_simulink\ctl_interface.h`monitor routine to spy on the key variables.



## C28x Motor Control Environment

Project Location: `gmp_pro\ctl\suite\motor_control\pmsm\projects\motor_control_c28x_example`

You may import the project by CCS12.

If there're some product don't exist, you should run `gmp_pro\tools\facilities_generator\gmp_fac_install.bat`to install these TI CCS product.

device peripheral config

Encoder SPI port, Mode 

| Signal          | Peripheral | GPIO Index |
| --------------- | ---------- | ---------- |
| SPI CS          | GPIO       | GPIO48     |
| SPI SIMO (PICO) | SPIA       | GPIO8      |
| SPI SOMI (POCI) | SPIA       | GPIO17     |
| SPI CLK         | SPIA       | GPIO9      |

print SCI port, baud rate 115200 bps

| Signal  | Peripheral | GPIO Index |
| ------- | ---------- | ---------- |
| SCIA RX | SCIA       | GPIO28     |
| SCIA TX | SCIA       | GPIO29     |

EPWM channel, 10 kHz PWM 

| Signal  | Peripheral | GPIO index |
| ------- | ---------- | ---------- |
| PWM U H | ePWM1      | GPIO0      |
| PWM U L | ePWM1      | GPIO1      |
| PWM V H | ePWM2      | GPIO2      |
| PWM V L | ePWM2      | GPIO3      |
| PWM W H | ePWM6      | GPIO10     |
| PWM W L | ePWM6      | GPIO11     |





