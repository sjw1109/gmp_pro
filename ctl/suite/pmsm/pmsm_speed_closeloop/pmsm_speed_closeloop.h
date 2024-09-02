// This is an example of PMSM controller via Controller Nano framework

#include <ctl/framework/ctl_nano.h>
#include <ctl/suite/pmsm/pmsm.ctl.h>

#ifndef _FILE_PMSM_SPEED_CLOSELOOP_H_
#define _FILE_PMSM_SPEED_CLOSELOOP_H_

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


//////////////////////////////////////////////////////////////////////////
// The following functions should implemented by user
// copy from <ctl_nano.h>

// deal with all the adc and other input messages
void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj);

// implement all the controller routine
void ctl_core_stage_routine(ctl_object_nano_t* pctl_obj);

// output all the PWM and other digital or analog signal
void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj);

// request other information via peripheral, for instance SPI.
void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj);

// enable PWM output
void controller_output_enable(ctl_object_nano_t* pctl_obj);

// disable PWM output
void controller_output_disable(ctl_object_nano_t* pctl_obj);

// controller monitor routine
void controller_monitor_routine(ctl_object_nano_t* pctl_obj);


#endif // _FILE_PMSM_SPEED_CLOSELOOP_H_
