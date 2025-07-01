#include <gmp_core.h>


#include <ctl/component/digital_power/basic/boost.h>

void ctl_init_boost_ctrl(
    // Boost controller
    boost_ctrl_t *boost,
    // Voltage PID controller
    parameter_gt v_kp, parameter_gt v_Ti, parameter_gt v_Td,
    // Current PID controller
    parameter_gt i_kp, parameter_gt i_Ti, parameter_gt i_Td,
    // Controller frequency, Hz
    parameter_gt fs)
{
    ctl_init_pid(
        // continuous pid handle
        &boost->current_pid,
        // PID parameters
        v_kp, v_Ti, v_Td,
        // controller frequency
        fs);

    ctl_init_pid(
        // continuous pid handle
        &boost->voltage_pid,
        // PID parameters
        i_kp, i_Ti, i_Td,
        // controller frequency
        fs);
}

