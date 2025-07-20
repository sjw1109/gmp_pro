

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
typedef struct _tag_sinv_ctrl_type
{
    //
    // input port
    //

    // DC Bus voltage
    // positive is u bus
    adc_ift *adc_udc;

    // DC Bus current
    // positive is inverter state
    adc_ift *adc_idc;

    // inductor current
    // positive is inverter
    adc_ift *adc_il;

    // grid voltage
    // positive is u bus
    adc_ift *adc_ugrid;

    // grid current
    // positive is output inverter state
    adc_ift *adc_igrid;

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

    // current set
    ctrl_gt ig_set;

    // current reference is a AC value
    ctrl_gt ig_ref;

    // voltage set
    ctrl_gt v_set;

    // Power factor set, pf_set = cos phi
    ctrl_gt pf_set;

    // modulation target
    ctrl_gt modulation;

    // target phase
    ctrl_gt target_phase;

    //
    // Controller object
    //

    // ADC input filter
    ctl_low_pass_filter_t lpf_udc;
    ctl_low_pass_filter_t lpf_idc;
    ctl_low_pass_filter_t lpf_il;
    ctl_low_pass_filter_t lpf_ugrid;
    ctl_low_pass_filter_t lpf_igrid;

    // Single phase PLL
    ctl_single_phase_pll spll;

    // open loop angle source
    ctl_src_rg_t rg;

    // ramp generator

    // QPR controller for current base frequency
    qpr_ctrl_t sinv_qpr_base;

    // QR controller for current 3th harmonic
    qr_ctrl_t sinv_qr_3;

    // QR controller for current 5th harmonic
    qr_ctrl_t sinv_qr_5;

    // QR controller for current 7th harmonic
    qr_ctrl_t sinv_qr_7;

    // PID controller for voltage
    pid_regular_t voltage_pid;

    //
    // flag stack
    //

    // enable the whole controller
    fast_gt flag_enable_system;

    // enable current controller
    fast_gt flag_enable_current_ctrl;

    // current controller add an addition harmonic controller
    fast_gt flag_enable_harmonic_ctrl;

    // enable voltage controller
    fast_gt flag_enable_voltage_ctrl;

    // 0: working in inverter mode
    // 1: working in rectifier mode
    fast_gt flag_rectifier_mode;

    // enable ramp generator module
    fast_gt flag_enable_ramp;

    // select angle source is ramp generator
    fast_gt flag_angle_freerun;

    // enable single phase PLL
    fast_gt flag_enable_spll;

} sinv_ctrl_t;

GMP_STATIC_INLINE
void ctl_clear_sinv(sinv_ctrl_t *sinv)
{
    ctl_clear_pid(&sinv->voltage_pid);

    ctl_clear_qpr_controller(&sinv->sinv_qpr_base);
    ctl_clear_qr_controller(&sinv->sinv_qr_3);
    ctl_clear_qr_controller(&sinv->sinv_qr_5);
    ctl_clear_qr_controller(&sinv->sinv_qr_7);
}

GMP_STATIC_INLINE
void ctl_clear_sinv_with_pll(sinv_ctrl_t *sinv)
{
    ctl_clear_sinv(sinv);
    ctl_clear_single_phase_pll(&sinv->spll);
}

