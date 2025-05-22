

#ifndef _FILE_IM_CONSULTANT_H_
#define _FILE_IM_CONSULTANT_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_im_dsn_consultant
{
    // Motor Design parameters, Pole pairs
    // unit, 1
    uint16_t pole_pair;

    // Motor Design parameters, stator parameters
    // unit, SI Ohm
    parameter_gt Rs;

    // Motor Design parameters, Armature inductance Ls
    // unit, SI H
    parameter_gt Ls;

    // Motor Design parameters, Rotor parameters
    // unit SI Ohm
    parameter_gt Rr;

    // Motor Design parameters, Rotor inductance Lr
    // unit, SI H
    parameter_gt Lr;

    // Motor Design parameters, mutual inductance Lm
    // unit, SI H
    parameter_gt Lm;

    // Motor Design parameters, inertia
    // unit, kg.m/s2
    parameter_gt inertia;

    // Motor Design parameters, damping
    parameter_gt damp;

} im_dsn_consultant_t;

// setup the Inductor motor design consultant object
// if any parameter is unknown, fill 0, except pole_pair
void ctl_setup_im_dsn_consultant(im_dsn_consultant_t *im_dsn, uint16_t pole_pair, parameter_gt Rs, parameter_gt Ls,
                                 parameter_gt Rr, parameter_gt Lr, parameter_gt Lm, parameter_gt inertia,
                                 parameter_gt damp)
{
    im_dsn->pole_pair = pole_pair;
    im_dsn->Rs = Rs;
    im_dsn->Ls = Ls;
    im_dsn->Rr = Rr;
    im_dsn->Lr = Lr;
    im_dsn->Lm = Lm;
    im_dsn->inertia = inertia;
    im_dsn->damp = damp;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_IM_CONSULTANT_H_
