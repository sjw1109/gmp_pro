
#include <gmp_core.h>

#include <ctl/ctl_core.h>

//////////////////////////////////////////////////////////////////////////
// Motor controller basic structure

#include <ctl/component/motor_control/current_loop/motor_current_ctrl.h>

ec_gt ctl_init_motor_current_ctrl(ctl_motor_current_ctrl_t *obj)
{
    obj->udc = float2ctrl(1.0);
    ctl_vector3_clear(&obj->iabc);
    ctl_vector2_clear(&obj->idq_ref);
    ctl_vector2_clear(&obj->vdq_ff);
    obj->theta = 0;

    //ctl_vector3_clear(&obj->Tabc);
    ctl_vector3_clear(&obj->idq0);
    ctl_vector3_clear(&obj->iab0);
    ctl_vector3_clear(&obj->vdq0);
    ctl_vector3_clear(&obj->vab0);

    ctl_init_pid(&obj->idq_ctrl[phase_d]);
    ctl_init_pid(&obj->idq_ctrl[phase_q]);

    obj->flag_enable_current_controller = 1;
    obj->flag_enable_svpwm = 1;

    return GMP_EC_OK;
}

ec_gt ctl_setup_motor_current_ctrl(ctl_motor_current_ctrl_t *obj,
                                   // PID parameter for motor current controller
                                   ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
                                   // PID saturation parameter for motor current controller
                                   ctrl_gt out_min, ctrl_gt out_max)
{
    ctl_setup_pid(&obj->idq_ctrl[phase_d], kp, ki, kd, out_min, out_max);
    ctl_setup_pid(&obj->idq_ctrl[phase_q], kp, ki, kd, out_min, out_max);

    return GMP_EC_OK;
}

void ctl_clear_motor_current_ctrl(ctl_motor_current_ctrl_t *obj)
{
    // clear PID controller
    ctl_clear_pid(&obj->idq_ctrl[0]);
    ctl_clear_pid(&obj->idq_ctrl[1]);

    // clear Feed forward controller
    obj->vdq_ff.dat[0] = 0;
    obj->vdq_ff.dat[1] = 0;
}

