/**
 * @file pmsm.ctl.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief PMSM controller source code.
 * @version 0.1
 * @date 2024-09-28
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <core/gmp_core.h>

#include <ctl/ctl_core.h>

#include <ctl/suite/pmsm/pmsm.ctl.h>

/**
 * @brief init a pmsm controller object.
 *
 * @param entity an pointer to the controller objects.
 */
void ctl_init_pmsm_ctl_entity(pmsm_ctl_entity_t *entity)
{
    int i = 0;

    ctl_init_adc_tri_channel(&entity->Vabc);
    ctl_init_adc_tri_channel(&entity->Iabc);

    ctl_init_adc_channel(&entity->Vdcbus);
    ctl_init_adc_channel(&entity->Idcbus);

    ctl_init_pos_encoder(&entity->pos_encoder);
    ctl_init_spd_calculator(&entity->spd_calc);

    //	ctl_init_ramp_gen(&entity->rg);

    entity->enable_output = 0;
    entity->enable_current_controller = 0;
    entity->enable_speed_controller = 0;

    for (i = 0; i < 2; ++i)
    {
        ctl_init_track_pid(&entity->Idq_ctl[i]);
        // ctl_init_divider(&entity->div_current[i]);
        // ctl_init_slope_limit(&entity->traj_current[i]);
        // ctl_init_pid(&entity->pid_current[i]);
    }

    ctl_init_track_pid(&entity->spd_ctl);
    // ctl_init_divider(&entity->div_speed);
    // ctl_init_slope_limit(&entity->traj_speed);
    // ctl_init_pid(&entity->pid_speed);

    for (i = 0; i < 2; ++i)
    {

        entity->Idq_set[i] = 0;

        entity->Idq_set_user[i] = 0;
        entity->Vdq_set_user[i] = 0;
    }
    entity->spd_set = 0;

    ctl_set_phasor_via_angle(0, &entity->phasor);

    ctl_init_svpwm(&entity->svpwm);

    // init other ctrl_gt variables
    entity->angle_fbk_user = 0;
    entity->enable_current_controller = 1;
    entity->enable_speed_controller = 1;
    entity->enable_outer_angle = 0;
    entity->enable_outer_speed = 0;

    entity->spd_set = 0;
    entity->spd_set_user = 0;

    for (i = 0; i < 3; ++i)
    {
        entity->Iab.dat[i] = 0;
        entity->Vab.dat[i] = 0;
        entity->Vab.dat[i] = 0;

        entity->Idq0.dat[i] = 0;
        entity->Vdq0.dat[i] = 0;

        entity->Vab_set.dat[i] = 0;
        entity->Vdq_set.dat[i] = 0;
    }
}

/**
 * @brief Setup a PMSM controller object based on consultant structure.
 *
 * @param entity : a pointer to the PMSM controller object.
 * @param drv : a consultant object of motor driver.
 * @param np : a consultant object of motor name plate.
 */
