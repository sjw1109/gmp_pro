// This is a decouple module of PMSM motor

#ifndef _FILE_MTR_CTRL_DECOUPLE_H_
#define _FILE_MTR_CTRL_DECOUPLE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// PMSM decouple voltage feed forward
GMP_STATIC_INLINE
void ctl_mtr_pmsm_decouple(ctl_vector2_t *vdq_ff, ctl_vector2_t *idq, ctrl_gt lsd, ctrl_gt lsq, ctrl_gt omega_e,
                           ctrl_gt psi_e)
{
    vdq_ff->dat[phase_d] = -idq->dat[phase_q] * lsq * omega_e;
    vdq_ff->dat[phase_q] = (idq->dat[phase_d] * lsd + psi_e) * omega_e;
}

// Induction motor & 3-ph grid filter voltage feed forward
GMP_STATIC_INLINE
void ctl_mtr_decouple(ctl_vector2_t *vdq_ff, ctl_vector2_t *idq, ctrl_gt lsd, ctrl_gt lsq, ctrl_gt omega_e)
{
    vdq_ff->dat[phase_d] = -idq->dat[phase_q] * lsq * omega_e;
    vdq_ff->dat[phase_q] = idq->dat[phase_d] * lsd * omega_e;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_MTR_CTRL_DECOUPLE_H_
