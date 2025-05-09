// SOGI, Second Order Generalized. Integrator

#ifndef _FILE_SOGI_H_
#define _FILE_SOGI_H_

typedef struct _tag_sogi
{
    // input

    // output

    // parameters

    // resonant frequency
    ctrl_gt k_r;

    // damp coefficient
    ctrl_gt k_damp;

    // gain
    ctrl_gt gain;

    // intermediate

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
    parameter_gt freq_ctrl)
{
    sogi->k_damp = float2ctrl(2 * freq_c / freq_r);
    sogi->k_r = float2ctrl(2 * PI * freq_r / freq_ctrl);
    sogi->gain = float2ctrl(gain);
}

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
    parameter_gt freq_ctrl)
{
    sogi->k_damp = float2ctrl(damp);
    sogi->k_r = float2ctrl(2 * PI * freq_r / freq_ctrl);
    sogi->gain = float2ctrl(gain);
}


#endif // _FILE_SOGI_H_
