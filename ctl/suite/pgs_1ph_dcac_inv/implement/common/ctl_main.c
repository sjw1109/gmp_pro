
//
// THIS IS A DEMO SOURCE CODE FOR GMP LIBRARY.
//
// User should define your own controller objects,
// and initilize them.
//
// User should implement a ctl loop function, this
// function would be called every main loop.
//
// User should implement a state machine if you are using
// Controller Nanon framework.
//

#include <gmp_core.h>

#include <ctrl_settings.h>

#include "ctl_main.h"

#include <xplt.peripheral.h>

#include <ctl/component/intrinsic/continuous/continuous_pid.h>
pid_regular_t current_pid, voltage_pid;
pid_regular_t sinv_vlotage_pid;
ctrl_gt pwm_out_pu;

ctl_single_phase_pll spll;
ctrl_gt ac_input;

ctrl_gt sinv_pwm_pu[2];
pr_ctrl_t sinv_pr_base;
qpr_ctrl_t sinv_qpr_base;
ctrl_gt sinv_current_ref;

ctl_src_rg_t rg;

//
adc_bias_calibrator_t adc_calibrator;
fast_gt flag_enable_adc_calibrator = 0;
fast_gt index_adc_calibrator = 0;

ctrl_gt modulate_target;


ctrl_gt v_set = float2ctrl(0.02);

// enable motor running
volatile fast_gt flag_enable_system = 0;
volatile fast_gt flag_error = 0;

