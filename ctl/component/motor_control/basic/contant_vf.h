/**
 * @file contant_vf.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/component/intrinsic/discrete/stimulate.h>
#include <ctl/component/motor_control/basic/encoder_if.h>

typedef struct _tag_const_vf
{
    // encoder output
    ctl_rotation_encif_t enc;

    // parameters
    // E = 4.44 N \Phi \times f
    ctrl_gt v_over_f;

    // ramp generator
    ctl_src_rg_t rg;

} ctl_const_vf_controller;

// generate constant frequency
typedef struct _tag_const_f
{
    // encoder output
    ctl_rotation_encif_t enc;

    // ramp generator
    ctl_src_rg_t rg;

} ctl_const_f_controller;

ec_gt ctl_init_const_f_controller(ctl_const_f_controller *ctrl)
{
    ctrl->enc.elec_position = 0;
    ctrl->enc.position = 0;

    ctl_init_ramp_gen(&ctrl->rg);

    return GMP_EC_OK;
}

ec_gt ctl_setup_const_f_controller(ctl_const_f_controller *ctrl, parameter_gt frequency, parameter_gt isr_freq)
{
    //ctl_setup_ramp_gen(&ctrl->rg, float2ctrl(frequency / isr_freq), 1, 0);

    ctl_setup_ramp_gen_via_amp_freq(&ctrl->rg, isr_freq, frequency, 1, 0);

    return GMP_EC_OK;
}

void ctl_step_const_f_controller(ctl_const_f_controller *ctrl)
{
    ctrl->enc.elec_position = ctl_step_ramp_gen(&ctrl->rg);
    ctrl->enc.position = ctrl->enc.elec_position;
}
