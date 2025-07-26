// Three phase DC/AC inverter

#ifndef _FILE_THREE_PHASE_DC_AC_H_
#define _FILE_THREE_PHASE_DC_AC_H_

#include <ctl/component/intrinsic/continuous/continuous_pid.h>
#include <ctl/component/intrinsic/discrete/discrete_filter.h>
#include <ctl/component/intrinsic/discrete/proportional_resonant.h>

#include <ctl/component/digital_power/three_phase/pll.h>
#include <ctl/math_block/coordinate/coord_trans.h>

#ifndef CURRENT_SAMPLE_PHASE_MODE
// 3: Sample 3 phase current
// 2: Sample 2 phase current
#define CURRENT_SAMPLE_PHASE_MODE (3)
#endif // CURRENT_SAMPLE_PHASE_MODE

#ifndef VOLTAGE_SAMPLE_PHASE_MODE
// 3: Sample 3 phase voltage
// 2: Sample 2 phase voltage
// 1: Sample 2 line voltage
#define VOLTAGE_SAMPLE_PHASE_MODE (3)
#endif // VOLTAGE_SAMPLE_PHASE_MODE

typedef struct _tag_inv_ctrl_type
{
    //
    // input port
    //
    // positive direction: inverter
    //

    // DC Bus voltage
    // positive is u bus
    adc_ift *adc_udc;

    // DC Bus current
    // positive is inverter state
    adc_ift *adc_idc;

    // current measurement
    adc_ift *adc_iabc[3];

    // voltage measurement
    adc_ift *adc_uabc[3];

    //
    // output port
    //

    // channel 0: A phase
    // channel 1: B phase
    // channel 2: C phase
    ctl_vector3_t pwm_out_pu;

    //
    // Feed-forward
    //

    // 2*pi*f*Lf*(i_base/u_base)
    ctrl_gt omega_L;

    //
    // intermediate variables
    //

#if VOLTAGE_SAMPLE_PHASE_MODE == 3
    ctl_vector3_t vab0;
#elif (VOLTAGE_SAMPLE_PHASE_MODE == 2) || (VOLTAGE_SAMPLE_PHASE_MODE == 1)
    ctl_vector2_t vab;
#endif // VOLTAGE_SAMPLE_PHASE_MODE

#if CURRENT_SAMPLE_PHASE_MODE == 3
    ctl_vector3_t iab0;
#elif CURRENT_SAMPLE_PHASE_MODE == 2
    ctl_vector2_t iab;
#endif // CURRENT_SAMPLE_PHASE_MODE

    // voltage set
    ctl_vector2_t vdq_set;

    // measurement voltage
    ctl_vector2_t vdq;

    // current set
    ctl_vector2_t idq_set;

    // measurement current
    ctl_vector2_t idq;

    // modulation result, dq
    ctl_vector2_t vdq_pos_out;

    // modulation total result, alpha-beta
    ctl_vector2_t vab_out;

    // positive alpha-beta
    ctl_vector2_t vab_pos;

    // modulation result, dq, neg
    ctl_vector2_t vdq_neg_out;

    // negative alpha-beta
    ctl_vector2_t vab_neg;

    // harmonic alpha-beta
    ctl_vector2_t vab_harm;

    // current angle
    ctrl_gt angle;

    // current phasor
    ctl_vector2_t phasor;

    //
    // Controller object
    //

    // input filter
    ctl_low_pass_filter_t lpf_udc;
    ctl_low_pass_filter_t lpf_idc;
    ctl_low_pass_filter_t lpf_iabc[3];
    ctl_low_pass_filter_t lpf_uabc[3];

    // PLL object
    three_phase_pll_t pll;

    // ramp generator for free run
    ctl_src_rg_t rg;

    // voltage controller, dq
    ctl_pid_t voltage_ctrl[2];

    // current controller, dq
    ctl_pid_t current_ctrl[2];

    // negative current controller, dq negative
    ctl_pid_t neg_current_ctrl[2];

    // QR controller for current 3th harmonic, alpha beta
    qr_ctrl_t sinv_qr_3[2];

    // QR controller for current 5th harmonic, alpha beta
    qr_ctrl_t sinv_qr_5[2];

    // QR controller for current 7th harmonic, alpha beta
    qr_ctrl_t sinv_qr_7[2];

    // QR controller for current 9th harmonic, alpha beta
    qr_ctrl_t sinv_qr_9[2];

    //
    // flag stack
    //

    // system enable
    fast_gt flag_enable_system;

    // voltage controller
    fast_gt flag_enable_voltage_ctrl;

    // voltage droop control
    fast_gt flag_enable_voltage_droop;

    // current controller
    fast_gt flag_enable_current_ctrl;

    // current feed forward
    fast_gt flag_enable_current_ff;

    // harmonic controller
    fast_gt flag_enable_harm_ctrl;

    // negative current controller
    fast_gt flag_enable_negative_current_ctrl;

    // PLL
    fast_gt flag_enable_pll;

    // Select angle source
    // 1: angle information is from rg
    // 0: angle information is from pll
    fast_gt flag_enable_freerun;

} inv_ctrl_t;