void ctl_setup_pmsm_ctl_entity(pmsm_ctl_entity_t *entity, ctl_motor_driver_consultant_t *drv,
                               ctl_pmsm_nameplate_consultant_t *np)
{
    int i = 0;

    // setup ADC input stage
    ctl_setup_adc_tri_channel(&entity->Vabc, CTRL_T(drv->adc_voltage_full_scale / np->rated_voltage),
                              CTRL_T(drv->adc_voltage_bias), drv->adc_resolution_bit, drv->adc_resolution_bit);

    ctl_setup_adc_tri_channel(&entity->Iabc, CTRL_T(drv->adc_current_full_scale / np->rated_current),
                              CTRL_T(drv->adc_current_bias), drv->adc_resolution_bit, drv->adc_resolution_bit);

    ctl_setup_adc_channel(&entity->Vdcbus, CTRL_T(drv->adc_dc_voltage_full_scale / np->rated_voltage),
                          CTRL_T(drv->adc_dc_voltage_bias), drv->adc_resolution_bit, drv->adc_resolution_bit);
    ctl_setup_adc_channel(&entity->Idcbus, CTRL_T(drv->adc_dc_current_full_scale / np->rated_current),
                          CTRL_T(drv->adc_dc_current_bias), drv->adc_resolution_bit, drv->adc_resolution_bit);

    // setup encoder
    ctl_setup_pos_encoder(&entity->pos_encoder, np->pole_pairs, drv->position_enc_base);

    // speed calculator
    ctl_setup_spd_calculator(&entity->spd_calc, drv->control_law_freq, drv->speed_calc_div_times, np->rated_speed_rpm,
                             np->pole_pairs, drv->control_law_freq / drv->speed_calc_div_times / 10);

    parameter_gt current_slope = drv->jerk / (drv->control_law_freq / 1000);

    // setup controller without tuning
    for (i = 0; i < 2; ++i)
        ctl_setup_track_pid(&entity->Idq_ctl[i], CTRL_T(1.0), CTRL_T(0.01), CTRL_T(0.0), -CTRL_T(1.0), CTRL_T(1.0),
                            -CTRL_T(current_slope), CTRL_T(current_slope),
                            0 // disable divider
        );

    // void setup_init_track_pid(track_pid_t * tp,
    //	ctrl_gt kp, ctrl_gt ki, ctrl_gt kd, // pid parameters
    //	ctrl_gt sat_min, ctrl_gt sat_max, // saturation limit
    //	ctrl_gt slope_min, ctrl_gt slope_max, // slope limit
    //	uint32_t division //division factor
    //)

    parameter_gt speed_slope =
        drv->acceleration / np->rated_speed_rpm / (drv->control_law_freq / drv->speed_calc_div_times);

    ctl_setup_track_pid(&entity->spd_ctl, CTRL_T(1.0), CTRL_T(0.01), CTRL_T(0.0), -CTRL_T(1.0), CTRL_T(1.0),
                        -CTRL_T(speed_slope), CTRL_T(speed_slope),
                        drv->speed_div_times // divider factor
    );

    // setup output stage
    ctl_setup_svpwm(&entity->svpwm, drv->pwm_half_cycle);
}

/**
 * @brief Tuning PMSM controller based on PMSM parameters
 *
 * @param entity a handle of PMSM controller
 * @param dsn a handle of PMSM designer parameter consultant handle
 * @param drv a handle of motor driver parameter consultant handle
 * @param np a handle of PMSM motor name plate constultant handle
 */
void ctl_tuning_pmsm_pid_via_consultant(pmsm_ctl_entity_t *entity,
                                        // use it to calculate controller parameters
                                        ctl_pmsm_dsn_consultant_t *dsn,
                                        // use it to calculate controller parameters
                                        ctl_motor_driver_consultant_t *drv,
                                        // use it to per unit controller
                                        ctl_pmsm_nameplate_consultant_t *np)
{
    float kp, ki;

    // PID tuning

    kp = np->rated_current / np->rated_voltage * dsn->Ld /
         (1.0 / (2.0 * PI * drv->current_closeloop_bw) + 1.0 / (2.0 * PI * drv->control_law_freq));
    ki = dsn->Rs / dsn->Ld;
    if (ki >= 5.0f)
        ki = 5.0f;
    if (kp >= 10.0f)
        kp = 10.0f;
    ctl_set_pid_parameter(&entity->Idq_ctl[phase_D].pid, CTRL_T(kp), CTRL_T(ki), CTRL_T(0.0));

    kp = np->rated_current / np->rated_voltage * dsn->Lq /
         (1.0 / (2.0 * PI * drv->current_closeloop_bw) + 1.0 / (2.0 * PI * drv->control_law_freq));
    ki = dsn->Rs / dsn->Lq;
    if (ki >= 5.0f)
        ki = 5.0f;
    if (kp >= 10.0f)
        kp = 10.0f;
    ctl_set_pid_parameter(&entity->Idq_ctl[phase_Q].pid, CTRL_T(kp), CTRL_T(ki), CTRL_T(0.0));

    kp = dsn->inertia / ctl_consult_pmsm_Kt(dsn) * (2.0 * PI * drv->speed_closeloop_bw);
    ki = 1 / (2.0 * PI * drv->speed_closeloop_bw);
    if (ki >= 5.0f)
        ki = 5.0f;
    if (kp >= 10.0f)
        kp = 10.0f;
    ctl_set_pid_parameter(&entity->spd_ctl.pid, CTRL_T(kp), CTRL_T(ki), CTRL_T(0.0));
}

