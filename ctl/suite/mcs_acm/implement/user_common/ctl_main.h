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

#ifndef _FILE_CTL_MAIN_H_
#define _FILE_CTL_MAIN_H_

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


//
//#include <gmp_core.h>
//
//#include <ctl/ctl_core.h>

#include <ctl/suite/motor_control/pmsm/pmsm_servo.h>

#include <ctl/component/motor_control/basic/encoder.h>

// const F & VF controller
#include <ctl/component/motor_control/basic/vf_generator.h>

// BUILD_LEVEL 1: Voltage Open loop
// BUILD_LEVEL 2: Current Open loop
// BUILD_LEVEL 3: Current Open loop with actual position
// BUILD_LEVEL 4: Speed Close loop
#define BUILD_LEVEL (1)

// position encoder
extern ctl_pos_encoder_t pos_enc;

// speed encoder
extern ctl_spd_calculator_t spd_enc;

// PMSM servo objects
extern pmsm_servo_fm_t pmsm_servo;

// PMSM const frequency controller
extern ctl_const_f_controller const_f;





#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

// other periodic things.
GMP_STATIC_INLINE
void ctl_dispatch(void)
{

}

// controller core
GMP_STATIC_INLINE
void ctl_fmif_core_stage_routine(ctl_object_nano_t *pctl_obj)
{
    // input stage
    ctl_input_motor_current_ctrl(&pmsm_servo.current_ctrl, &pmsm_servo.iabc_input);
    
    ctl_step_spd_calc(&spd_enc);

    // constant frequency generator
    ctl_step_const_f_controller(&const_f);

    // run PMSM servo framework ISR function
    ctl_step_pmsm_servo_framework(&pmsm_servo);

    // Modulation
    // Tabc = svpwm(vab) / udc;
    ctl_ct_svpwm_calc(&pmsm_servo.current_ctrl.vab0, &pmsm_servo.Tabc);

    // Prepare PWM data
    ctl_calc_pwm_tri_channel(&pmsm_servo.uabc, &pmsm_servo.Tabc);
}


#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifdef __cplusplus
}
#endif // _cplusplus

#endif // _FILE_CTL_MAIN_H_

