

// This file implement a universal motor controller interface

#include <ctl/component/interface/interface_base.h>

#ifndef _FILE_MOTOR_UNIVERSAL_INTERFACE_H_
#define _FILE_MOTOR_UNIVERSAL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// Motor position encoder interface
//

typedef struct _tag_rotation_encoder_t
{
    // All the rotation encoder should start with the following 3 items

    // output: mechanical position of the encoder, unit, p.u.
    ctrl_gt position;

    // output: electrical position output, unit p.u.
    ctrl_gt elec_position;

    // output: mechanical revolutions of the motor
    int32_t revolutions;

} rotation_ift;

// Get encoder position
GMP_STATIC_INLINE
ctrl_gt ctl_get_encoder_position(rotation_ift *enc)
{
    return enc->position;
}

// Get encoder position of electric
GMP_STATIC_INLINE
ctrl_gt ctl_get_encoder_elec_postion(rotation_ift *enc)
{
    return enc->elec_position;
}

//////////////////////////////////////////////////////////////////////////
// Motor speed encoder interface
//

// speed encoder
typedef struct _tag_speed_encoder_t
{
    // All the speed encoder should start with the following 1 item(s).

    // output: mechanical speed output, unit p.u.
    ctrl_gt speed;

} velocity_ift;

// Get encoder speed
GMP_STATIC_INLINE
ctrl_gt ctl_get_encoder_speed(velocity_ift *enc)
{
    return enc->speed;
}

//////////////////////////////////////////////////////////////////////////
// Torque encoder interface

// torque measurement
typedef struct _tag_torque_sensor_interface_t
{
    // All torque encoder should start with the following 1 item(s).

    // output mechanical torque output, unit p.u.
    ctrl_gt torque;
} torque_ift;

// Type transfer

// position encoder
#define CTL_POSITION_IF(X) ((rotation_ift *)X)

// velocity encoder
#define CTL_SPEED_IF(X) ((velocity_ift *)X)

//////////////////////////////////////////////////////////////////////////4
// universal single motor controller interface type

// This struct only contain all the motor input sensor.
typedef struct _tag_universal_mtr_if_t
{
    // voltage sensor
    tri_adc_ift *uabc;

    // current sensor
    tri_adc_ift *iabc;

    // DC bus voltage sensor
    adc_ift *udc;

    // DC bus current sensor
    adc_ift *idc;

    // position encoder
    rotation_ift *position;

    // speed encoder
    velocity_ift *velocity;

    // torque sensor
    torque_ift *torque;

    //// PWM output / GPIO output
    //// for modulation output the three variables is duty of three channel.
    //// for directly output the three variables is switch state of three channel.
    // tri_pwm_ift *uabc_target;

} mtr_ift;

// controller should communicate with motor controller via the following function.
// interface function

// current access function
GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_current_single(mtr_ift *mtr, uint32_t phase)
{
    gmp_base_assert(mtr->iabc);

    gmp_base_assert(phase < 4);

    return mtr->iabc->value.dat[phase];
}

GMP_STATIC_INLINE
vector3_gt *ctl_get_mtr_current(mtr_ift *mtr)
{
    gmp_base_assert(mtr->iabc);

    return &mtr->iabc->value;
}

// voltage access function
GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_voltage_single(mtr_ift *mtr, uint32_t phase)
{
    gmp_base_assert(mtr->iabc);
    gmp_base_assert(phase < 4);

    return mtr->uabc->value.dat[phase];
}

GMP_STATIC_INLINE
vector3_gt *ctl_get_mtr_voltage(mtr_ift *mtr)
{
    gmp_base_assert(mtr->uabc);

    return &mtr->uabc->value;
}

// dc bus access function
GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_dc_voltage(mtr_ift *mtr)
{
    gmp_base_assert(mtr->udc);

    return mtr->udc->value;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_dc_current(mtr_ift *mtr)
{
    gmp_base_assert(mtr->idc);

    return mtr->idc->value;
}

// position access
GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_elec_theta(mtr_ift *mtr)
{
    gmp_base_assert(mtr->position);

    return mtr->position->elec_position;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_theta(mtr_ift *mtr)
{
    gmp_base_assert(mtr->position);

    return mtr->position->position;
}

GMP_STATIC_INLINE
int32_t ctl_get_mtr_revolution(mtr_ift *mtr)
{
    gmp_base_assert(mtr->position);

    return mtr->position->revolutions;
}

// speed access
GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_velocity(mtr_ift *mtr)
{
    gmp_base_assert(mtr->velocity);

    return mtr->velocity->speed;
}

// torque access
GMP_STATIC_INLINE
ctrl_gt ctl_get_mtr_torque(mtr_ift *mtr)
{
    gmp_base_assert(mtr->torque);

    return mtr->torque->torque;
}

//// pwm output access
//// when the controller is working in PWM mode, three parameter is (-0.5, 0.5) means duty cycle of output PWM.
//// when the controller is working in GPIO mode, three parameter has special meanings
////  + 0 close whole output,
////  + 1 open the upper bridge
////  + -1 open the lower bridge
// void ctl_set_mtr_out_duty(mtr_ift* mtr, ctrl_gt phase_u, ctrl_gt phase_v, ctrl_gt phase_w)
//{
//     gmp_base_assert(mtr->uabc_target);

//    mtr->uabc_target->value.dat[phase_U] = phase_u;
//    mtr->uabc_target->value.dat[phase_V] = phase_v;
//    mtr->uabc_target->value.dat[phase_W] = phase_w;
//}

// User should call the following function to attach interface to motor universal interface

GMP_STATIC_INLINE
void ctl_attach_mtr_current(mtr_ift *mtr, tri_adc_ift *iabc)
{
    mtr->iabc = iabc;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_voltage(mtr_ift *mtr, tri_adc_ift *uabc)
{
    mtr->uabc = uabc;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_dc_current(mtr_ift *mtr, adc_ift *idc)
{
    mtr->idc = idc;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_dc_voltage(mtr_ift *mtr, adc_ift *udc)
{
    mtr->udc = udc;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_adc_channels(mtr_ift *mtr, tri_adc_ift *iabc, tri_adc_ift *uabc, adc_ift *idc, adc_ift *udc)
{
    mtr->iabc = iabc;
    mtr->uabc = uabc;
    mtr->idc = idc;
    mtr->udc = udc;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_position(mtr_ift *mtr, rotation_ift *pos)
{
    mtr->position = pos;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_velocity(mtr_ift *mtr, velocity_ift *vel)
{
    mtr->velocity = vel;
}

GMP_STATIC_INLINE
void ctl_attach_mtr_torque(mtr_ift *mtr, torque_ift *torque)
{
    mtr->torque = torque;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_MOTOR_UNIVERSAL_INTERFACE_H_