// CTL initialize routine
void ctl_init()
{
    ctl_init_pid(
        // continuous pid handle
        &current_pid,
        // PID parameters
        1, 0.002, 0,
        // controller frequency
        20e3);

    ctl_init_pid(
        // continuous pid handle
        &sinv_vlotage_pid,
        // PID parameters
        0.8, 0.1, 0,
        // controller frequency
        20e3);

    ctl_init_pid(
        // continuous pid handle
        &voltage_pid,
        // PID parameters
        1, 0.01, 0,
        // controller frequency
        20e3);

    ctl_init_single_phase_pll(
        // handle of Single phase PLL object
        &spll,
        // gain of SPLL module
        0.75f/20,
        // Time of integrate
        //0.10f,
        1000.0f,
        // filter cut frequency
        9.0f,
        // Target frequency, grid frequency, Hz
        50.0f,
        // isr frequency, Hz
        20e3);

    ctl_init_pr_controller(&sinv_pr_base, 0.0001, 1000, 50, 20e3);
    ctl_init_qpr_controller(&sinv_qpr_base, 0.00000001, 500, 50, 5 ,20e3);
    ctl_init_qpr_controller(&qpr_test, 0.01, 0.08, 50, 5, 20e3);

    ctl_init_ramp_gen_via_amp_freq(&rg, 20e3, 50, 1, 0);

    //    // setup ADC calibrate
    //    ctl_filter_IIR2_setup_t adc_calibrator_filter;
    //    adc_calibrator_filter.filter_type = FILTER_IIR2_TYPE_LOWPASS;
    //    adc_calibrator_filter.fc = 20;
    //    adc_calibrator_filter.fs = CONTROLLER_FREQUENCY;
    //    adc_calibrator_filter.gain = 1;
    //    adc_calibrator_filter.q = 0.707f;
    //    ctl_init_adc_calibrator(&adc_calibrator, &adc_calibrator_filter);
    //
    // #ifdef PMSM_CTRL_USING_QEP_ENCODER
    //    // init Auto - turn encoder
    //    ctl_init_autoturn_pos_encoder(&pos_enc, MOTOR_PARAM_POLE_PAIRS, MTR_ENCODER_LINES);
    //    // Set encoder offset
    //    ctl_set_autoturn_pos_encoder_offset(&pos_enc, MTR_ENCODER_OFFSET);
    // #endif // PMSM_CTRL_USING_QEP_ENCODER
    //
    //    flag_enable_system = 0;
    //
    //    // create a speed observer by position encoder
    //    ctl_init_spd_calculator(
    //        // attach position with speed encoder
    //        &spd_enc, pmsm_ctrl.mtr_interface.position,
    //        // set spd calculator parameters
    //        CONTROLLER_FREQUENCY, 5, MOTOR_PARAM_MAX_SPEED, 1, 150);
    //
    // #if defined OPENLOOP_CONST_FREQUENCY
    //    ctl_init_const_f_controller(&const_f, 20, CONTROLLER_FREQUENCY);
    // #else  // OPENLOOP_CONST_FREQUENCY
    //    // frequency target 20 Hz, frequency slope 40 Hz/s
    //    ctl_init_const_slope_f_controller(&slope_f, 20.0f, 40.0f, CONTROLLER_FREQUENCY);
    // #endif // OPENLOOP_CONST_FREQUENCY
    //
    //    // attach a speed encoder object with motor controller
    //    ctl_attach_mtr_velocity(&pmsm_ctrl.mtr_interface, &spd_enc.encif);
    //
    //    // set pmsm_ctrl parameters
    //    pmsm_bare_controller_init_t pmsm_ctrl_init;
    //
    //    pmsm_ctrl_init.fs = CONTROLLER_FREQUENCY;
    //
    //    // current pid controller parameters
    //    pmsm_ctrl_init.current_pid_gain = (parameter_gt)(MOTOR_PARAM_LS * MTR_CTRL_CURRENT_LOOP_BW * 2 * PI *
    //                                                     MTR_CTRL_VOLTAGE_BASE / MTR_CTRL_CURRENT_BASE);
    //    pmsm_ctrl_init.current_Ti = (parameter_gt)(MOTOR_PARAM_LS / MOTOR_PARAM_RS);
    //    pmsm_ctrl_init.current_Td = 0;
    //    pmsm_ctrl_init.voltage_limit_min = float2ctrl(-1.0);
    //    pmsm_ctrl_init.voltage_limit_max = float2ctrl(1.0);
    //
    //    // speed pid controller parameters
    //    pmsm_ctrl_init.spd_ctrl_div = SPD_CONTROLLER_PWM_DIVISION;
    //    pmsm_ctrl_init.spd_pid_gain = (parameter_gt)(0.2);
    //    pmsm_ctrl_init.spd_Ti = (parameter_gt)(4.0f / MTR_CTRL_SPEED_LOOP_BW);
    //    pmsm_ctrl_init.spd_Td = 0;
    //    pmsm_ctrl_init.current_limit_min = float2ctrl(-0.45);
    //    pmsm_ctrl_init.current_limit_max = float2ctrl(0.45);
    //
    //    // accelerator parameters
    //    pmsm_ctrl_init.acc_limit_min = -150.0f;
    //    pmsm_ctrl_init.acc_limit_max = 150.0f;
    //
    //    // init the PMSM controller
    //    ctl_init_pmsm_bare_controller(&pmsm_ctrl, &pmsm_ctrl_init);

    // BUG TI cannot print out sizeof() result if no type is specified.
    // gmp_base_print(TEXT_STRING("PMSM SERVO struct has been inited, size :%d\r\n"), (int)sizeof(pmsm_ctrl_init));

    // #if (BUILD_LEVEL == 1)
    // #if defined OPENLOOP_CONST_FREQUENCY
    //     ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &const_f.enc);
    // #else  // OPENLOOP_CONST_FREQUENCY
    //     ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &slope_f.enc);
    // #endif // OPENLOOP_CONST_FREQUENCY
    //
    //     ctl_pmsm_ctrl_voltage_mode(&pmsm_ctrl);
    //     ctl_set_pmsm_ctrl_vdq_ff(&pmsm_ctrl, float2ctrl(0.2), float2ctrl(0.2));
    //
    // #elif (BUILD_LEVEL == 2)
    // #if defined OPENLOOP_CONST_FREQUENCY
    //     ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &const_f.enc);
    // #else  // OPENLOOP_CONST_FREQUENCY
    //     ctl_attach_mtr_position(&pmsm_ctrl.mtr_interface, &slope_f.enc);
    // #endif // OPENLOOP_CONST_FREQUENCY
    //     ctl_pmsm_ctrl_current_mode(&pmsm_ctrl);
    //     ctl_set_pmsm_ctrl_idq_ff(&pmsm_ctrl, float2ctrl(0.1), float2ctrl(0.1));
    //
    // #elif (BUILD_LEVEL == 3)
    //
    //     ctl_pmsm_ctrl_current_mode(&pmsm_ctrl);
    //     ctl_set_pmsm_ctrl_idq_ff(&pmsm_ctrl, float2ctrl(0.1), float2ctrl(0.05));
    //
    // #elif (BUILD_LEVEL == 4)
    //
    //     ctl_pmsm_ctrl_velocity_mode(&pmsm_ctrl);
    //     ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(0.25));
    // #endif // BUILD_LEVEL

    // if in simulation mode, enable system
#if !defined SPECIFY_PC_ENVIRONMENT
    // stop here and wait for user start the motor controller
		ctl_disable_output();
    while (flag_enable_system == 0)
    {
    }

#endif // SPECIFY_PC_ENVIRONMENT

    ctl_enable_output();

    // Debug mode online the controller
    // ctl_enable_pmsm_ctrl(&pmsm_ctrl);

#if defined SPECIFY_ENABLE_ADC_CALIBRATE
    // Enable ADC calibrate
    flag_enable_adc_calibrator = 1;
    index_adc_calibrator = 0;

    // Select ADC calibrate
    // ctl_disable_pmsm_ctrl_output(&pmsm_ctrl);
    ctl_enable_adc_calibrator(&adc_calibrator);
#endif // SPECIFY_ENABLE_ADC_CALIBRATE
}

