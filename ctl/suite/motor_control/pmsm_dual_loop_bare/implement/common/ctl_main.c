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

#include <ctl/ctl_core.h>
#include <gmp_core.h>

#include <ctrl_settings.h>

#include "peripheral.h"

// PMSM controller
pmsm_bare_controller_t pmsm_ctrl;

// speed encoder
ctl_spd_calculator_t spd_enc;

// PMSM const frequency controller
ctl_const_f_controller const_f;

// CTL initialize routine
void ctl_init()
{
    // create a speed observer by position encoder
    ctl_init_spd_calculator(
        // attach position with speed encoder
        &spd_enc, pmsm_ctrl.mtr_interface.position,
        // set spd calculator parameteers
        CONTROLLER_FREQUENCY, 5, MOTOR_PARAM_MAX_SPEED, 1, 150);

    ctl_setup_const_f_controller(&const_f, 20, CONTROLLER_FREQUENCY);

    // attach a speed encoder object with motor controller
    ctl_attach_mtr_velocity(&pmsm_ctrl.mtr_interface, &spd_enc.encif);

    // set pmsm_ctrl parameters
    pmsm_bare_controller_init_t pmsm_ctrl_init;

    pmsm_ctrl_init.fs = CONTROLLER_FREQUENCY;

    // current pid controller parameters
    pmsm_ctrl_init.current_pid_gain = 2.15f;
    pmsm_ctrl_init.current_Ti = 1.0f / 500;
    pmsm_ctrl_init.current_Td = 0;
    pmsm_ctrl_init.voltage_limit_min = float2ctrl(-0.45);
    pmsm_ctrl_init.voltage_limit_max = float2ctrl(0.45);

    // speed pid controller parameters
    pmsm_ctrl_init.spd_ctrl_div = SPD_CONTROLLER_PWM_DIVISION;
    pmsm_ctrl_init.spd_pid_gain = 3.5f;
    pmsm_ctrl_init.spd_Ti = 1.0f / 100;
    pmsm_ctrl_init.spd_Td = 0;
    pmsm_ctrl_init.current_limit_min = float2ctrl(-0.6);
    pmsm_ctrl_init.current_limit_max = float2ctrl(0.6);

    // accelerator parameters
    pmsm_ctrl_init.acc_limit_min = -150.0f;
    pmsm_ctrl_init.acc_limit_max = 150.0f;

    // init the PMSM controller
    ctl_init_pmsm_bare_controller(&pmsm_ctrl, &pmsm_ctrl_init);

    // BUG TI cannot print out sizeof() result if no type is specified.
    gmp_base_print(TEXT_STRING("PMSM SERVO struct has been inited, size :%d\r\n"), (int)sizeof(pmsm_ctrl_init));

#if (BUILD_LEVEL == 1)
    // ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &const_f.enc);
    ctl_pmsm_ctrl_voltage_mode(&pmsm_ctrl);
    ctl_set_pmsm_ctrl_vdq_ff(&pmsm_ctrl, float2ctrl(0.2), float2ctrl(0.2));

#elif (BUILD_LEVEL == 2)
    ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &const_f.enc);
    ctl_pmsm_ctrl_current_mode(&pmsm_ctrl);
    ctl_set_pmsm_ctrl_idq_ff(&pmsm_ctrl, float2ctrl(0.3), float2ctrl(0.1));

#elif (BUILD_LEVEL == 3)

    ctl_pmsm_ctrl_current_mode(&pmsm_ctrl);
    ctl_set_pmsm_ctrl_idq_ff(&pmsm_ctrl, float2ctrl(0.1), float2ctrl(0.05));

#elif (BUILD_LEVEL == 4)

    ctl_pmsm_ctrl_velocity_mode(&pmsm_ctrl);
    ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(0.25));
#endif // BUILD_LEVEL

    // Debug mode online the controller
    ctl_enable_pmsm_ctrl(&pmsm_ctrl);
}

//////////////////////////////////////////////////////////////////////////
// endless loop function here

void ctl_mainloop(void)
{

    int spd_target = gmp_base_get_system_tick() / 100;

    ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(0.1) * spd_target - float2ctrl(1.0));

    // User Controller logic here.
    //if (gmp_base_get_system_tick() > 350)
    //{
    //    ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(-0.5));
    //}

    //else if (gmp_base_get_system_tick() > 250)
    //{
    //    ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(0));
    //}

    //else if (gmp_base_get_system_tick() > 150)
    //{
    //    ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(0.5));
    //}

    return;
}

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

void ctl_fmif_monitor_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
}

// return value:
// 1 change to next progress
// 0 keep the same state
fast_gt ctl_fmif_sm_pending_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}

// return value:
// 1 change to next progress
// 0 keep the same state
fast_gt ctl_fmif_sm_calibrate_routine(ctl_object_nano_t *pctl_obj)
{
    return ctl_cb_pmsm_frmework_current_calibrate(&pmsm);
}

fast_gt ctl_fmif_sm_ready_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}

// Main relay close, power on the main circuit
fast_gt ctl_fmif_sm_runup_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 1;
}

fast_gt ctl_fmif_sm_online_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}

fast_gt ctl_fmif_sm_fault_routine(ctl_object_nano_t *pctl_obj)
{
    // not implement
    return 0;
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
