
#ifndef _FILE_MOTOR_PER_UNIT_CONSULTANT_H_
#define _FILE_MOTOR_PER_UNIT_CONSULTANT_H_

typedef struct _tag_per_unit_consultant_t
{
    parameter_gt base_voltage;    // V: V
    parameter_gt base_power;      // P: W
    parameter_gt base_current;    // I: A

    parameter_gt base_freq;       // f: Hz
    parameter_gt base_speed;      // omega: rad/s
    parameter_gt base_speed_krpm; // n: krpm

    parameter_gt base_impedence;  // R: Ohm
    parameter_gt base_inductance; // L: H
    parameter_gt base_flux;       // phi: Wb

    parameter_gt base_inertia;    // H: kg m^2



}ctl_per_unit_consultant_t;


#endif // _FILE_MOTOR_PER_UNIT_CONSULTANT_H_
