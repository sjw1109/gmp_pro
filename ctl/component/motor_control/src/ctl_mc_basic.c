/**
 * @file ctl_motor_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <gmp_core.h>

#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// SVPWM module, this module has been deleted
// NEW SVPWM module please reference

// #include <ctl/component/motor/svpwm.h>
//
// void ctl_init_svpwm(ctl_svpwm_channel_t *svpwm)
//{
//	svpwm->Ualpha = 0;
//	svpwm->Ubeta = 0;
//
//	svpwm->pwm_period = 0;
//
//	for (int i = 0; i < 3; ++i)
//	{
//		svpwm->T[i] = 0;
//		svpwm->pwm_cmp[i] = 0;
//	}
// }
//
// void ctl_setup_svpwm(ctl_svpwm_channel_t *svpwm, pwm_gt pwm_period)
//{
//	svpwm->pwm_period = pwm_period;
//
//	// re-calculate SVPWM comparator
//	ctl_svpwm_modulation(svpwm);
// }

// void ctl_svpwm_set_Uab(ctl_svpwm_channel_t* svpwm, ctl_vector3_t Uab)
//{
//	svpwm->Ualpha = Uab.dat[0];
//	svpwm->Ubeta = Uab.dat[1];
//
// }

//////////////////////////////////////////////////////////////////////////
// Encoder module

#include <ctl/component/motor_control/basic/encoder.h>

// Absolute rotation position encoder
// 

// void ctl_init_pos_encoder(ctl_pos_encoder_t *enc)
//{
//    // interface
//    enc->encif.position = 0;
//    enc->encif.elec_position = 0;
//
//    enc->offset = 0;
//    enc->pole_pairs = 1;
//    enc->position_base = 1;
//}

void ctl_init_pos_encoder(ctl_pos_encoder_t *enc, uint16_t poles, uint32_t position_base)
{
    enc->encif.position = 0;
    enc->encif.elec_position = 0;
    enc->encif.revolutions = 0;

    enc->offset = 0;

    enc->pole_pairs = poles;
    enc->position_base = position_base;
}

void ctl_init_multiturn_pos_encoder(ctl_pos_multiturn_encoder_t *enc, uint16_t poles, uint32_t position_base)
{
    enc->encif.position = 0;
    enc->encif.elec_position = 0;
    enc->encif.revolutions = 0;

    enc->offset = 0;

    enc->pole_pairs = poles;
    enc->position_base = position_base;
}

void ctl_init_autoturn_pos_encoder(ctl_pos_autoturn_encoder_t *enc, uint16_t poles, uint32_t position_base)
{
    enc->encif.position = 0;
    enc->encif.elec_position = 0;
    enc->encif.revolutions = 0;

    enc->offset = 0;

    enc->pole_pairs = poles;
    enc->position_base = position_base;
}

// Speed position encoder
//

//void ctl_init_spd_encoder(ctl_spd_encoder_t *spd_encoder)
//{
//    spd_encoder->speed_base = 3000;
//    spd_encoder->encif.speed = 0;
//    spd_encoder->speed_krpm = 0;
//}

void ctl_init_spd_encoder(ctl_spd_encoder_t *enc, parameter_gt speed_base)
{
    enc->speed_base = speed_base;
    enc->encif.speed = 0;
    enc->speed_krpm = 0;
}

void ctl_init_spd_calculator(ctl_spd_calculator_t *sc)
{
    sc->old_position = 0;
    sc->scale_factor = float2ctrl(1.0);
    sc->encif.speed = 0;
    ctl_init_lp_filter(&sc->spd_filter);
    ctl_init_divider(&sc->div);
}

void ctl_setup_spd_calculator(
    // speed calculator objects
    ctl_spd_calculator_t *sc,
    // control law frequency, unit Hz
    parameter_gt control_law_freq,
    // division of control law frequency, unit ticks
    uint16_t speed_calc_div,
    // Speed per unit base value, unit rpm
    parameter_gt rated_speed_rpm,
    // pole pairs, if you pass a elec-angle,
    uint16_t pole_pairs,
    // just set this value to 1.
    // generally, speed_filter_fc approx to speed_calc freq divided by 5
    parameter_gt speed_filter_fc,
    // link to a position encoder
    ctl_rotation_encif_t *pos_encif)
{
    uint16_t maximum_div = (uint16_t)rated_speed_rpm / 30;
    if (speed_calc_div < maximum_div)
    {
        maximum_div = speed_calc_div;
    }

    sc->scale_factor = float2ctrl(60.0f * control_law_freq / maximum_div / pole_pairs / rated_speed_rpm);
    ctl_setup_lp_filter(&sc->spd_filter, control_law_freq / maximum_div, speed_filter_fc);
    ctl_setup_divider(&sc->div, maximum_div);

    sc->pos_encif = pos_encif;
}

//////////////////////////////////////////////////////////////////////////
// PMSM consultant

#include <ctl/component/motor_control/consultant/pmsm_consultant.h>

void ctl_init_pmsm_nameplate_consultant(ctl_pmsm_nameplate_consultant_t *np)
{
    np->rated_speed_rpm = (parameter_gt)3000;
    np->rated_output_power = (parameter_gt)1000;
    np->rated_voltage = (parameter_gt)300;
    np->rated_current = (parameter_gt)300;
    np->power_factor = (parameter_gt)0.9;
    np->rated_freq = (parameter_gt)50;
    np->rated_torque = (parameter_gt)0.3;
    np->eta = (parameter_gt)0.9;
}

void ctl_setup_pmsm_nameplate_consultant(ctl_pmsm_nameplate_consultant_t *np, parameter_gt rated_output_power,
                                         parameter_gt rated_voltage, parameter_gt rated_current,
                                         parameter_gt rated_freq, parameter_gt rated_speed, parameter_gt rated_torque,
                                         parameter_gt power_factor, parameter_gt efficiency)
{
    np->rated_output_power = rated_output_power;
    np->rated_voltage = rated_voltage;
    np->rated_current = rated_current;

    np->rated_freq = rated_freq;
    np->rated_speed_rpm = rated_speed;
    np->rated_torque = rated_torque;

    np->power_factor = power_factor;
    np->eta = efficiency;
}

void ctl_init_pmsm_dsn_consultant(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    pmsm_dsn->pole_pair = 1;

    pmsm_dsn->Rs = 0;

    pmsm_dsn->Ld = 0;

    pmsm_dsn->Lq = 0;

    pmsm_dsn->flux = 0;

    pmsm_dsn->inertia = 0;

    pmsm_dsn->damp = 0;
}

void ctl_import_pmsm_dsn_default_param(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    pmsm_dsn->pole_pair = MOTOR_DSN_POLE_PAIR;

    pmsm_dsn->Rs = (parameter_gt)MOTOR_DSN_STATOR_RES;

    pmsm_dsn->Ld = (parameter_gt)MOTOR_DSN_LD;

    pmsm_dsn->Lq = (parameter_gt)MOTOR_DSN_LQ;

    pmsm_dsn->flux = (parameter_gt)MOTOR_DSN_FLUX_WB;

    pmsm_dsn->inertia = (parameter_gt)MOTOR_DSN_INERTIA;

    pmsm_dsn->damp = (parameter_gt)MOTOR_DSN_DAMP;
}

void ctl_setup_pmsm_dsn_consultant(ctl_pmsm_dsn_consultant_t *pmsm_dsn, uint16_t pole_pair, parameter_gt Rs,
                                   parameter_gt Ld, parameter_gt Lq, parameter_gt flux, parameter_gt inertia,
                                   parameter_gt damp)
{
    pmsm_dsn->pole_pair = pole_pair;

    pmsm_dsn->Rs = Rs;

    pmsm_dsn->Ld = Ld;

    pmsm_dsn->Lq = Lq;

    pmsm_dsn->flux = flux;

    pmsm_dsn->inertia = inertia;

    pmsm_dsn->damp = damp;
}

void ctl_dsn_pmsm_Rs_via_RsLL(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt Rll)
{
    pmsm_dsn->Rs = Rll / 2;
}

void ctl_dsn_pmsm_Ls_via_Lsll(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt Lll)
{
    pmsm_dsn->Ld = Lll / 2;
    pmsm_dsn->Lq = Lll / 2;
}

void ctl_dsn_pmsm_load(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt inertia, parameter_gt damp)
{
    pmsm_dsn->inertia = inertia;
    pmsm_dsn->damp = damp;
}

parameter_gt ctl_consult_pmsm_flux_Wb(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    return pmsm_dsn->flux;
}

parameter_gt ctl_consult_pmsm_Ke(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    return pmsm_dsn->flux * (parameter_gt)CTL_CONST_PARAM_2_SQRT_6;
}

parameter_gt ctl_consult_pmsm_Ke_Vp_krpm(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    return pmsm_dsn->flux * (parameter_gt)CTL_CONST_PARAM_2_SQRT_6 * (parameter_gt)CTL_CONST_PARAM_100PI_OVER_3 *
           (parameter_gt)CTL_CONST_PARAM_SQRT_2;
}

parameter_gt ctl_consult_pmsm_Ke_Vrms_krpm(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    return pmsm_dsn->flux * (parameter_gt)CTL_CONST_PARAM_2_SQRT_6 * (parameter_gt)CTL_CONST_PARAM_100PI_OVER_3;
}

parameter_gt ctl_consult_pmsm_Kt(ctl_pmsm_dsn_consultant_t *pmsm_dsn)
{
    return pmsm_dsn->flux * (parameter_gt)CTL_CONST_PARAM_3_OVER_2 * pmsm_dsn->pole_pair;
}

void ctl_set_pmsm_flux_Wb(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt flux)
{
    pmsm_dsn->flux = flux;
}

void ctl_dsn_pmsm_flux_via_Ke(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt Ke)
{
    pmsm_dsn->flux = Ke / (parameter_gt)CTL_CONST_PARAM_2_SQRT_6;
}

void ctl_dsn_pmsm_flux_via_Ke_Vp_krpm(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt Ke)
{
    pmsm_dsn->flux = Ke / (parameter_gt)CTL_CONST_PARAM_2_SQRT_6 /
                     ((parameter_gt)CTL_CONST_PARAM_100PI_OVER_3 * (parameter_gt)CTL_CONST_PARAM_SQRT_2);
}

void ctl_dsn_pmsm_flux_via_Ke_Vrms_krpm(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt Ke)
{
    pmsm_dsn->flux = Ke / (parameter_gt)CTL_CONST_PARAM_2_SQRT_6 / (parameter_gt)CTL_CONST_PARAM_100PI_OVER_3;
}

void ctl_dsn_pmsm_pmsm_flux_via_Kt(ctl_pmsm_dsn_consultant_t *pmsm_dsn, parameter_gt Kt)
{
    pmsm_dsn->flux = Kt / ((parameter_gt)CTL_CONST_PARAM_3_OVER_2 * pmsm_dsn->pole_pair);
}

//////////////////////////////////////////////////////////////////////////
// const f module

#include <ctl/component/motor_control/basic/vf_generator.h>

ec_gt ctl_init_const_f_controller(ctl_const_f_controller *ctrl)
{
    ctrl->enc.elec_position = 0;
    ctrl->enc.position = 0;

    ctl_init_ramp_gen(&ctrl->rg);

    return GMP_EC_OK;
}

ec_gt ctl_setup_const_f_controller(ctl_const_f_controller *ctrl, parameter_gt frequency, parameter_gt isr_freq)
{
    // ctl_setup_ramp_gen(&ctrl->rg, float2ctrl(frequency / isr_freq), 1, 0);

    ctl_setup_ramp_gen_via_amp_freq(&ctrl->rg, isr_freq, frequency, 1, 0);

    return GMP_EC_OK;
}