/**
 * @brief pmsm controller input stage. This function should be called in input stage.
 *
 * @param entity a handle of PMSM controller.
 * @param adc_current ADC current real value from ADC, phase A, B, C
 * @param adc_voltage ADC voltage real value from ADC, phase A, B, C
 * @param adc_dc_current ADC current real value from ADC, DC bus current.
 * @param adc_dc_voltage ADC voltage real value from ADC, DC bus voltage
 * @param motor_encoder Motor encoder input from user.
 */
void ctl_input_pmsm_ctl(pmsm_ctl_entity_t *entity, uint32_t adc_current[3], uint32_t adc_voltage[3],
                        uint32_t adc_dc_current, uint32_t adc_dc_voltage, uint32_t motor_encoder)
{
    // Input Phase data
    ctl_input_adc_tri_channel_raw_data(&entity->Iabc, adc_current[0], adc_current[1], adc_current[2]);

    ctl_input_adc_tri_channel_raw_data(&entity->Vabc, adc_voltage[0], adc_voltage[1], adc_voltage[2]);

    // Input DC bus data
    ctl_input_adc_source_data(&entity->Idcbus, adc_dc_current);

    ctl_input_adc_source_data(&entity->Vdcbus, adc_dc_voltage);

    // Input Encoder data
    ctl_input_pos_encoder(&entity->pos_encoder, motor_encoder);
}

/**
 * @brief PMSM controller input stage. This function should be called in input stage.
 *  This function may inverse all the input current. If you use 3-shunt current measurement this function isi necessary.
 * @param entity a handle of PMSM controller
 * @param adc_current ADC current real value frome ADC, phase A, B, C. Three phase would be inversed.
 * @param adc_voltage ADC voltage real value from ADC, phase A, B, C
 * @param adc_dc_current ADC current real value from ADC, DC bus current.
 * @param adc_dc_voltage ADC voltage real value from ADC, DC bus voltage
 * @param motor_encoder Motor encoder input from user.
 */
void ctl_input_with_inv_current_pmsm_ctl(pmsm_ctl_entity_t *entity, uint32_t adc_current[3], uint32_t adc_voltage[3],
                                         uint32_t adc_dc_current, uint32_t adc_dc_voltage, uint32_t motor_encoder)
{
    // Input Phase data
    ctl_input_adc_tri_channel_raw_data(&entity->Iabc, (2 << (entity->Iabc.resolution - 1)) - adc_current[0],
                                       (2 << (entity->Iabc.resolution - 1)) - adc_current[1],
                                       (2 << (entity->Iabc.resolution - 1)) - adc_current[2]);

    ctl_input_adc_tri_channel_raw_data(&entity->Vabc, adc_voltage[0], adc_voltage[1], adc_voltage[2]);

    // Input DC bus data
    ctl_input_adc_source_data(&entity->Idcbus, adc_dc_current);

    ctl_input_adc_source_data(&entity->Vdcbus, adc_dc_voltage);

    // Input Encoder data
    ctl_input_pos_encoder(&entity->pos_encoder, motor_encoder);
}

/**
 * @brief This function should be called every main ISR. Or you may call this function in controller framework core stage.
 * 
 * @param entity handle of PMSM controller
 */
