/**
 * @file ctl_main.cpp
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// const F & VF controller
#include <ctl/component/motor_control/basic/vf_generator.h>

// speed encoder
#include <ctl\component\motor_control\basic\encoder.h>

// motor controller
#include <ctl/suite/mcs_pmsm/pmsm_ctrl.h>

#include <ctl/component/interface/pwm_channel.h>

#include <ctl/component/interface/adc_channel.h>

#include <xplt.peripheral.h>

#include <ctl/component/intrinsic/discrete/stimulate.h>

#include <ctl/component/digital_power/topology_preset/single_phase_dc_ac.h>


#ifndef _FILE_CTL_MAIN_H_
#define _FILE_CTL_MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// controller objects
extern sinv_ctrl_t sinv_ctrl;

// periodic callback function things.
GMP_STATIC_INLINE
void ctl_dispatch(void)
{
    ctl_step_sinv(&sinv_ctrl);
}

#ifdef __cplusplus
}
#endif // _cplusplus

#endif // _FILE_CTL_MAIN_H_