//////////////////////////////////////////////////////////////////////////
// endless loop function here

uint16_t sgen_out = 0;

void ctl_mainloop(void)
{
    // int spd_target = gmp_base_get_system_tick() / 100 - 4;

    // ctl_set_pmsm_ctrl_speed(&pmsm_ctrl, float2ctrl(0.1) * spd_target - float2ctrl(1.0));

    //
    // Judge if PWM is enabled
    //
    // if (pmsm_ctrl.flag_enable_output)
    //{
    //    ctl_enable_output();
    //}
    // else
    //{
    //    ctl_disable_output();
    //}

    //
    // ADC Auto calibrate
    //
    // if (flag_enable_adc_calibrator)
    //{
    //    if (ctl_is_adc_calibrator_cmpt(&adc_calibrator) && ctl_is_adc_calibrator_result_valid(&adc_calibrator))
    //    {
    //        // set_adc_bias_via_channel(index_adc_calibrator, ctl_get_adc_calibrator_result(&adc_calibrator));

    //        if (index_adc_calibrator == 3) // dc bus calibrate
    //        {
    //            idc.bias = idc.bias + ctl_div(ctl_get_adc_calibrator_result(&adc_calibrator), idc.gain);

    //            flag_enable_adc_calibrator = 0;

    //            // enable pmsm controller
    //            ctl_enable_pmsm_ctrl_output(&pmsm_ctrl);
    //        }
    //        // index_adc_calibrator == 2 ~ 0, for Iabc
    //        else
    //        {
    //            // iabc get result
    //            iabc.bias[index_adc_calibrator] =
    //                iabc.bias[index_adc_calibrator] +
    //                ctl_div(ctl_get_adc_calibrator_result(&adc_calibrator), iabc.gain[index_adc_calibrator]);

    //            // move to next position
    //            index_adc_calibrator += 1;

    //            // clear calibrator
    //            ctl_clear_adc_calibrator(&adc_calibrator);

    //            // enable calibrator to next position
    //            ctl_enable_adc_calibrator(&adc_calibrator);
    //        }

    //        if (index_adc_calibrator > MTR_ADC_IDC)
    //            flag_enable_adc_calibrator = 0;
    //    }
    //}
		
		if(sinv_ig.control_port.value>=0.1)
		{
			ctl_disable_output();
		}

    return;
}