void ctl_step_pmsm_ctl_entity(pmsm_ctl_entity_t *entity)
{
    ctrl_gt angle; // rotor angle
    ctrl_gt speed; // rator speed feedback
    //	ctl_vector2_t phasor; // angle phasor
    ctl_vector3_t abc_vector; // Uabc and Iabc

    // Sync Track controller state
    if (entity->enable_current_controller)
    {
        ctl_enable_track_pid(&entity->Idq_ctl[phase_D]);
        ctl_enable_track_pid(&entity->Idq_ctl[phase_Q]);
    }
    else
    {
        ctl_disable_track_pid(&entity->Idq_ctl[phase_D]);
        ctl_disable_track_pid(&entity->Idq_ctl[phase_Q]);
    }

    if (entity->enable_speed_controller)
    {
        ctl_enable_track_pid(&entity->spd_ctl);
    }
    else
    {
        ctl_disable_track_pid(&entity->spd_ctl);
    }

    // deal with adc data
    ctl_step_adc_tri_channel(&entity->Iabc);
    ctl_step_adc_tri_channel(&entity->Vabc);

    ctl_step_adc_channel(&entity->Idcbus);
    ctl_step_adc_channel(&entity->Vdcbus);

    ctl_get_elec_angle_via_pos_encoder(&entity->pos_encoder);

    // get angle information
    // Select Angle source
    if (entity->enable_outer_angle)
    {
        // DANGEROUS:
        // THIS SWITCH SHOULD ONLY BE ONPENED MANUALLY BY PROFESSIONAL DEBUGGING EXPERT.
        // OR FORCE ANGLE MODE MAY DEMAGE YOUR MOTOR CONTROLLER.
        angle = entity->angle_fbk_user;
    }
    else
    {
        angle = entity->pos_encoder.elec_angle;
    }

    // Get speed feedback
    if (!entity->enable_outer_speed)
    {
        // ctl_step_spd_calc(&entity->spd_calc, angle);
        ctl_step_spd_calc(&entity->spd_calc, entity->pos_encoder.elec_angle);
    }
    speed = ctl_get_pmsm_ctl_spd(entity);

    // get phasor
    ctl_set_phasor_via_angle(angle, &entity->phasor);

    // Current Clark Transform
    ctl_get_adc_tri_channel_via_vector3(&entity->Iabc, &abc_vector);
    ctl_ct_clark(&abc_vector, &entity->Iab);
    // Current Park Transform
    ctl_ct_park(&entity->Iab, &entity->phasor, &entity->Idq0);

    // Voltage Clark Transform
    //	ctl_get_adc_tri_channel_via_vector3(&entity->Vabc, &abc_vector);
    //	ctl_ct_clark(&abc_vector, &entity->Vab);
    // Voltage Park Transform
    //	ctl_ct_park(&entity->Vab, &entity->phasor, &entity->Vdq0);

    // Sync User Target
    entity->spd_set = entity->spd_set_user;

    // Call Speed Controller
    ctl_step_track_pid(&entity->spd_ctl, entity->spd_set, speed);

    if (entity->enable_speed_controller)
    {
        entity->Idq_set[phase_Q] = ctl_get_track_pid_output(&entity->spd_ctl);
    }
    else
    {
        entity->Idq_set[phase_D] = entity->Idq_set_user[phase_D];
        entity->Idq_set[phase_Q] = entity->Idq_set_user[phase_Q];
    }

    // Call current Controller
    ctl_step_track_pid(&entity->Idq_ctl[phase_D], entity->Idq_set[phase_D], entity->Idq0.dat[phase_D]);
    ctl_step_track_pid(&entity->Idq_ctl[phase_Q], entity->Idq_set[phase_Q], entity->Idq0.dat[phase_Q]);

    if (entity->enable_current_controller)
    {
        entity->Vdq_set.dat[phase_D] = ctl_get_track_pid_output(&entity->Idq_ctl[phase_D]);
        entity->Vdq_set.dat[phase_Q] = ctl_get_track_pid_output(&entity->Idq_ctl[phase_Q]);
        entity->Vdq_set.dat[2] = 0;
    }
    else
    {
        entity->Vdq_set.dat[phase_D] = entity->Vdq_set_user[phase_D];
        entity->Vdq_set.dat[phase_Q] = entity->Vdq_set_user[phase_Q];
        entity->Vdq_set.dat[2] = 0;
    }

    // ipark
    ctl_ct_ipark(&entity->Vdq_set, &entity->phasor, &entity->Vab_set);

    // svpwm generator
    ctl_set_svpwm_via_ab(&entity->svpwm, &entity->Vab_set);
    ctl_svpwm_calc(&entity->svpwm);
    ctl_svpwm_modulation(&entity->svpwm);
}

