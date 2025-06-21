// Discrete SOGI object

#ifndef _FILE_DISCRETE_SOGI_H_
#define _FILE_DISCRETE_SOGI_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// clang-format off

// assume input is R(s)
//
// d_integrate is D(s), which is a band pass filter, then
//tex:
// $$
// \frac{D(s)}{Q(s)}=\frac{k\omega_0 s}{s^2+k\omega_0 s+\omega_0^2}
// $$
//

// q_integrate is Q(s), which is a low pass filter, then
//tex:
// $$
// \frac{D(s)}{Q(s)}=\frac{k\omega_0^2}{s^2+k\omega_0 s+\omega_0^2}
// $$
//

// clang-format on

typedef struct _tag_discrete_sogi
{
    //
    // Input Section
    //

    // input buffer
    float32_t u[3]; //!< AC input data buffer

    //
    // Output Section
    //

    //
    // orthogonal signal generator
    // Low pass Filter
    //
    float32_t osg_u[3];

    // orthogonal quadrature signal generator
    // Band pass Filter
    float32_t osg_qu[3];

    //
    // Parameters Section
    //
    ctrl_gt b0;
    ctrl_gt b2;
    ctrl_gt a1;
    ctrl_gt a2;
    ctrl_gt qb0;
    ctrl_gt qb1;
    ctrl_gt qb2;
} discrete_sogi_t;

GMP_STATIC_INLINE
void ctl_clear_discrete_sogi(discrete_sogi_t *sogi)
{
    sogi->u[0] = float2ctrl(0.0f);
    sogi->u[1] = float2ctrl(0.0f);
    sogi->u[2] = float2ctrl(0.0f);

    sogi->osg_u[0] = float2ctrl(0.0f);
    sogi->osg_u[1] = float2ctrl(0.0f);
    sogi->osg_u[2] = float2ctrl(0.0f);

    sogi->osg_qu[0] = float2ctrl(0.0f);
    sogi->osg_qu[1] = float2ctrl(0.0f);
    sogi->osg_qu[2] = float2ctrl(0.0f);
}

void ctl_init_discrete_sogi(
    // Handle of discrete SOGI object
    discrete_sogi_t *sogi,
    // damp coefficient, generally is 0.5
    parameter_gt k_damp,
    // center frequency, Hz
    parameter_gt fn,
    // isr frequency, Hz
    parameter_gt fs);

GMP_STATIC_INLINE
void ctl_step_discrete_sogi(discrete_sogi_t *sogi, ctrl_gt u)
{
    sogi->u[0] = u;

    sogi->osg_u[0] = ctl_mul(sogi->b0, (sogi->u[0] - sogi->u[2])) + ctl_mul(sogi->a1, sogi->osg_u[1]) +
                     ctl_mul(sogi->a2, sogi->osg_u[2]);

    sogi->osg_u[2] = sogi->osg_u[1];
    sogi->osg_u[1] = sogi->osg_u[0];

    sogi->osg_qu[0] = ctl_mul(sogi->qb0, sogi->u[0]) + ctl_mul(sogi->qb1 * sogi->u[1]) +
                      ctl_mul(sogi->qb2 * sogi->u[2]) + ctl_mul(sogi->a1 * sogi->osg_qu[1]) +
                      ctl_mul(sogi->a2 * sogi->osg_qu[2]);

    sogi->osg_qu[2] = sogi->osg_qu[1];
    sogi->osg_qu[1] = sogi->osg_qu[0];

    sogi->u[2] = sogi->u[1];
    sogi->u[1] = sogi->u[0];
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_discrete_sogi_ds(discrete_sogi_t *sogi)
{
    return sogi->osg_u[0];
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_discrete_sogi_qs(discrete_sogi_t *sogi)
{
    return sogi->osg_qu[1];
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_DISCRETE_SOGI_H_
