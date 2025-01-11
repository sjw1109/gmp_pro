// Dead-beat Prediction Current Control general framework;
// 

// necessary headers
// #include "gmp_core.h"
// #include "ctl/ctl_core.h"

#include "ctl/component/intrinsic/interface/adc_channel.h"
#include "ctl/component/intrinsic/interface/pwm_channel.h"
#include "ctl/component/motor_control/basic/encoder.h"

// before DPC_current control the I,V,speed,theta etc.should be loaded

typedef struct _tag_DPC_current_ctrl
{
    // input paramter
    // sensors channels

    // current eletric status, should be loaded before control
    ctl_vector3_t Iabc_k;
    ctrl_gt Vdc;
    ctrl_gt Idc;
    ctl_vector2_t Idq_set;

    // control variables
    ctl_vector2_t Iab_k;  
    ctl_vector2_t Idq_k;
    ctl_vector2_t Vdq_k;

    // current mechanical status, should be loaded before control
    ctrl_gt speed;
    ctrl_gt theta;
    ctl_vector2_t phasor;

    // for prediction
    ctrl_gt control_law_freq;
    ctrl_gt control_law_period;
    // predict result
    ctl_vector2_t Vdq_pre;
    ctl_vector2_t Idq_pre;

    // motor parameter
    ctrl_gt Motor_Ld;
    ctrl_gt Motor_Lq;
    ctrl_gt Motor_Rs;
    ctrl_gt Motor_PMFlux;

    // output
    ctl_vector2_t Vdq_set;
    
    //control the compensation mode:
    // 0 for default : no Compensation;
    // 1 for : one step Current Prediction Compensation
    // 2 for : robust Optimisation
    fast_gt flag_DPC_Compensation_mode;

}ctl_DPC_current_ctrl_t;


ec_gt ctl_init_DPC_current_ctrl(ctl_DPC_current_ctrl_t *obj);
ec_gt ctl_setup_DPC_current_ctrl(ctl_DPC_current_ctrl_t *obj, \
                                    ctl_pmsm_dsn_consultant_t *dsn, \
                                    ctl_motor_driver_consultant_t *drv);

GMP_STATIC_INLINE
void ctl_input_DPC_current_ctrl(ctl_DPC_current_ctrl_t *obj, \
                                    ctl_vector2_t *idq_ref, \
                                    adc_tri_channel_t *adc_current, \
                                    adc_channel_t *adc_dc_current, \
                                    adc_channel_t *adc_dc_voltage, \
                                    ctl_pos_encoder_t *motor_encoder, \
                                    ctl_spd_calculator_t *spd_calc)
{
    ctl_get_adc_tri_channel_via_vector3(adc_current, &(obj->Iabc_k));
    obj->Idc = adc_dc_current->value;
    obj->Vdc = adc_dc_voltage->value;
    obj->theta = motor_encoder->encif.elec_position;
    obj->speed = spd_calc->encif.speed;
    ctl_set_phasor_via_angle(obj->theta, &(obj->phasor));
    obj->Idq_set.dat[phase_d] = idq_ref->dat[phase_d];
    obj->Idq_set.dat[phase_q] = idq_ref->dat[phase_q];
}

// step call-back funciton.
// The function below should be called once every current control period
GMP_STATIC_INLINE
void ctl_step_DPC_current_ctrl(ctl_DPC_current_ctrl_t *obj)
{
    // temp value
    ctl_vector2_t edq, edq_pre;

    // current clark, park
    // voltage is recorded every control period, no need to measure
    ctl_ct_clark(obj->Iabc_k, obj->Iab_k);
    ctl_ct_park(obj->Iab_k, &(obj->phasor), &(obj->Idq_k))
    
    // temp edq cal
    edq.dat[phase_d] = ctl_mul(ctl_mul(obj->Motor_Lq, obj->Idq_k.dat[phase_q]), \
                                obj->speed);
    edq.dat[phase_q] = ctl_mul((ctl_mul(obj->Motor_Ld, obj->Idq_k.dat[phase_d]) + obj->Motor_PMFlux), \
                                obj->speed);
    // current prediction
    switch(obj->flag_DPC_Compensation_mode)
    {
        case 0:
            obj->Idq_pre.dat[phase_d] = obj->Idq_k.dat[phase_d];
            obj->Idq_pre.dat[phase_q] = obj->Idq_k.dat[phase_q];
            break;
        case 1:
            obj->Idq_pre.dat[phase_d] = obj->Idq_k.dat[phase_d] + \
                                        ctl_mul((ctl_div(obj->control_law_period, obj->Motor_Ld)), \
                                                (obj->Vdq_k.dat[phase_d]\
                                                    - ctl_mul(obj->Motor_Rs,obj->Idq_k[phase_d])\
                                                    + edq.dat[phase_d]));
            obj->Idq_pre.dat[phase_q] = obj->Idq_k.dat[phase_q] + \
                                        ctl_mul((ctl_div(obj->control_law_period, obj->Motor_Lq)), \
                                                (obj->Vdq_k.dat[phase_q]\
                                                    - ctl_mul(obj->Motor_Rs,obj->Idq_k[phase_q])\
                                                    - edq.dat[phase_q]));
            break;
        // case 2:
        default:
            obj->Idq_pre.dat[phase_d] = obj->Idq_set.dat[phase_d];
            obj->Idq_pre.dat[phase_q] = obj->Idq_set.dat[phase_q];
            break;
    }
    edq_pre.dat[phase_d] = ctl_mul(ctl_mul(obj->Motor_Lq, obj->Idq_pre.dat[phase_q]), \
                                    obj->speed);
    edq_pre.dat[phase_q] = ctl_mul((ctl_mul(obj->Motor_Ld, obj->Idq_pre.dat[phase_d]) + obj->Motor_PMFlux), \
                                    obj->speed);
    
    // voltage prediction
    obj->Vdq_pre.dat[phase_d] = ctl_mul(ctl_mul(obj->Motor_Ld, obj->control_law_freq), \
                                        (obj->Idq_set.dat[phase_d] - obj->Idq_pre.dat[phase_d])) \
                                + ctl_mul(obj->Motor_Rs, obj->Idq_pre.dat[phase_d]) \
                                - edq_pre.dat[phase_d];
    obj->Vdq_pre.dat[phase_q] = ctl_mul(ctl_mul(obj->Motor_Lq, obj->control_law_freq), \
                                        (obj->Idq_set.dat[phase_q] - obj->Idq_pre.dat[phase_q])) \
                                + ctl_mul(obj->Motor_Rs, obj->Idq_pre.dat[phase_q]) \
                                + edq_pre.dat[phase_q];
    
    // output voltage post-process

    // set output voltage
    obj->Vdq_set.dat[phase_d] = obj->Vdq_pre.dat[phase_d];
    obj->Vdq_set.dat[phase_q] = obj->Vdq_pre.dat[phase_q];

    // prepare for next period, 
    obj->Vdq_k.dat[phase_d] = obj->Vdq_set.dat[phase_d];
    obj->Vdq_k.dat[phase_q] = obj->Vdq_set.dat[phase_q];
}

