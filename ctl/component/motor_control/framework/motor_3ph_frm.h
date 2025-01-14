
#ifndef _FILE_MOTOR_3PH_FRM_H_
#define _FILE_MOTOR_3PH_FRM_H_

// This is the standard framework for 3 - phases motor

typedef struct _tag_mtr_ctrl_framework
{
    // general framework
    ctl_object_nano_t framework;

    // input analog port
    adc_tri_channel_t vabc;
    adc_tri_channel_t iabc;
    adc_channel_t vdcbus;
    adc_channel_t idcbus;

    // encoder port
    ctl_rotation_encif_t *pos;
    ctl_speed_encif_t *spd;

    // output port
    pwm_tri_channel_t tabc;

    // Clark trans result
    // alpha-beta axis value
    ctl_vector2_t iab;
    ctl_vector2_t vab;

    // Park trans result
    ctl_vector2_t idq;
    ctl_vector2_t vdq;

    // modulation

}ctl_mtr_ctrl_fm_t;


GMP_STATIC_INLINE 
void ctl_input_mtr_ctrl(ctl_mtr_ctrl_fm_t* fm,
    adc_gt va, adc_gt vb, adc_gt vc,
    adc_gt ia, adc_gt ib, adc_gt ic,
    adc_gt vdc, adc_gt idc)
{

}


#endif // _FILE_MOTOR_3PH_FRM_H_

