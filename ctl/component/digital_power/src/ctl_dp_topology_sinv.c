/**
 * @file ctl_dp_topology_sinv.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-07-20
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#include <gmp_core.h>

#include <math.h>

#include <ctl/component/digital_power/topology_preset/single_phase_dc_ac.h>

void ctl_upgrade_sinv_param(sinv_ctrl_t *sinv, sinv_init_t *init)
{
    ctl_init_single_phase_pll(&sinv->spll, init->pll_ctrl_kp, init->pll_ctrl_Ti, init->pll_ctrl_cut_freq,
                              init->base_freq, init->f_ctrl);

 // ctl_init_ramp_gen(&sinv->rg, float2ctrl(1.0f / init->base_freq), 1, 0);
    ctl_init_ramp_gen_via_amp_freq(&sinv->rg, init->f_ctrl, init->base_freq, 1, 0);

    ctl_init_lp_filter(&sinv->lpf_idc, init->f_ctrl, init->adc_filter_fc);
    ctl_init_lp_filter(&sinv->lpf_udc, init->f_ctrl, init->adc_filter_fc);
    ctl_init_lp_filter(&sinv->lpf_il, init->f_ctrl, init->adc_filter_fc);
    ctl_init_lp_filter(&sinv->lpf_igrid, init->f_ctrl, init->adc_filter_fc);
    ctl_init_lp_filter(&sinv->lpf_ugrid, init->f_ctrl, init->adc_filter_fc);

    ctl_init_pid_ser(&sinv->voltage_pid, init->v_ctrl_kp, init->v_ctrl_Ti, init->v_ctrl_Td, init->f_ctrl);

    ctl_init_qpr_controller(&sinv->sinv_qpr_base, init->i_ctrl_kp, init->i_ctrl_kr, init->base_freq,
                            init->i_ctrl_cut_freq, init->f_ctrl);

    ctl_init_qr_controller(&sinv->sinv_qr_3, init->harm_ctrl_kr_3, init->base_freq * 3.0f, init->harm_ctrl_cut_freq_3,
                           init->f_ctrl);
    ctl_init_qr_controller(&sinv->sinv_qr_5, init->harm_ctrl_kr_5, init->base_freq * 5.0f, init->harm_ctrl_cut_freq_5,
                           init->f_ctrl);
    ctl_init_qr_controller(&sinv->sinv_qr_7, init->harm_ctrl_kr_7, init->base_freq * 7.0f, init->harm_ctrl_cut_freq_7,
                           init->f_ctrl);
}

void ctl_init_sinv_ctrl(sinv_ctrl_t *sinv, sinv_init_t *init)
{
    ctl_upgrade_sinv_param(sinv, init);

    ctl_clear_sinv(sinv);

    sinv->pf_set = 1;
}

void ctl_attach_sinv_with_adc(sinv_ctrl_t *sinv, adc_ift *udc, adc_ift *idc, adc_ift *il, adc_ift *ugrid,
                              adc_ift *igrid)
{
    sinv->adc_idc = idc;
    sinv->adc_udc = udc;
    sinv->adc_il = il;
    sinv->adc_igrid = igrid;
    sinv->adc_ugrid = ugrid;
}