GMP_STATIC_INLINE
ctrl_gt ctl_step_sinv(sinv_ctrl_t *sinv)
{
    //ctrl_gt target_phase;

    // Filter for ADC input channel
    ctl_step_lowpass_filter(&sinv->lpf_idc, sinv->adc_idc->value);
    ctl_step_lowpass_filter(&sinv->lpf_udc, sinv->adc_udc->value);
    ctl_step_lowpass_filter(&sinv->lpf_il, sinv->adc_il->value);
    ctl_step_lowpass_filter(&sinv->lpf_ugrid, sinv->adc_ugrid->value);
    ctl_step_lowpass_filter(&sinv->lpf_igrid, sinv->adc_igrid->value);

    // Single phase PLL
    if (sinv->flag_enable_spll)
    {
        // Single phase PLL
        ctl_step_single_phase_pll(
            // handle of Single phase PLL object
            &sinv->spll,
            // input AC value
            ctl_get_lowpass_filter_result(&sinv->lpf_ugrid));
    }

    if (sinv->flag_enable_system)
    {

        // enable angle ramp generator
        if (sinv->flag_enable_ramp)
        {
            ctl_step_ramp_gen(&sinv->rg);
        }

        // Select Angle source

        // Select ramp generator as angle source
        if (sinv->flag_angle_freerun)
        {
            sinv->target_phase = ctl_mul(ctl_sin(ctl_get_ramp_gen_output(&sinv->rg)), sinv->pf_set) +
                           ctl_mul(ctl_cos(ctl_get_ramp_gen_output(&sinv->rg)),
                                   ctl_sqrt(float2ctrl(1) - ctl_mul(sinv->pf_set, sinv->pf_set)));
        }
        // Select SPLL as angle source
        else
        {
            sinv->target_phase = ctl_mul(sinv->spll.phasor.dat[phasor_sin], sinv->pf_set) +
                           ctl_mul(sinv->spll.phasor.dat[phasor_cos],
                                   ctl_sqrt(float2ctrl(1) - ctl_mul(sinv->pf_set, sinv->pf_set)));
        }

        // Voltage Loop
        if (sinv->flag_enable_voltage_ctrl)
        {
            // Rectifier AC -> DC mode
            if (sinv->flag_rectifier_mode)
            {
                sinv->ig_set =
                    -ctl_step_pid_ser(&sinv->voltage_pid, sinv->v_set - ctl_get_lowpass_filter_result(&sinv->lpf_udc));
            }
            // Converter DC -> AC mode
            else
            {
                // Need a true RMS calculator

                // sinv->ig_set =
                //     ctl_step_pid_ser(&sinv->voltage_pid, sinv->v_set -
                //     ctl_get_lowpass_filter_result(&sinv->lpf_ugrid));
            }
        }

        // Current Loop
        if (sinv->flag_enable_current_ctrl)
        {
            sinv->ig_ref = ctl_mul(sinv->target_phase, sinv->ig_set);

            // enable current loop
            sinv->modulation = ctl_step_qpr_controller(&sinv->sinv_qpr_base,
                                                       sinv->ig_ref - ctl_get_lowpass_filter_result(&sinv->lpf_igrid));

            // enable harmonic control
            if (sinv->flag_enable_harmonic_ctrl)
            {
                // 3th, 5th, 7th order harmonic control
                sinv->modulation +=
                    ctl_step_qr_controller(&sinv->sinv_qr_3, -ctl_get_lowpass_filter_result(&sinv->lpf_igrid));
                sinv->modulation +=
                    ctl_step_qr_controller(&sinv->sinv_qr_5, -ctl_get_lowpass_filter_result(&sinv->lpf_igrid));
                sinv->modulation +=
                    ctl_step_qr_controller(&sinv->sinv_qr_7, -ctl_get_lowpass_filter_result(&sinv->lpf_igrid));
            }
        }
        else
        {
            sinv->ig_ref = ctl_mul(sinv->spll.phasor.dat[phasor_sin], sinv->ig_set);
            sinv->modulation = sinv->ig_ref;
        }

        // Modulation stage

#ifndef CTL_SINV_CTRL_UNIPOLAR_MODULATION
        // Unipolar SPWM 1
        sinv->sinv_pwm_pu[0] = ctl_div2(-sinv->modulation + float2ctrl(1));
        sinv->sinv_pwm_pu[1] = ctl_div2(sinv->modulation + float2ctrl(1));
#else  // CTL_SINV_CTRL_UNIPOLAR_MODULATION
       // Unipolar SPWM 2
        sinv->sinv_pwm_pu[0] = -sinv->modulation;
        sinv->sinv_pwm_pu[1] = sinv->modulation;
#endif // CTL_SINV_CTRL_UNIPOLAR_MODULATION
    }
    // output is 0, should disable output
    else
    {
        sinv->sinv_pwm_pu[0] = 0;
        sinv->sinv_pwm_pu[1] = 0;
    }

    return sinv->modulation;
}

// function set for change controller state

// VOLTAGE OPEN LOOP
GMP_STATIC_INLINE
void ctl_set_sinv_openloop_inverter(sinv_ctrl_t *sinv)
{
    // enable the whole controller
    sinv->flag_enable_system = 0;

    // enable current controller
    sinv->flag_enable_current_ctrl = 0;

    // current controller add an addition harmonic controller
    sinv->flag_enable_harmonic_ctrl = 0;

    // enable voltage controller
    sinv->flag_enable_voltage_ctrl = 0;

    // 0: working in inverter mode
    // 1: working in rectifier mode
    sinv->flag_rectifier_mode = 0;

    // enable ramp generator module
    sinv->flag_enable_ramp = 1;

    // select angle source is ramp generator
    sinv->flag_angle_freerun = 1;

    // enable single phase PLL
    sinv->flag_enable_spll = 1;
}

// function set for change controller state

// Current controller is on for inverter
GMP_STATIC_INLINE
void ctl_set_sinv_current_closeloop_inverter(sinv_ctrl_t *sinv)
{
    // enable the whole controller
    sinv->flag_enable_system = 0;

    // enable current controller
    sinv->flag_enable_current_ctrl = 1;

    // current controller add an addition harmonic controller
    sinv->flag_enable_harmonic_ctrl = 0;

    // enable voltage controller
    sinv->flag_enable_voltage_ctrl = 0;

    // 0: working in inverter mode
    // 1: working in rectifier mode
    sinv->flag_rectifier_mode = 0;

    // enable ramp generator module
    sinv->flag_enable_ramp = 1;

    // select angle source is ramp generator
    sinv->flag_angle_freerun = 1;

    // enable single phase PLL
    sinv->flag_enable_spll = 1;
}

