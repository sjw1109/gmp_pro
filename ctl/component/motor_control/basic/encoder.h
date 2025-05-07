/**
 * @file encoder.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// based on filter module
#include "core/std/gmp_cport.h"

#include <ctl/component/intrinsic/discrete/divider.h>
#include <ctl/component/intrinsic/discrete/discrete_filter.h>
#include <ctl/component/motor_control/basic/motor_universal_interface.h>

#ifndef _FILE_ENCODER_H_
#define _FILE_ENCODER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // position encoder

    // absolute position encoder
    typedef struct _tag_ctl_pos_encoder_t
    {
        // output interface, encoder output interface
        rotation_ift encif;

        // raw data of position encoder
        uint32_t raw;

        // offset,
        // position + offset means the true position
        ctrl_gt offset;

        // pole_pairs
        // pole_pairs*(position + offset) = Electrical position
        uint16_t pole_pairs;

        // uint32_t p.u. base value
        uint32_t position_base;

    } pos_encoder_t;

    // void ctl_init_pos_encoder(ctl_pos_encoder_t *pos_encoder);

    // void ctl_setup_pos_encoder(ctl_pos_encoder_t *enc, uint16_t poles, uint32_t position_base);

    void ctl_init_pos_encoder(pos_encoder_t *enc, uint16_t poles, uint32_t position_base);

    // This function may calculate and get angle from encoder source data
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_pos_encoder(pos_encoder_t *enc, uint32_t raw)
    {
        // record raw data
        enc->raw = raw;

        // calculate absolute mechanical position
        enc->encif.position = ctl_div(raw, enc->position_base);

        // calculate electrical position
        ctrl_gt elec_pos = enc->pole_pairs * (enc->encif.position + GMP_CONST_1 - enc->offset);
        ctrl_gt elec_pos_pu = ctrl_mod_1(elec_pos);

        // record electrical position data
        enc->encif.elec_position = elec_pos_pu;

        return enc->encif.elec_position;
    }

    // GMP_STATIC_INLINE
    // void ctl_input_pos_encoder_shift(ctl_pos_encoder_t* pos_encoder,
    //	uint32_t raw, fast_gt shift)
    //{
    //	pos_encoder->position = raw << shift;
    // }

    //// GET ANGLE, unit rad
    // GMP_STATIC_INLINE
    // ctrl_gt ctl_get_elec_angle_via_pos_encoder(ctl_pos_encoder_t *pos_encoder)
    //{
    //     ctrl_gt elec_pos = pos_encoder->poles * (pos_encoder->position + GMP_CONST_1 - pos_encoder->offset);

    //    ctrl_gt elec_pos_pu = ctrl_mod_1(elec_pos);

    //    //	ctrl_gt angle = ctl_mul(elec_pos_pu, GMP_CONST_2_PI);

    //    pos_encoder->elec_angle = elec_pos_pu;

    //    return pos_encoder->elec_angle;
    //}

    GMP_STATIC_INLINE
    void ctl_set_pos_encoder_offset(pos_encoder_t *enc, ctrl_gt offset)
    {
        enc->offset = offset;
    }

    //////////////////////////////////////////////////////////////////////////
    // Multi-turns encoder

    // User should input revolution of motor
    typedef struct _tag_ctl_pos_multiturn_encoder_t
    {
        // output interface, encoder output interface
        rotation_ift encif;

        // input raw encoder data
        uint32_t raw;

        // offset,
        // position + offset means the true position
        ctrl_gt offset;

        // pole_pairs
        // poles*(position + offset) = Electrical position
        uint16_t pole_pairs;

        // uint32_t p.u. base value
        uint32_t position_base;

    } pos_multiturn_encoder_t;

    void ctl_init_multiturn_pos_encoder(pos_multiturn_encoder_t *enc, uint16_t poles, uint32_t position_base);

    // This function may calculate and get angle from encoder source data
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_multiturn_pos_encoder(pos_multiturn_encoder_t *enc, uint32_t raw, int32_t revolutions)
    {
        // record raw data
        enc->raw = raw;

        // calculate absolute mechanical position
        enc->encif.position = ctl_div(raw, enc->position_base);

        // calculate electrical position
        ctrl_gt elec_pos = enc->pole_pairs * (enc->encif.position + GMP_CONST_1 - enc->offset);
        ctrl_gt elec_pos_pu = ctrl_mod_1(elec_pos);

        // record electrical position data
        enc->encif.elec_position = elec_pos_pu;

        // record revolutions of motor
        enc->encif.revolutions = revolutions;

        return enc->encif.elec_position;
    }

    //////////////////////////////////////////////////////////////////////////
    // Auto - turn position encoder

    // only need position as input, turns will be calculated automatically.
    typedef struct _tag_ctl_pos_autoturn_encoder_t
    {

        // output interface, encoder output interface
        rotation_ift encif;

        // input raw encoder data
        uint32_t raw;

        // offset,
        // position + offset means the true position
        ctrl_gt offset;

        // pole_pairs
        // poles*(position + offset) = Electrical position
        uint16_t pole_pairs;

        // record last position to calculate revolutions.
        ctrl_gt last_pos;

        // uint32_t p.u. base value
        uint32_t position_base;

    } pos_autoturn_encoder_t;

    void ctl_init_autoturn_pos_encoder(pos_autoturn_encoder_t *enc, uint16_t poles, uint32_t position_base);

    // This function may calculate and get angle from encoder source data
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_autoturn_pos_encoder(pos_autoturn_encoder_t *enc, uint32_t raw)
    {
        // record raw data
        enc->raw = raw;

        // calculate absolute mechanical position
        enc->encif.position = ctl_div(raw, enc->position_base);

        // calculate electrical position
        ctrl_gt elec_pos = (enc->encif.position + GMP_CONST_1 - enc->offset) * enc->pole_pairs;
        ctrl_gt elec_pos_pu = ctrl_mod_1(elec_pos);

        // record electrical position data
        enc->encif.elec_position = elec_pos_pu;

        // judge if multi turn count event has occurred.
        if (enc->encif.position - enc->last_pos > GMP_CONST_1_OVER_2)
        {
            // position has a negative step
            enc->encif.revolutions -= 1;
        }
        if (enc->last_pos - enc->encif.position > GMP_CONST_1_OVER_2)
        {
            // position has a positive step
            enc->encif.revolutions += 1;
        }

        // record last position
        enc->last_pos = enc->encif.position;

        return enc->encif.elec_position;
    }

    //////////////////////////////////////////////////////////////////////////
    // speed encoder
    typedef struct _tag_ctl_volocity_encoder_t
    {
        // output: Speed p.u. value
        velocity_ift encif;

        // Speed p.u. base, unit rad/s or rpm
        parameter_gt speed_base;

        // Speed krpm value
        ctrl_gt speed_krpm;
    } spd_encoder_t;

    // void ctl_init_spd_encoder(ctl_spd_encoder_t *spd_encoder);

    // void ctl_setup_spd_encoder(ctl_spd_encoder_t *enc, parameter_gt speed_base);

    void ctl_init_spd_encoder(spd_encoder_t *enc, parameter_gt speed_base);

    //////////////////////////////////////////////////////////////////////////
    // Speed calculator based on position

    typedef struct _tag_speed_calculator_t
    {
        // output: Speed p.u. value
        velocity_ift encif;

        // input postion p.u. input value
        rotation_ift *pos_encif;

        // position old data
        ctrl_gt old_position;

        // speed scale factor
        // speed = delta_ticks * scale_factor
        ctrl_gt scale_factor;

        // speed filter
        ctl_low_pass_filter_t spd_filter;

        // speed division
        ctl_divider_t div;

    } spd_calculator_t;

    // void ctl_init_spd_calculator(ctl_spd_calculator_t *sc);

    //// (pos - old_pos) * speed_calc_freq -> rad/s
    //// 60/(2*pi)*(rad/s) -> rpm
    //// rpm / (pole_pairs * rate_speed_rpm) -> Mech p.u.

    //// User must ensure fcalc > n_max_rpm / 30
    // void ctl_setup_spd_calculator(
    //     // speed calculator objects
    //     ctl_spd_calculator_t *sc,
    //     // control law frequency, unit Hz
    //     parameter_gt control_law_freq,
    //     // division of control law frequency, unit ticks
    //     uint16_t speed_calc_div,
    //     // Speed per unit base value, unit rpm
    //     parameter_gt rated_speed_rpm,
    //     // pole pairs, if you pass a elec-angle,
    //     uint16_t pole_pairs,
    //     // just set this value to 1.
    //     // generally, speed_filter_fc approx to speed_calc freq divided by 5
    //     parameter_gt speed_filter_fc,
    //     // link to a position encoder
    //     ctl_rotation_encif_t *pos_encif);

    void ctl_init_spd_calculator(
        // speed calculator objects
        spd_calculator_t *sc,
        // link to a position encoder
        rotation_ift *pos_encif,
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
        parameter_gt speed_filter_fc);

    // Step Speed calculate function
    GMP_STATIC_INLINE
    void ctl_step_spd_calc(spd_calculator_t *sc)
    {
        //        ctrl_gt CTRL_PI = GMP_CONST_PI;
        //        ctrl_gt CTRL_2PI = GMP_CONST_2_PI;

        if (ctl_step_divider(&sc->div))
        {

            // sc->old_position = sc->position;
            // sc->position = position_rad;

            ctrl_gt delta = ctl_get_encoder_position(sc->pos_encif) - sc->old_position;
            // direction correction
            if (delta < -GMP_CONST_1_OVER_2)
            {
                delta += GMP_CONST_1;
            }
            else if (delta > GMP_CONST_1_OVER_2)
            {
                delta -= GMP_CONST_1;
            }

            ctrl_gt new_spd = ctl_mul((delta), sc->scale_factor);

            sc->encif.speed = ctl_step_lowpass_filter(&sc->spd_filter, new_spd);
            // sc->speed = sc->spd_filter.out;

            // update old position
            sc->old_position = ctl_get_encoder_position(sc->pos_encif);
        }
    }

    // This function is obsolete, use `ctl_get_encoder_speed` instead.
    // GMP_STATIC_INLINE
    // ctrl_gt ctl_get_spd_via_pos_encoder(ctl_spd_calculator_t *sc)
    //{
    //    return sc->speed;
    //}

    // GMP_STATIC_INLINE
    // void ctl_enable_spd_calc(ctl_spd_calculator_t* sc)
    //{
    //	sc->div.flag_bypass = 0;
    // }
    //
    // GMP_STATIC_INLINE
    // void ctl_disable_spd_calc(ctl_spd_calculator_t* sc)
    //{
    //	sc->div.flag_bypass = 1;
    // }

    // input variables
    // absolute position encoder
    // QEP encoder
    // Hall encoder
    // Observer
    // sin/cos encoder

    // general header for encoder
    // typedef struct _tag_gmp_encoder_t
    //{
    //	// output variables
    //	gmp_math_t mech_ang; // p.u.
    //	int32_t revolution;  // turns
    //	gmp_math_t elec_ang; // p.u.
    //
    //	// derived variables
    //	gmp_math_t speed;    // unit: krpm
    //
    //	// intrinsic variables
    //	uint16_t pole_pairs;
    //	gmp_math_t mech_ang_offset; // offset of mechanical angle, p.u.
    //
    //}gmp_enc_t;
    //
    //// QEP encoder
    // typedef struct _tag_gmp_qep_encoder_t
    //{
    //	// general header
    //	gmp_enc_t header;
    //
    //	// input variables
    ////	uint32_t cnt;
    //
    //	// intermediate variables
    //	uint32_t cnt_last;
    //
    //	// intrinsic variables
    //	uint32_t lines;        // encoder lines
    //	uint32_t division;     // division factor for control frequency
    //
    //}gmp_qep_encoder_t;
    //
    //// Tamagawa TS5700N8401 series absolute position encoder
    // typedef struct _tag_gmp_tamagawa_encoder_t
    //{
    //	// general header
    //	gmp_enc_t header;
    //
    //	// input variables
    //
    //
    // }gmp_tamagawa_encoder_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_ENCODER_H_
