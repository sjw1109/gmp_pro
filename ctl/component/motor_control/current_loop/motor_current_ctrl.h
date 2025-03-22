// FOC Motor Controller general framework
// This module would be implemented by FPGA.

// necessary headers
#include <ctl/component/intrinsic/continuous/continuous_pid.h>
#include <ctl/component/interface/adc_channel.h>
#include <ctl/component/interface/pwm_channel.h>

#include <ctl/math_block/coordinate/coord_trans.h>

#ifndef _FILE_MOTOR_CURRENT_CTRL_H_
#define _FILE_MOTOR_CURRENT_CTRL_H_

// INPUT:
//  + ctrl_gt: udc
//  + vector3: iabc
//  + vector2: idq_ref
//  + vector2: vdq_ff
//  + ctrl_gt: theta

// OUTPUT:
// + vector3: Tabc
// + vector3: idq0
// + vector3: iab0
// + vector2: udq0
// + vector2: uab0

// Structure:
// iab = clark(iabc)
// idq = park(iab)
// pi_iq: PI(iq - iq_ref) = vq_ctrl;
// pi_id: PI(id - id_ref) = vd_crrl;
// vq = vq_ctrl + vq_ff;
// vd = vd_ctrl + vd_ff;
// vab = inv_park(vdq);
// Tabc = svpwm(vab) / udc;

typedef struct _tag_motor_current_ctrl
{
    // input parameters
    ctrl_gt udc;
    ctl_vector3_t iabc;
    ctl_vector2_t idq_ref;
    ctl_vector2_t vdq_ff;
    ctrl_gt theta;

    // output parameters
//    ctl_vector3_t Tabc;
    ctl_vector3_t idq0;
    ctl_vector3_t iab0;
    ctl_vector3_t vdq0;
    ctl_vector3_t vab0;

    // controller objects
    ctl_pid_t idq_ctrl[2];

    // flags
    fast_gt flag_enable_current_controller;

    //fast_gt flag_enable_svpwm;
} ctl_motor_current_ctrl_t;

//ec_gt ctl_init_motor_current_ctrl(ctl_motor_current_ctrl_t *obj);
//
//ec_gt ctl_setup_motor_current_ctrl(ctl_motor_current_ctrl_t *obj,
//                                   // PID parameter for motor current controller
//                                   ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
//                                   // PID saturation parameter for motor current controller
//                                   ctrl_gt out_min, ctrl_gt out_max);

void ctl_setup_motor_current_ctrl(ctl_motor_current_ctrl_t *obj,
                                  // PID parameter for motor current controller
                                  ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
                                  // PID saturation parameter for motor current controller
                                  ctrl_gt out_min, ctrl_gt out_max);

// Clear all the residual informations
void ctl_clear_motor_current_ctrl(ctl_motor_current_ctrl_t *obj);

GMP_STATIC_INLINE
void ctl_input_motor_current_ctrl(ctl_motor_current_ctrl_t *obj, tri_adc_channel_t *adc_channel)
{
    ctl_get_tri_adc_channel_via_vector3(adc_channel, &obj->iabc);
}

GMP_STATIC_INLINE
void ctl_set_motor_current_ctrl_idq_ref(ctl_motor_current_ctrl_t *obj, ctrl_gt id_ref, ctrl_gt iq_ref)
{
    obj->idq_ref.dat[phase_d] = id_ref;
    obj->idq_ref.dat[phase_q] = iq_ref;
}

GMP_STATIC_INLINE
void ctl_set_motor_current_ctrl_vdq_ff(ctl_motor_current_ctrl_t *obj, ctrl_gt vd_ff, ctrl_gt vq_ff)
{
    obj->vdq_ff.dat[phase_d] = vd_ff;
    obj->vdq_ff.dat[phase_q] = vq_ff;
}

// This is the debug variable
// ctl_vector2_t phasor;

GMP_STATIC_INLINE
void ctl_step_motor_current_ctrl(ctl_motor_current_ctrl_t *obj, ctrl_gt theta)
{
    ctl_vector2_t phasor;

    // Save theta
    obj->theta = theta;

    // + iab = clark(iabc)
    ctl_ct_clark(&obj->iabc, &obj->iab0);

    // + phasor = \angle(theta)
    ctl_set_phasor_via_angle(obj->theta, &phasor);
    // idq = park(iab)
    ctl_ct_park(&obj->iab0, &phasor, &obj->idq0);

    // enable current controller
    if (obj->flag_enable_current_controller)
    {
        // pi_iq: PI(iq - iq_ref) = vq_ctrl;
        obj->vdq0.dat[phase_d] =
            ctl_step_pid_ser(&obj->idq_ctrl[phase_d], obj->idq_ref.dat[phase_d] - obj->idq0.dat[phase_d]);

        // pi_id: PI(id - id_ref) = vd_crrl;
        obj->vdq0.dat[phase_q] =
            ctl_step_pid_ser(&obj->idq_ctrl[phase_q], obj->idq_ref.dat[phase_q] - obj->idq0.dat[phase_q]);
    }
    else
    {
        obj->vdq0.dat[phase_d] = 0;
        obj->vdq0.dat[phase_q] = 0;
    }

        // vq = vq_ctrl + vq_ff;
        obj->vdq0.dat[phase_d] += obj->vdq_ff.dat[phase_d];

        // vd = vd_ctrl + vd_ff;
        obj->vdq0.dat[phase_q] += obj->vdq_ff.dat[phase_q];

        obj->vdq0.dat[phase_0] = 0;

        // vab = inv_park(vdq);
        ctl_ct_ipark(&obj->vdq0, &phasor, &obj->vab0);

    // if (obj->flag_enable_svpwm)
    // {
    //     // Tabc = svpwm(vab) / udc;
    //     //ctl_ct_svpwm_calc(&obj->vab0, &obj->Tabc);
    // }
    // else
    // {
    //     ctl_vector3_clear(&obj->Tabc);
    // }

}

GMP_STATIC_INLINE
void ctl_enable_motor_current_controller(ctl_motor_current_ctrl_t *obj)
{
    obj->flag_enable_current_controller = 1;
}

GMP_STATIC_INLINE
void ctl_disable_motor_current_controller(ctl_motor_current_ctrl_t *obj)
{
    obj->flag_enable_current_controller = 0;
}

//GMP_STATIC_INLINE
//void ctl_get_motor_current_controller_modulation(ctl_motor_current_ctrl_t *obj, ctl_vector3_t *tabc)
//{
//    tabc->dat[phase_A] = obj->Tabc.dat[phase_A];
//    tabc->dat[phase_B] = obj->Tabc.dat[phase_B];
//    tabc->dat[phase_C] = obj->Tabc.dat[phase_C];
//}

GMP_STATIC_INLINE
ctrl_gt ctl_get_motor_current_controller_id(ctl_motor_current_ctrl_t *obj)
{
    return obj->idq0.dat[phase_d];
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_motor_current_controller_iq(ctl_motor_current_ctrl_t *obj)
{
    return obj->idq0.dat[phase_q];
}

//GMP_STATIC_INLINE
//void ctl_set_motor_current_controller_zero_output(ctl_motor_current_ctrl_t *obj)
//{
//    obj->Tabc.dat[0] = float2ctrl(0.5);
//    obj->Tabc.dat[1] = float2ctrl(0.5);
//    obj->Tabc.dat[2] = float2ctrl(0.5);
//}

#endif // _FILE_MOTOR_CURRENT_CTRL_H_