/**
 * @brief Set PMSM controller work as open-loop mode.
 * 
 * @param entity handle of PMSM controller.
 */
void ctl_set_pmsm_ctl_entity_as_openloop(pmsm_ctl_entity_t *entity)
{
    entity->enable_outer_angle = 1;
    entity->enable_outer_speed = 0;
    entity->enable_output = 1;
    entity->enable_current_controller = 0;
    entity->enable_speed_controller = 0;
}

/**
 * @brief Set PMSM controller work as current-loop mode.
 * 
 * @param entity handle of PMSM controller
 */
void ctl_set_pmsm_ctl_entity_as_currentloop(pmsm_ctl_entity_t *entity)
{
    entity->enable_outer_angle = 1;
    entity->enable_outer_speed = 0;
    entity->enable_output = 1;
    entity->enable_current_controller = 1;
    entity->enable_speed_controller = 0;
}

/**
 * @brief Set PMSM controller work as speed-loop mode
 * 
 * @param entity handle of PMSM controller
 */
void ctl_set_pmsm_ctl_entity_as_speedloop(pmsm_ctl_entity_t *entity)
{
    entity->enable_outer_angle = 0;
    entity->enable_outer_speed = 0;
    entity->enable_output = 1;
    entity->enable_current_controller = 1;
    entity->enable_speed_controller = 1;
}

/**
 * @brief Set PMSM controller Voltage target
 * 
 * @param entity handle of PMSM controller
 * @param Vd Vd target, unit p.u.
 * @param Vq Vq target, unit p.u.
 */
void ctl_set_pmsm_ctl_Vdq(pmsm_ctl_entity_t *entity, ctrl_gt Vd, ctrl_gt Vq)
{
    entity->Vdq_set_user[phase_D] = Vd;
    entity->Vdq_set_user[phase_Q] = Vq;
}

/**
 * @brief Set PMSM controller Currnet target
 * 
 * @param entity handle of PMSM controller
 * @param Id Id target, unit p.u.
 * @param Iq Iq target, unit p.u.
 */
void ctl_set_pmsm_ctl_Idq(pmsm_ctl_entity_t *entity, ctrl_gt Id, ctrl_gt Iq)
{
    entity->Idq_set_user[phase_D] = Id;
    entity->Idq_set_user[phase_Q] = Iq;
}

/**
 * @brief Set PMSM controller Speed target
 * 
 * @param entity handle of PMSM controller
 * @param spd_pu speed target, unit p.u.
 */
void ctl_set_pmsm_ctl_spd(pmsm_ctl_entity_t *entity, ctrl_gt spd_pu)
{
    entity->spd_set_user = spd_pu;
}

/**
 * @brief Get PMSM controller Speed real value.
 * 
 * @param entity handle of PMSM controller.
 * @return ctrl_gt return speed real value, unit p.u.
 */
ctrl_gt ctl_get_pmsm_ctl_spd(pmsm_ctl_entity_t *entity)
{
    ctrl_gt speed;

    if (entity->enable_outer_speed)
    {
        speed = entity->speed_fbk_user;
    }
    else
    {
        speed = entity->spd_calc.speed;
    }

    return speed;
}
