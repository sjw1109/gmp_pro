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
#include <ctl/suite/motor_control/pmsm_dual_loop_bare/pmsm_ctrl.h>

#include <ctl/component/interface/pwm_channel.h>

#include <peripheral.h>

#ifndef _FILE_CTL_MAIN_H_
#define _FILE_CTL_MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // speed encoder
    extern spd_calculator_t spd_enc;

    // PMSM servo objects
    // extern pmsm_fm_t pmsm;

    // PMSM const frequency controller
    extern ctl_const_f_controller const_f;

    // PMSM controller
    extern pmsm_bare_controller_t pmsm_ctrl;

    // periodic callback function things.
    GMP_STATIC_INLINE void ctl_dispatch(void)
    {

        ctl_step_const_f_controller(&const_f);

        ctl_step_spd_calc(&spd_enc);

        ctl_step_pmsm_ctrl(&pmsm_ctrl);
    }

#ifndef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

    // periodic interrupt function

#else // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO is defined

//// controller core
// GMP_STATIC_INLINE
// void ctl_fmif_core_stage_routine(ctl_object_nano_t *pctl_obj)
//{
//     // input stage
//     ctl_input_motor_current_ctrl(&pmsm.current_ctrl, &pmsm.iabc_input);
//
//     ctl_step_spd_calc(&spd_enc);
//
//     // constant frequency generator
//     ctl_step_const_f_controller(&const_f);
//
//     // run PMSM servo framework ISR function
//     ctl_step_pmsm_framework(&pmsm);
//
//     // Modulation
//     // Tabc = svpwm(vab) / udc;
//     ctl_ct_svpwm_calc(&pmsm.current_ctrl.vab0, &pmsm.Tabc);
//
//     // Prepare PWM data
//     ctl_calc_pwm_tri_channel(&pmsm.uabc, &pmsm.Tabc);
// }

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifdef __cplusplus
}
#endif // _cplusplus

#endif // _FILE_CTL_MAIN_H_
