// MTPA controller
// For Salient Pole PMSM, Ld != Lq
// in order to gain maximum torque is is total current command should be ssaperate into two parts

// tex:
//$$
//  i_{d}=\frac{\sqrt{\psi_{f}^{2}+8\left(L_{d}-L_{q}\right)^{2}i_{s}^{2}}-\psi_{f}}{4\left(L_{d}-L_{q}\right)}
//$$

#ifndef _FILE_PMSM_MTPA_H_
#define _FILE_PMSM_MTPA_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


typedef struct _tag_mpta_current_distributor
{
    // input parameters
    ctrl_gt input_current;

} mtpa_current_distributor_t;


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // _FILE_PMSM_MTPA_H_
