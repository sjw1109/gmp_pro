/**
 * @file stimulate.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#ifndef _FILE_SIGNAL_GENERATOR_H_
#define _FILE_SIGNAL_GENERATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// Sine & Cosine Wave generator
typedef struct _tag_sine_generator_t
{
    // frequency
    ctrl_gt ph_sin_delta;
    ctrl_gt ph_cos_delta;

    // current phasor
    ctrl_gt ph_sin;
    ctrl_gt ph_cos;

} ctl_src_sg_t;

// ec_gt ctl_init_sincos_gen(ctl_src_sg_t *sg);

// ec_gt ctl_setup_sincos_gen(ctl_src_sg_t *sg,
//                            ctrl_gt init_angle,  // rad
//                            ctrl_gt step_angle); // rad

void ctl_init_sincos_gen(ctl_src_sg_t *sg,
                         parameter_gt init_angle,  // pu
                         parameter_gt step_angle); // pu

    GMP_STATIC_INLINE
void ctl_step_sincos_gen(ctl_src_sg_t *sg)
{
    ctrl_gt sin_new = sg->ph_sin * sg->ph_cos_delta + sg->ph_cos * sg->ph_sin_delta;
    ctrl_gt cos_new = sg->ph_cos * sg->ph_cos_delta - sg->ph_sin * sg->ph_sin_delta;

    sg->ph_sin = sin_new;
    sg->ph_cos = cos_new;
}

GMP_STATIC_INLINE
void ctl_get_sg_sin(ctl_src_sg_t *sg)
{
    return sg->ph_sin;
}

GMP_STATIC_INLINE
void ctl_get_sg_cos(ctl_src_sg_t *sg)
{
    return sg->ph_cos;
}

//////////////////////////////////////////////////////////////////////////
// Ramp wave generator
typedef struct _tag_ramp_generator_t
{
    // output range
    ctrl_gt minimum;
    ctrl_gt maximum;

    // current output
    ctrl_gt current;

    // ramp slope
    ctrl_gt slope;
} ctl_src_rg_t;

// ec_gt ctl_init_ramp_gen(ctl_src_rg_t *rg);

// ec_gt ctl_setup_ramp_gen(ctl_src_rg_t *rg, ctrl_gt slope, parameter_gt amp_pos, parameter_gt amp_neg);

void ctl_init_ramp_gen(ctl_src_rg_t *rg, ctrl_gt slope, parameter_gt amp_pos, parameter_gt amp_neg);

// ec_gt ctl_setup_ramp_gen_via_amp_freq(ctl_src_rg_t *rg, parameter_gt isr_freq, parameter_gt target_freq,
//                                       parameter_gt amp_pos, parameter_gt amp_neg);

void ctl_init_ramp_gen_via_amp_freq(ctl_src_rg_t *rg, parameter_gt isr_freq, parameter_gt target_freq,
                                    parameter_gt amp_pos, parameter_gt amp_neg);

GMP_STATIC_INLINE
ctrl_gt ctl_step_ramp_gen(ctl_src_rg_t *rg)
{
    rg->current += rg->slope;

    if (rg->current > rg->maximum)
        rg->current = rg->minimum;

    return rg->current;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_ramp_gen_output(ctl_src_rg_t *rg)
{
    return rg->current;
}

GMP_STATIC_INLINE
void ctl_set_ramp_freq(ctl_src_rg_t *rg, ctrl_gt slope)
{
    rg->slope = slope;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_SIGNAL_GENERATOR_H_