GMP_STATIC_INLINE
void ctl_step_inv_ctrl(inv_ctrl_t *ctrl)
{
    // input filter
    ctl_step_lowpass_filter(&ctrl->lpf_udc, ctrl->adc_udc->value);
    ctl_step_lowpass_filter(&ctrl->lpf_idc, ctrl->adc_idc->value);

    for (size_gt i = 0; i < 3; ++i)
    {
        ctl_step_lowpass_filter(&ctrl->lpf_iabc[i], ctrl->adc_iabc[i]);
        ctl_step_lowpass_filter(&ctrl->lpf_uabc[i], ctrl->adc_uabc[i]);
    }

// voltage Clark
#if CURRENT_SAMPLE_PHASE_MODE == 3
    ctl_ct_clark();
#elif CURRENT_SAMPLE_PHASE_MODE == 2
    ctl_ct_clark_2ph();
#endif // CURRENT_SAMPLE_PHASE_MODE

    // current Clark
#if VOLTAGE_SAMPLE_PHASE_MODE == 3
    ctl_ct_clark();
#elif VOLTAGE_SAMPLE_PHASE_MODE == 2
    ctl_ct_clark_2ph();
#elif VOLTAGE_SAMPLE_PHASE_MODE == 1
    ctl_ct_clark_from_line();
#endif // VOLTAGE_SAMPLE_PHASE_MODE

    // three phase PLL
    if (ctrl->flag_enable_pll)
    {
        ctrl->angle = ctl_step_pll_3ph(&ctrl->pll, ctrl->vab0.dat[phase_alpha], ctrl->vab0.dat[phase_beta]);
    }

    // controller body
    if (ctrl->flag_enable_system)
    {
        ctrl->angle = ctl_step_ramp_gen(&ctrl->rg);

        if (ctrl->flag_enable_freerun)
        {
            ctl_set_phasor_via_angle(ctrl->angle, ctrl->phasor);
        }
        else
        {
            ctrl->phasor.dat[0] = ctrl->pll.phasor.dat[0];
            ctrl->phasor.dat[1] = ctrl->pll.phasor.dat[1];
        }

        // park
        ctl_ct_park2(&ctrl->iab0, &phasor, &ctrl->idq);
        ctl_ct_park2(&ctrl->vab0, &phasor, &ctrl->vdq);

        // voltage droop control
        if (ctrl->flag_enable_voltage_droop)
        {
        }

        // voltage outer loop
        if (ctrl->flag_enable_voltage_ctrl)
        {
            ctrl->idq_set[phase_d] =
                ctl_step_pid_ser(&ctrl->voltage_ctrl[phase_d], ctrl->vdq_set[phase_d] - ctrl->vdq[phase_d]);

            ctrl->idq_set[phase_q] =
                ctl_step_pid_ser(&ctrl->voltage_ctrl[phase_q], ctrl->vdq_set[phase_q] - ctrl->vdq[phase_q]);
        }

        // current positive loop
        if (ctrl->flag_enable_current_ctrl)
        {
            ctrl->vdq_pos_out[phase_d] =
                ctl_step_pid_ser(&ctrl->current_ctrl[phase_d], ctrl->idq_set[phase_d] - ctrl->idq[phase_d]);
            ctrl->vdq_pos_out[phase_q] =
                ctl_step_pid_ser(&ctrl->current_ctrl[phase_q], ctrl->idq_set[phase_q] - ctrl->idq[phase_q]);

            if (ctrl->flag_enable_current_ff)
            {
                ctrl->vdq_pos_out[phase_d] -= ctl_mul(ctrl->omega_L, ctrl->idq.dat[phase_q]);
                ctrl->vdq_pos_out[phase_q] += ctl_mul(ctrl->omega_L, ctrl->idq.dat[phase_q]);
            }
        }

        // modulation
        ctl_ct_ipark2(&ctrl->vdq_pos_out, &phasor, &ctrl->vab_pos);

        // current negative loop
        if (ctrl->flag_enable_negative_current_ctrl)
        {
        }
        else
        {
            ctl_vector2_clear(&ctrl->vab_neg);
        }

        // harm control
        if (ctrl->flag_enable_harm_ctrl)
        {
        }
        else
        {
            ctl_vector2_clear(&ctrl->vab_harm);
        }

        // mix them up
        ctrl->vab_out.dat[0] = ctrl->vab_pos.dat[0] + ctrl->vab_neg.dat[0] + ctrl->vab_harm.dat[0];
        ctrl->vab_out.dat[1] = ctrl->vab_pos.dat[1] + ctrl->vab_neg.dat[1] + ctrl->vab_harm.dat[1];
    }
    else
    {
        ctrl->pwm_out_pu.dat[phase_A] = float2ctrl(0.5);
        ctrl->pwm_out_pu.dat[phase_B] = float2ctrl(0.5);
        ctrl->pwm_out_pu.dat[phase_C] = float2ctrl(0.5);
    }
}

GMP_STATIC_INLINE
void ctl_enable_three_phase_inverter(inv_ctrl_t *inv)
{
    inv->flag_enable_system = 1;
}

GMP_STATIC_INLINE
void ctl_disable_three_phase_inverter(inv_ctrl_t *inv)
{
    inv->flag_enable_system = 0;
}

#endif // _FILE_THREE_PHASE_DC_AC_H_
