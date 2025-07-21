
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

#include <ctl/component/digital_power/topology_preset/single_phase_dc_ac.h>

sinv_ctrl_t sinv_ctrl;

// enable motor running
#if !defined SPECIFY_PC_ENVIRONMENT
volatile fast_gt flag_enable_system = 0;
#else
volatile fast_gt flag_enable_system = 1;
#endif // SPECIFY_PC_ENVIRONMENT

volatile fast_gt flag_error = 0;

// CTL initialize routine
void ctl_init()
{
    // stop here and wait for user start the motor controller
    ctl_disable_output();

    sinv_init_t init;

    init.base_freq = 50.0f;

    // voltage controller parameters
    init.v_ctrl_kp = 0.4f;
    init.v_ctrl_Ti = 0.2f;
    init.v_ctrl_Td = 0;

    // PLL parameters
    init.pll_ctrl_kp = 0.75f / 20;
    init.pll_ctrl_Ti = 1000.0f;
    init.pll_ctrl_cut_freq = 9.0f;

    // current controller parameters
    init.i_ctrl_kp = 0.00000001f;
    init.i_ctrl_kr = 20.0f;
    init.i_ctrl_cut_freq = 8.0f;

    // harmonic controller parameters
    init.harm_ctrl_kr_3 = 1.0f;
    init.harm_ctrl_cut_freq_3 = 5.0f;
    init.harm_ctrl_kr_5 = 1.0f;
    init.harm_ctrl_cut_freq_5 = 5.0f;
    init.harm_ctrl_kr_7 = 1.0f;
    init.harm_ctrl_cut_freq_7 = 5.0f;

    // adc input filter cut frequency
    init.adc_filter_fc = 1000.0;

    // controller frequency
    init.f_ctrl = CONTROLLER_FREQUENCY;

    // init sinv Controller
    ctl_init_sinv_ctrl(&sinv_ctrl, &init);

#if BUILD_LEVEL == 1
    // Voltage open loop
    ctl_set_sinv_openloop_inverter(&sinv_ctrl);
    ctl_set_sinv_current_ref(&sinv_ctrl, float2ctrl(0.1));
    ctl_set_sinv_freerun(&sinv_ctrl);
    ctl_disable_sinv_harm_ctrl(&sinv_ctrl);

#elif BUILD_LEVEL == 2
    // current close loop
    ctl_set_sinv_current_closeloop_inverter(&sinv_ctrl);
    ctl_set_sinv_current_ref(&sinv_ctrl, float2ctrl(0.1));
    ctl_set_sinv_freerun(&sinv_ctrl);
    ctl_disable_sinv_harm_ctrl(&sinv_ctrl);

#elif BUILD_LEVEL == 3

    // current close loop, with harm control
    ctl_set_sinv_current_closeloop_inverter(&sinv_ctrl);
    ctl_set_sinv_current_ref(&sinv_ctrl, float2ctrl(0.1));
    ctl_set_sinv_freerun(&sinv_ctrl);
    ctl_enable_sinv_harm_ctrl(&sinv_ctrl);

#elif BUILD_LEVEL == 4
    // rectifier voltage loop, without harm control
    ctl_set_sinv_voltage_closeloop_rectifier(&sinv_ctrl);
    ctl_set_sinv_voltage_ref(&sinv_ctrl, float2ctrl(0.4));
    ctl_disable_sinv_harm_ctrl(&sinv_ctrl);
    ctl_set_sinv_pll(&sinv_ctrl);

#elif BUILD_LEVEL == 5
    // rectifier voltage loop, with harm control
    ctl_set_sinv_voltage_closeloop_rectifier(&sinv_ctrl);
    ctl_set_sinv_voltage_ref(&sinv_ctrl, float2ctrl(0.4));
    ctl_enable_sinv_harm_ctrl(&sinv_ctrl);
    ctl_set_sinv_pll(&sinv_ctrl);

#elif BUILD_LEVEL == 6
    // inverter voltage loop, without harm control
    ctl_set_sinv_voltage_closeloop_inverter(&sinv_ctrl);
    ctl_set_sinv_voltage_ref(&sinv_ctrl, float2ctrl(0.4));
    ctl_disable_sinv_harm_ctrl(&sinv_ctrl);
    ctl_set_sinv_pll(&sinv_ctrl);

#elif BUILD_LEVEL == 7
    // inverter voltage loop, with harm control
    ctl_set_sinv_voltage_closeloop_inverter(&sinv_ctrl);
    ctl_set_sinv_voltage_ref(&sinv_ctrl, float2ctrl(0.4));
    ctl_enable_sinv_harm_ctrl(&sinv_ctrl);
    ctl_set_sinv_pll(&sinv_ctrl);

#endif // BUILD_LEVEL

    ctl_disable_output();

    // if in simulation mode, enable system automatically
#if !defined SPECIFY_PC_ENVIRONMENT

    while (flag_enable_system == 0)
    {
    }

#endif // SPECIFY_PC_ENVIRONMENT
}

//////////////////////////////////////////////////////////////////////////
// endless loop function here

uint16_t sgen_out = 0;
fast_gt firsttime_flag = 0;
fast_gt startup_flag = 0;
fast_gt started_flag = 0;
time_gt tick_bias = 0;

void ctl_mainloop(void)
{
    // When the program is reach here, the following things will happen:
    // 1. software non-block delay 500ms
    // 2. judge if spll theta error convergence has occurred
    // 3. then enable system

    // first time flag
    if ((flag_enable_system == 1) && firsttime_flag == 0)
    {
        tick_bias = gmp_base_get_system_tick();
        firsttime_flag = 1;
    }

    // a delay of 500ms
    if ((flag_enable_system == 1) && (started_flag == 0) && ((gmp_base_get_system_tick() - tick_bias) > 500) &&
        (startup_flag == 0))
    {
        startup_flag = 1;
    }

    // judge if PLL is close to target
    if ((flag_enable_system == 1) && (started_flag == 0) && (startup_flag == 1) &&
        ctl_get_spll_error_fbk(&sinv_ctrl.spll) < CTRL_SPLL_EPSILON)
    {
        ctl_enable_output();
        started_flag = 1;
    }

    // if system is disabled
    if (flag_enable_system == 0)
    {
        ctl_disable_output();

        // clear all flags
        firsttime_flag = 0;
        startup_flag = 0;
        started_flag = 0;
        tick_bias = 0;
    }

    return;
}
