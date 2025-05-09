// SOGI, Second Order Generalized. Integrator

#ifndef _FILE_SOGI_H_
#define _FILE_SOGI_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // assume input is R(s)
    //
    // d_integrate is D(s), which is a band pass filter, then
    // tex:
    // $$
    // \frac{D(s)}{Q(s)}=\frac{k\omega_0 s}{s^2+k\omega_0 s+\omega_0^2}
    // $$
    //

    // q_integrate is Q(s), which is a low pass filter, then
    // tex:
    // $$
    // \frac{D(s)}{Q(s)}=\frac{k\omega_0^2}{s^2+k\omega_0 s+\omega_0^2}
    // $$
    //

    typedef struct _tag_sogi
    {
        // input

        // output
        ctrl_gt d_integrate;
        ctrl_gt q_integrate;

        // parameters

        // resonant frequency
        ctrl_gt k_r;

        // damp coefficient
        ctrl_gt k_damp;

        // gain
        ctrl_gt gain;

        // intermediate
        ctrl_gt integrate_reference;

    } ctl_sogi_t;

    void ctl_init_sogi_controller(
        // controller handle
        ctl_sogi_t *sogi,
        // gain of this controller
        parameter_gt gain,
        // resonant frequency
        parameter_gt freq_r,
        // cut frequency
        parameter_gt freq_c,
        // controller frequency
        parameter_gt freq_ctrl);

    void ctl_init_sogi_controller_with_damp(
        // controller handle
        ctl_sogi_t *sogi,
        // gain of this controller
        parameter_gt gain,
        // resonant frequency
        parameter_gt freq_r,
        // cut frequency, generally 1.414 is a great choice
        parameter_gt damp,
        // controller frequency
        parameter_gt freq_ctrl);

    // default return D result.
    GMP_STATIC_INLINE
    ctrl_gt ctl_step_sogi_controller(
        // controller handle
        ctl_sogi_t *sogi,
        // input reference
        ctrl_gt input)
    {
        sogi->integrate_reference = ctl_mul(sogi->k_damp, input - sogi->d_integrate);
        sogi->d_integrate += ctl_mul(sogi->k_r, sogi->integrate_reference - sogi->q_integrate);
        sogi->q_integrate += ctl_mul(sogi->k_r, sogi->d_integrate);

        return sogi->d_integrate;
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_clear_sogi_controller(
        // controller handle
        ctl_sogi_t *sogi)
    {
        sogi->d_integrate = 0;
        sogi->q_integrate = 0;
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_SOGI_H_
