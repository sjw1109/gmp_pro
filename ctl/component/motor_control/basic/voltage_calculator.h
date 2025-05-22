// This module is used to calclate Vabc or Valpha Vbeta based on Vbus and PWM duty

#ifndef _FILE_VOLTAGE_CALCULATOR_H_
#define _FILE_VOLTAGE_CALCULATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// Calculate voltage based on SVPWM generator
typedef struct _tag_voltage_calculator
{
    // input, DC bus voltage, unit p.u.
    ctrl_gt ubus;

    // input, reference SVPWM generate data
    ctl_vector3_t svpwm_uabc;

    // ouptut u alpha, u beta
    ctrl_gt u_alpha;
    ctrl_gt u_beta;

    // output u abc
    ctl_vector3_t uabc;

} ctl_volt_calculate_t;

// calculate u alpha, u beta, uabc by ubus and svpwm_uabc

GMP_STATIC_INLINE
void ctl_step_voltage_calculator(ctl_volt_calculate_t volt_calc)
{
    ctrl_gt temp = ctl_mul(volt_calc.ubus, GMP_CONST_1_OVER_SQRT3);

    /* Scale the incomming Modulation functions with the DC bus voltage value*/
    /* and calculate the 3 Phase voltages */

    volt_calc.uabc.dat[phase_U] =
        ctl_mul(temp, (ctl_mul2(v.svpwm_uabc.dat[phase_U]) - volt_calc.svpwm_uabc.dat[phase_V] -
                       volt_calc.svpwm_uabc.dat[phase_W]));
    volt_calc.uabc.dat[phase_V] =
        ctl_mul(temp, (ctl_mul2(volt_calc.svpwm_uabc.dat[phase_V]) - volt_calc.svpwm_uabc.dat[phase_U] -
                       volt_calc.svpwm_uabc.dat[phase_W]));
    volt_calc.uabc.dat[phase_W] =
        ctl_mul(temp, (ctl_mul2(volt_calc.svpwm_uabc.dat[phase_W]) - volt_calc.svpwm_uabc.dat[phase_V] -
                       volt_calc.svpwm_uabc.dat[phase_U]));

    /* Voltage transformation (a,b,c)  ->  (Alpha,Beta)	*/
    volt_calc.u_alpha = volt_calc.uabc.dat[phase_U];
    volt_calc.u_beta =
        _IQmpy((volt_calc.uabc.dat[phase_U] + ctl_mul2(volt_calc.uabc.dat[phase_V])), GMP_CONST_1_OVER_SQRT3);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_VOLTAGE_CALCULATOR_H_
