// FOC Motor Controller general framework
// This module would be implemented by FPGA.


svpwm_channel_t

// INPUT:
//  + ctrl_gt: udc
//  + vector3: iabc
//  + vector2: idq_ref
//  + vector2: vdq_ff
//  + ctrl_gt: theta

// OUTPUT:
// + vector3: Tabc
// + vector2: idq
// + vector2: iab
// + vector2: udq
// + vector2: uab

// Structure:
// iab = clark(iabc)
// idq = park(iab)
// pi_iq: PI(iq - iq_ref) = vq_ctrl;
// pi_id: PI(id - id_ref) = vd_crrl;
// vq = vq_ctrl + vq_ff;
// vd = vd_ctrl + vd_ff;
// vab = inv_park(vdq);
// Tabc = svpwm(vab) / udc;

struct mc_foc_core
{
    // input parameters
    ctrl_gt udc;
    ctl_vector3_t iabc;
    ctl_vector2_t idq_ref;
    ctl_vector2_t vdq_ff;
    ctrl_gt theta;

    // output parameters
    ctl_vector3_t Tabc;
    ctl_vector2_t idq;
    ctl_vector2_t iab;
    ctl_vector2_t udq;
    ctl_vector2_t uab;

    // controller objects
    ctl_pid_t idq[2];
    ctl_svpwm_channel_t svpwm;

    // flags
    fast_gt flag_enable_current_controller;

};
