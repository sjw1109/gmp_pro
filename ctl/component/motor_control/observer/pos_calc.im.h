
#ifndef _FILE_POS_CALC_IM_H_
#define _FILE_POS_CALC_IM_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct _tag_im_spd_calc
    {
        // output motor slip
        ctrl_gt slip;

        // output electrical angle encoder
        rotation_ift enc;

        // variable sync ratating d-axis magetizing current(p.u.)
        ctrl_gt imds;

        // variable stator speed
        ctrl_gt omega_s;

        // parameter magnetizing current constant
        ctrl_gt kr;

        // parameter slip calculate constant
        ctrl_gt kt;

        // parameter rotor flux angle calculate constant
        ctrl_gt ktheta;

    } ctl_im_spd_calc_t;

    void ctl_init_im_spd_calc(
        // IM speed calculate object
        ctl_im_spd_calc_t *calc,
        // rotor parameters, unit Ohm, H
        parameter_gt Rr, parameter_gt Lr,
        // base electrical frequency(Hz), ISR frequency (Hz)
        parameter_gt freq_base, parameter_gt isr_freq);

    GMP_STATIC_INLINE
    void ctl_clear_im_spd_calc(ctl_im_spd_calc_t *calc)
    {
        // clear parameters
        calc->imds = 0;
        calc->slip = 0;
        calc->omega_s = 0;
        calc->enc.elec_position = 0;
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_step_im_spd_calc(ctl_im_spd_calc_t *calc, ctrl_gt isd, ctrl_gt isq, ctrl_gt omega_r)
    {
        calc->imds += ctl_mul(calc->kr, (isd - calc->imds));
        calc->slip = ctl_div(ctl_mul(calc->kt, isq), calc->imds);

        calc->omega_s = omega_r + calc->slip;
        calc->enc.elec_position += ctl_mul(calc->ktheta, calc->omega_s) + GMP_CONST_1;

        calc->enc.elec_position = ctrl_mod_1(calc->enc.elec_position);

        // if (calc->theta > GMP_CONST_1)
        //     calc->enc.elec_position -= GMP_CONST_1;
        // else if (calc->theta < 0)
        //     calc->enc.elec_position += GMP_CONST_1;

        calc->enc.position = calc->enc.elec_position;

        return calc->enc.elec_position;
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_POS_CALC_IM_H_