// Current controller is on for inverter
GMP_STATIC_INLINE
void ctl_set_sinv_voltage_closeloop_inverter(sinv_ctrl_t *sinv)
{
    // enable the whole controller
    sinv->flag_enable_system = 0;

    // enable current controller
    sinv->flag_enable_current_ctrl = 1;

    // current controller add an addition harmonic controller
    sinv->flag_enable_harmonic_ctrl = 0;

    // enable voltage controller
    sinv->flag_enable_voltage_ctrl = 1;

    // 0: working in inverter mode
    // 1: working in rectifier mode
    sinv->flag_rectifier_mode = 0;

    // enable ramp generator module
    sinv->flag_enable_ramp = 1;

    // select angle source is ramp generator
    sinv->flag_angle_freerun = 1;

    // enable single phase PLL
    sinv->flag_enable_spll = 1;
}

//
GMP_STATIC_INLINE
void ctl_set_sinv_voltage_closeloop_rectifier(sinv_ctrl_t *sinv)
{
    // enable the whole controller
    sinv->flag_enable_system = 0;

    // enable current controller
    sinv->flag_enable_current_ctrl = 1;

    // current controller add an addition harmonic controller
    sinv->flag_enable_harmonic_ctrl = 0;

    // enable voltage controller
    sinv->flag_enable_voltage_ctrl = 1;

    // 0: working in inverter mode
    // 1: working in rectifier mode
    sinv->flag_rectifier_mode = 1;

    // enable ramp generator module
    sinv->flag_enable_ramp = 1;

    // select angle source is ramp generator
    sinv->flag_angle_freerun = 1;

    // enable single phase PLL
    sinv->flag_enable_spll = 1;
}

// set target value
GMP_STATIC_INLINE
void ctl_set_sinv_current_ref(sinv_ctrl_t *sinv, ctrl_gt i_target)
{
    sinv->ig_set = i_target;
}

GMP_STATIC_INLINE
void ctl_set_sinv_voltage_ref(sinv_ctrl_t *sinv, ctrl_gt v_target)
{
    sinv->v_set = v_target;
}

GMP_STATIC_INLINE
void ctl_set_sinv_power_factor(sinv_ctrl_t *sinv, ctrl_gt pf)
{
    sinv->pf_set = pf;
}

GMP_STATIC_INLINE
void ctl_enable_sinv_ctrl(sinv_ctrl_t *sinv)
{
    sinv->flag_enable_system = 1;
}

GMP_STATIC_INLINE
void ctl_disable_sinv_ctrl(sinv_ctrl_t *sinv)
{
    sinv->flag_enable_system = 0;
}

GMP_STATIC_INLINE
void ctl_set_sinv_freerun(sinv_ctrl_t* sinv)
{
    sinv->flag_angle_freerun = 1;
}

GMP_STATIC_INLINE
void ctl_set_sinv_pll(sinv_ctrl_t *sinv)
{
    sinv->flag_angle_freerun = 0;
}

GMP_STATIC_INLINE
void ctl_enable_sinv_harm_ctrl(sinv_ctrl_t *sinv)
{
    sinv->flag_enable_harmonic_ctrl = 1;
}

GMP_STATIC_INLINE
void ctl_disable_sinv_harm_ctrl(sinv_ctrl_t *sinv)
{
    sinv->flag_enable_harmonic_ctrl = 0;
}

// init structure for sinv
typedef struct _tag_single_phase_converter_init_t
{
    // base frequency
    parameter_gt base_freq;

    // voltage controller parameters
    parameter_gt v_ctrl_kp;
    parameter_gt v_ctrl_Ti;
    parameter_gt v_ctrl_Td;

    // current controller parameters
    parameter_gt i_ctrl_kp;
    parameter_gt i_ctrl_kr;
    parameter_gt i_ctrl_cut_freq;

    // PLL parameters
    parameter_gt pll_ctrl_kp;
    parameter_gt pll_ctrl_Ti;
    parameter_gt pll_ctrl_cut_freq;

    // harmonic controller parameters
    parameter_gt harm_ctrl_kr_3;
    parameter_gt harm_ctrl_cut_freq_3;
    parameter_gt harm_ctrl_kr_5;
    parameter_gt harm_ctrl_cut_freq_5;
    parameter_gt harm_ctrl_kr_7;
    parameter_gt harm_ctrl_cut_freq_7;

    // adc input filter cut frequency
    parameter_gt adc_filter_fc;

    // controller frequency
    parameter_gt f_ctrl;
} sinv_init_t;

void ctl_upgrade_sinv_param(sinv_ctrl_t *sinv, sinv_init_t *init);

void ctl_init_sinv_ctrl(sinv_ctrl_t *sinv, sinv_init_t *init);

// This function will attach ADC control port to sinv controller
void ctl_attach_sinv_with_adc(sinv_ctrl_t *sinv, adc_ift *udc, adc_ift *idc, adc_ift *il, adc_ift *ugrid, adc_ift *igrid);


#endif // _FILE_SINGLE_PHASE_DC_AC_H_
