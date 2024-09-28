
// necessary components
#include <ctl/component/common/gmp_math.h>
#include <ctl/component/interface_util/adc_channel.h>
#include <ctl/component/interface_util/pwm_channel.h>

#include <ctl/component/common/pid.h>
#include <ctl/component/common/stimulate.h>

#include <ctl/component/common/track_pid.h>

// Motor
#include <ctl/component/motor/coord_trans.h>
#include <ctl/component/motor/encoder.h>
#include <ctl/component/motor/svpwm.h>

// additionally
#include <ctl/component/motor/motor_driver_consultant.h>
#include <ctl/component/motor/pmsm_consultant.h>

#ifndef _FILE_PMSM_CTL_H_
#define _FILE_PMSM_CTL_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// Here's the PMSM controller template

// PMSM controller entity
typedef struct _tag_pmsm_ctl_entity {
  // .............................................. //
  // [input] ADC channels
  adc_tri_channel_t Vabc;
  adc_tri_channel_t Iabc;
  adc_channel_t Vdcbus;
  adc_channel_t Idcbus;

  // .............................................. //
  // [input] encoder input channels
  ctl_pos_encoder_t pos_encoder;

  ctl_spd_calculator_t spd_calc;

  // .............................................. //
  // [input] controller switch

  // Enable user angle input
  fast_gt enable_outer_angle;

  // Enable user speed input
  fast_gt enable_outer_speed;

  // Enable PWM output
  fast_gt enable_output;

  // Enable current controller
  fast_gt enable_current_controller;

  // Enable speed controller
  fast_gt enable_speed_controller;

  // .............................................. //
  // [input] controller user target
  ctrl_gt Vdq_set_user[2];
  ctrl_gt Idq_set_user[2];
  ctrl_gt spd_set_user;

  // User force input angle
  ctrl_gt angle_fbk_user; // rad

  // User force input speed
  ctrl_gt speed_fbk_user; // p.u.

  // .............................................. //
  // Controller segment
  // current controller
  track_pid_t Idq_ctl[2];
  // divider_t div_current[2];
  // slope_lim_t traj_current[2];
  // ctl_pid_t pid_current[2];

  // speed controller
  track_pid_t spd_ctl;
  // divider_t div_speed;
  // slope_lim_t traj_speed;
  // ctl_pid_t pid_speed;

  // .............................................. //
  // Controller targets
  // User should not modify these variables
  ctl_vector3_t Vab_set;
  ctl_vector3_t Vdq_set;
  ctrl_gt Idq_set[2];
  ctrl_gt spd_set;

  // .............................................. //
  // Controller feedback
  // User should not modify these variables
  // This variables means the current angle for Park and iPark transform
  ctl_vector2_t phasor;

  ctl_vector3_t Vab; // alpha-beta-0
  ctl_vector3_t Iab; // alpha-beta-0

  ctl_vector3_t Vdq0;
  ctl_vector3_t Idq0;
  ctrl_gt spd;

  // .............................................. //
  // output PWM channels
  svpwm_channel_t svpwm;

} pmsm_ctl_entity_t;

void ctl_init_pmsm_ctl_entity(pmsm_ctl_entity_t *entity);

void ctl_setup_pmsm_ctl_entity(pmsm_ctl_entity_t *entity,
                               ctl_motor_driver_consultant_t *drv,
                               ctl_pmsm_nameplate_consultant_t *np);

// Tuning PMSM controller based on Motor Design parameters
void ctl_tuning_pmsm_pid_via_consultant(
    pmsm_ctl_entity_t *entity,
    // use it to calculate controller parameters
    ctl_pmsm_dsn_consultant_t *dsn,
    // use it to calculate controller parameters
    ctl_motor_driver_consultant_t *drv,
    // use it to per unit controller
    ctl_pmsm_nameplate_consultant_t *np);

// This function should be invoked in Input Stage
void ctl_input_pmsm_ctl(pmsm_ctl_entity_t *entity, uint32_t adc_current[3],
                        uint32_t adc_voltage[3], uint32_t adc_dc_current,
                        uint32_t adc_dc_voltage, uint32_t motor_encoder);

// This function should be an alternative, for bridge resistance current sample
// case.
void ctl_input_with_inv_current_pmsm_ctl(
    pmsm_ctl_entity_t *entity, uint32_t adc_current[3], uint32_t adc_voltage[3],
    uint32_t adc_dc_current, uint32_t adc_dc_voltage, uint32_t motor_encoder);

// output routine for enable port
// This function should be invoked in Output Stage
GMP_STATIC_INLINE
fast_gt ctl_get_pmsm_ctl_output_enable(pmsm_ctl_entity_t *entity) {
  return entity->enable_output;
}

// output routine for PWM
// This function should be invoked in Output Stage
GMP_STATIC_INLINE
void ctl_get_pmsm_ctl_output_pwm_cmp_all(pmsm_ctl_entity_t *entity,
                                         pwm_gt *phase_u, pwm_gt *phase_v,
                                         pwm_gt *phase_w) {
  *phase_u = entity->svpwm.pwm_cmp[0];
  *phase_v = entity->svpwm.pwm_cmp[1];
  *phase_w = entity->svpwm.pwm_cmp[2];
}

GMP_STATIC_INLINE
void ctl_set_pmsm_force_angle(pmsm_ctl_entity_t *entity, ctrl_gt angle) {
  entity->angle_fbk_user = angle;
}

GMP_STATIC_INLINE
void ctl_set_pmsm_force_angle_pu(pmsm_ctl_entity_t *entity, ctrl_gt angle_pu) {
  entity->angle_fbk_user = ctrl_mpy(GMP_CONST_2_PI, angle_pu);
}

void ctl_set_pmsm_ctl_entity_as_openloop(pmsm_ctl_entity_t *entity);

void ctl_set_pmsm_ctl_entity_as_currentloop(pmsm_ctl_entity_t *entity);

void ctl_set_pmsm_ctl_entity_as_speedloop(pmsm_ctl_entity_t *entity);

// output routine for PWM
// This function should be invoked in Output Stage
GMP_STATIC_INLINE
pwm_gt ctl_pmsm_ctl_get_outout_cmp(pmsm_ctl_entity_t *entity, fast_gt index) {
  return entity->svpwm.pwm_cmp[index];
}

// Step Stage
// This function should be invoked in Step Stage
void ctl_step_pmsm_ctl_entity(pmsm_ctl_entity_t *entity);

// User Config set Vdq
void ctl_set_pmsm_ctl_Vdq(pmsm_ctl_entity_t *entity, ctrl_gt Vd, ctrl_gt Vq);

// User Config set Idq
void ctl_set_pmsm_ctl_Idq(pmsm_ctl_entity_t *entity, ctrl_gt Id, ctrl_gt Iq);

ctrl_gt ctl_get_pmsm_ctl_spd(pmsm_ctl_entity_t *entity);

void ctl_set_pmsm_ctl_spd(pmsm_ctl_entity_t *entity, ctrl_gt spd_pu);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PMSM_CTL_H_
