
#include <ctl/component/common/track_pid.h>



#ifndef _PMSM_CONFIG_H_
#define _PMSM_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////
// Here's the default PMSM motor parameters
// These parameters are given by macros
#define ADC_CURRENT_GAIN 20
#define ADC_CURRENT_BIAS 10
#define ADC_VOLTAGE_GAIN 20
#define ADC_VOLTAGE_BIAS 10
#define ADC_RESOLUTION 16

#define PWM_MODULATION_HALF_CYCLE ((10000))


//////////////////////////////////////////////////////////////////////////
// Here's the PMSM controller template

	enum ABC_ASIX_ENUM
	{
		phase_U = 0,
		phase_V = 1,
		phase_W = 2
	};

	enum DQ_ASIC_ENUM
	{
		phase_D = 0,
		phase_Q = 1
	};

	// PMSM controller entity
	typedef struct _tag_pmsm_ctl_entity
	{
		// input ADC channels
		adc_tri_channel_t Uabc;
		adc_tri_channel_t Iabc;
		adc_channel_t Udcbus;
		adc_channel_t Idcbus;

		// encoder input channels
		ctl_pos_encoder_t pos_encoder;
		ctl_spd_encoder_t spd_encoder;

		ctl_spd_calculator_t spd_calc;

		// ramp generator, setup ramp signal generator
		ctl_src_rg_t rg;

		// controller switch
		fast_gt enable_output; // Enable PWM output
		fast_gt enable_current_controller;
		fast_gt enable_speed_controller;

		// current controller
		track_pid_t idq_ctl[2];
		//divider_t div_current[2];
		//slope_lim_t traj_current[2];
		//ctl_pid_t pid_current[2];

		// speed controller
		track_pid_t spd_ctl;
		//divider_t div_speed;
		//slope_lim_t traj_speed;
		//ctl_pid_t pid_speed;

		// Controller targets
		ctrl_gt Vdq_set[2];
		ctrl_gt Idq_set[2];
		ctrl_gt spd_set;

		// Controller feedback
		// This variables means the current angle for Park and iPark transform
		ctl_vector2_t phasor;

		ctl_vector3_t Vab; // alpha-beta-0
		ctl_vector3_t Iab; // alpha-beta-0

		ctrl_gt Vdq[2];
		ctrl_gt Idq[2];
		ctrl_gt spd;

		// output PWM channels
		svpwm_channel_t svpwm;

	}pmsm_ctl_entity_t;

	void init_pmsm_ctl_entity(pmsm_ctl_entity_t *entity)
	{
		int i = 0;

		ctl_init_adc_tri_channel(&entity->Uabc);
		ctl_init_adc_tri_channel(&entity->Iabc);

		ctl_init_adc_channel(&entity->Udcbus);
		ctl_init_adc_channel(&entity->Idcbus);

		ctl_init_pos_encoder(&entity->pos_encoder);
		ctl_init_spd_encoder(&entity->spd_encoder);

		ctl_init_spd_calculator(&entity->spd_calc);

		init_ramp_gen(&entity->rg);

		entity->enable_output = 0;
		entity->enable_current_controller = 0;
		entity->enable_speed_controller = 0;

		for (i = 0; i < 2; ++i) {
			ctl_init_track_pid(&entity->idq_ctl[i]);
			//ctl_init_divider(&entity->div_current[i]);
			//ctl_init_slope_limit(&entity->traj_current[i]);
			//ctl_init_pid(&entity->pid_current[i]);
		}

		ctl_init_track_pid(&entity->spd_ctl);
		//ctl_init_divider(&entity->div_speed);
		//ctl_init_slope_limit(&entity->traj_speed);
		//ctl_init_pid(&entity->pid_speed);
		
		for (i = 0; i < 2; ++i) {
			entity->Vdq_set[i] = 0;
			entity->Idq_set[i] = 0;
		}
		entity->spd_set = 0;

		ctl_set_phasor_via_angle(0, &entity->phasor);

		ctl_init_svpwm(&entity->svpwm);

	}

	void setup_pmsm_ctl_entity(pmsm_ctl_entity_t* entity)
	{
		int i = 0;

		for (i = 0; i < 3; ++i) {
			setup_adc_channel(&entity->Uabc[i],
				ADC_VOLTAGE_GAIN, ADC_VOLTAGE_BIAS,
				ADC_RESOLUTION, ADC_RESOLUTION);
			setup_adc_channel(&entity->Iabc[i],
				ADC_CURRENT_GAIN, ADC_CURRENT_BIAS,
				ADC_RESOLUTION, ADC_RESOLUTION);
		}

		setup_adc_channel(&entity->Udcbus, 
			ADC_VOLTAGE_GAIN, ADC_VOLTAGE_BIAS,
			ADC_RESOLUTION, ADC_RESOLUTION);
		setup_adc_channel(&entity->Idcbus,
			ADC_CURRENT_GAIN, ADC_CURRENT_BIAS,
			ADC_RESOLUTION, ADC_RESOLUTION);

		ctl_setup_pos_encoder(&entity->pos_encoder);
		ctl_setup_spd_encoder(&entity->spd_encoder);

		for (i = 0; i < 2; ++i)
			ctl_setup_track_pid(&entity->idq_ctl[i],
				CTRL_T(1.0), CTRL_T(0.01), CTRL_T(0.0),
				-CTRL_T(1.0), CTRL_T(1.0),
				-CTRL_T(1.0), CTRL_T(1.0),
				0); // disable divider

		ctl_setup_track_pid(&entity->spd_ctl,
			CTRL_T(1.0), CTRL_T(0.01), CTRL_T(0.0),
			-CTRL_T(1.0), CTRL_T(1.0),
			-CTRL_T(1.0), CTRL_T(1.0),
			0); // disable divider

		ctl_setup_svpwm(&entity->svpwm, PWM_MODULATION_HALF_CYCLE);

	}

	void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
	{

	}
	
	// PMSM controller top module
	typedef struct _tag_pmsm_ctl_object
	{
		// Controller Framework
		ctl_object_nano_t base;


		// controller entity
		pmsm_ctl_entity_t ctrl;

		// Monitor Objects

		// Protect Objects

		// ADC Calibrate Objects
		adc_bias_calibrator_t calibrator;
		uint32_t calibrae_progress;

	}pmsm_ctl_object_t;




#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _PMSM_CONFIG_H_

