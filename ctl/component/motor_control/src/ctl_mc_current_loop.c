
#include <gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// Motor controller basic structure

#include <ctl/component/motor_control/current_loop/motor_current_ctrl.h>

void ctl_setup_motor_current_ctrl(
    // motor current controller
    ctl_motor_current_ctrl_t *obj,
    // PID parameter for motor current controller
    ctrl_gt kp, ctrl_gt Ti, ctrl_gt Td,
    // PID saturation parameter for motor current controller
    ctrl_gt out_min, ctrl_gt out_max,
    // controller rate, Hz
    parameter_gt fs)
{
    obj->udc = float2ctrl(1.0);
    ctl_vector3_clear(&obj->iabc);
    ctl_vector2_clear(&obj->idq_ref);
    ctl_vector2_clear(&obj->vdq_ff);
    obj->theta = 0;

    // ctl_vector3_clear(&obj->Tabc);
    ctl_vector3_clear(&obj->idq0);
    ctl_vector3_clear(&obj->iab0);
    ctl_vector3_clear(&obj->vdq0);
    ctl_vector3_clear(&obj->vab0);

    ctl_init_pid(&obj->idq_ctrl[phase_d], kp, Ti, Td,fs);
    ctl_set_pid_limit(&obj->idq_ctrl[phase_d], out_min, out_max);
    ctl_init_pid(&obj->idq_ctrl[phase_q], kp, Ti, Td,fs);
    ctl_set_pid_limit(&obj->idq_ctrl[phase_q], out_min, out_max);

    obj->flag_enable_current_controller = 1;
}
