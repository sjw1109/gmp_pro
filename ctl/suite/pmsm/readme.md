# Create A Single PMSM Controller Projects

This manual help you to create your own PMSM controller based on GMP framework and CTL library.


## Abstract about PMSM Controller 

PMSM controller contains the following modules:

| PMSM module name              | Summary                                                      |
| ----------------------------- | ------------------------------------------------------------ |
| `pmsm.ctl.h`                  | This file contains a speed controller for PMSM. The struct `pmsm_ctl_entity_t` contains a pure PMSM Speed controller. |
| folder: pmsm_speed_close_loop | This folder contains a wrapper of PMSM controller, based on CTL framework `ctl_nano`. |



## PMSM CONTROLLER ENTITY





## PMSM controller interface

PMSM controller may run in open-loop mode, current mode and speed mode. 



## Quick Start

In order to create a PMSM controller, you should implement a `pmsm_ctl_object_t` 
