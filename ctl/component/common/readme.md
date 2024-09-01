# common blocks for controller

This folder provides a set of common controller modules.


All the functions are organized as the following rules



// ctl_ : as prefix
// <action>_ : such as step(calculate one step), get, init, setup, and so on. 
               init, setup function should only be called in initialization process.
               get, set, step function may be called in main ISR
               input function should be called in input stage
               output function should be called in output stage
// <target object> : such as ramp, sincos, track_pid, and so on.
// _via_<data_source> : [optional] to specify the data source
//                      such as angle, pos_encoder, and so on.
// _<configurations> : [optional] to specify the key point of this function.
//                     such as 2(another implementation), par(parallel topology), and so on.
// Some examples show as follow
// + ctl_init_pid, ctl_setup_pid, ctl_set_pid_parameter, ctl_set_pid_limit
//   from <pid.h> 
// + ctl_init_ramp_gen, ctl_setup_ramp_gen, ctl_setup_ramp_gen_via_amp_freq, ctl_step_ramp_gen
//   from <signal_generator.h>

