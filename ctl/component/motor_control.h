// 把所有的电机控制相关的头文件全部引入

// basic includes

#include <ctl/ctl_core.h>


#ifndef GMP_CTL_DISABLE_COM_MC

#ifndef GMP_CTL_DISABLE_COM_MC_BASIC

#include <ctl/component/motor_control/basic/encoder.h>

//#include <ctl/component/motor_control/basic/encoder_calibrate.h>

#include <ctl/component/motor_control/basic/encoder_if.h>

#include <ctl/component/motor_control/basic/svpwm.h>

#include <ctl/component/motor_control/basic/vf_generator.h>

#endif // GMP_CTL_DISABLE_COM_MC_BASIC

#ifndef GMP_CTL_DISABLE_COM_MC_CONSULTANT

#include <ctl/component/motor_control/consultant/motor_driver_consultant.h>

#include <ctl/component/motor_control/consultant/pmsm_consultant.h>

#endif // GMP_CTL_DISABLE_COM_MC_CONSULTANT

#ifndef GMP_CTL_DISABLE_COM_MC_CURRENT_LOOP

#include <ctl/component/motor_control/current_loop/dpcc.h>

#include <ctl/component/motor_control/current_loop/dtc.h>

//#include <ctl/component/motor_control/current_loop/hyh_DPC_current_ctrl.h>

#include <ctl/component/motor_control/current_loop/motor_current_ctrl.h>

#include <ctl/component/motor_control/current_loop/mtpv_ref.h>

#endif // GMP_CTL_DISABLE_COM_MC_CURRENT_LOOP

#ifndef GMP_CTL_DISBALE_COM_MC_MOTION

#include <ctl/component/motor_control/motion/basic_pos_loop_p.h>

#include <ctl/component/motor_control/motion/knob_pos_loop.h>

#include <ctl/component/motor_control/motion/motion_formulate.h>

#endif // GMP_CTL_DISBALE_COM_MC_MOTION

#ifndef GMP_CTL_DISABLE_COM_MC_OBSERVER

#include <ctl/component/motor_control/observer/im.fo.h>

#include <ctl/component/motor_control/observer/pmsm.esmo.h>

#include <ctl/component/motor_control/observer/pmsm.fo.h>

#include <ctl/component/motor_control/observer/pmsm.hfi.h>

#include <ctl/component/motor_control/observer/pmsm.smo.h>

//#include <ctl/component/motor_control/observer/param_esti_RLS.h>

#endif // GMP_CTL_DISABLE_COM_MC_OBSERVER

#endif // GMP_CTL_DISABLE_COM_MC
