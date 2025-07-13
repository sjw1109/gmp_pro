

#include <ctl/component/intrinsic/continuous/continuous_pid.h>
#include <ctl/component/intrinsic/discrete/discrete_filter.h>
#include <ctl/component/intrinsic/discrete/proportional_resonant.h>

#ifndef _FILE_SINGLE_PHASE_DC_AC_H_
#define _FILE_SINGLE_PHASE_DC_AC_H_

typedef enum _tag_single_phase_name
{
    PHASE_N = 0,
    PHASE_L = 1
} single_phase_name_num;

// SINV modulation method select
// enable the following macro
//  + Two phase bridge only works in one phase
// disable the following macro
//  + Two phase bridge symmetric conduction
//
// #define CTL_SINV_CTRL_UNIPOLAR_MODULATION

// DC/AC Converter with LC filter
typedef struct _tag_sinv_type
{
    //
    // input port
    //

    // capacitor voltage
    // positive is u bus
    adc_ift *uc;

    // inductor current
    // positive is inverter
    adc_ift *il;

    // input voltage
    // positive is u bus
    adc_ift *uin;

    //
    // output port
    //

    // channel 0: L phase
    // channel 1: N phase
    ctrl_gt sinv_pwm_pu[2];

    //
    // Feed-forward
    //

    //
    // intermediate variables
    //

    //
    // Controller object
    //

    // SPLL
    ctl_single_phase_pll spll;

    // PR controller for current
    qpr_ctrl_t sinv_qpr_base;

    // PID controller for voltage
    pid_regular_t vlotage_pid;

    //
    // flag stack
    //

} sinv_t;

GMP_STATIC_INLINE
ctrl_gt ctl_step_sinv(sinv_t *sinv)
{

    // SPLL
    ctl_step_single_phase_pll(
        // handle of Single phase PLL object
        &spll,
        // input AC value
        ctl_sin(ctl_step_ramp_gen(&rg)));

    // Select Angle source
    // + SPLL
    // + R

    // Voltage Openloop
    // modulate_target = ctl_mul(spll.phasor.dat[phase_alpha], v_set);

    modulate_target = ctl_step_qpr_controller(&sinv_qpr_base, ctl_mul(spll.phasor.dat[0], float2ctrl(v_set)) -
                                                                  sinv_il.control_port.value);

    // ADC sample is negative
    // modulate_target = ctl_step_qpr_controller(&sinv_qpr_base, ctl_mul(spll.phasor.dat[0], float2ctrl(v_set)) +
    //                                                              sinv_il.control_port.value);
#ifndef CTL_SINV_CTRL_UNIPOLAR_MODULATION
    // Unipolar SPWM 1
    sinv_pwm_pu[0] = ctl_div2(-modulate_target + float2ctrl(1));
    sinv_pwm_pu[1] = ctl_div2(modulate_target + float2ctrl(1));
#else  // CTL_SINV_CTRL_UNIPOLAR_MODULATION
    // Unipolar SPWM 2
    // sinv_pwm_pu[0] = -modulate_target;
    // sinv_pwm_pu[1] = modulate_target;
#endif // CTL_SINV_CTRL_UNIPOLAR_MODULATION
}

#endif // _FILE_SINGLE_PHASE_DC_AC_H_
