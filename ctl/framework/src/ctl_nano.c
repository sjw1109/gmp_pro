
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>

#include <ctl/framework/ctl_nano.h>


//////////////////////////////////////////////////////////////////////////
// WEAK FUNCTION DEFINITION

// This is the main file of GMP CTL (controller template library) module,
// The functions provided in this file may be invoked in your controller Main ISR or Main loop.
// User should follow the instruction of the function manual.

// ....................................................................//
// The following functions may running in Main ISR

GMP_WEAK_FUNC_PREFIX
void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

GMP_WEAK_FUNC_PREFIX
void ctl_core_stage_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}


GMP_WEAK_FUNC_PREFIX
void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

GMP_WEAK_FUNC_PREFIX
void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

// ....................................................................//
// The following functions may running in Main Loop

GMP_WEAK_FUNC_PREFIX
void controller_monitor_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

GMP_WEAK_FUNC_PREFIX
void controller_security_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

// return value:
// 1 change to next progress
// 0 keep the same state
GMP_WEAK_FUNC_PREFIX
fast_gt ctl_nano_sm_pending_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
	return 0;
}

// return value:
// 1 change to next progress
// 0 keep the same state
GMP_WEAK_FUNC_PREFIX
fast_gt ctl_nano_sm_calibrate_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
	return 1;
}

GMP_WEAK_FUNC_PREFIX
fast_gt ctl_nano_sm_ready_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
	return 0;
}

// Main relay close, power on the main circuit
GMP_WEAK_FUNC_PREFIX
fast_gt ctl_nano_sm_runup_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
	return 1;
}

GMP_WEAK_FUNC_PREFIX
void ctl_nano_sm_online_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

GMP_WEAK_FUNC_PREFIX
void ctl_nano_sm_fault_routine(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

// ....................................................................//
// The following functions may called in Main ISR and Main Loop

GMP_WEAK_FUNC_PREFIX
void controller_output_enable(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}

GMP_WEAK_FUNC_PREFIX
void controller_output_disable(ctl_object_nano_t* pctl_obj)
GMP_WEAK_FUNC_SUFFIX
{
	// not implement
}



//////////////////////////////////////////////////////////////////////////
// Kernal functions


// Controller core
// This function should be called in your controller Main ISR.
// This function may invoke all the other functions related to the controller.
void controller_dispatch(ctl_object_nano_t* pctl_obj)
{
	// Step 0: ISR tick update
	pctl_obj->isr_tick = pctl_obj->isr_tick + 1;

#ifndef GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK
	if (pctl_obj->security_endorse == GMP_CTL_ENDORSE)
	{
#endif // GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK

		// Step I: input stage
		// All the analog input and digital input and peripheral input should be processed here.
		// + Analog ADC input
		// + digital GPIO input
		// + peripheral input, such as, encoder, and etc.
		ctl_input_stage_routine(pctl_obj);

		if (pctl_obj->state_machine >= CTL_SM_CALIBRATE)
		{

			// Step II: controller core stage
			// All the control law should be implemented here.
			ctl_core_stage_routine(pctl_obj);

			// Step III: output stage
			// All the output routine should be implemented here.
			// + PWM output
			// + analog output
			// + GPIO output
			// + peripheral request, such as, request encoder routine.
			ctl_output_stage_routine(pctl_obj);
		}

#ifndef GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK
	}
#endif // GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK

	// Step IV: request stage
	// All the real time request stage.
	ctl_request_stage_routine(pctl_obj);

}


// Controller state machine
// This function should be called in your controller main loop routine.
// This function should implement all the controller state machine transfer.
void controller_state_dispatch(ctl_object_nano_t* pctl_obj)
{
	// All the real-time controller state machine should implement here.
	pctl_obj->mainloop_tick = pctl_obj->mainloop_tick + 1;

	// Security ensure
	if (pctl_obj->switch_security_routine &&
		pctl_obj->state_machine > CTL_SM_CALIBRATE)
		controller_security_routine(pctl_obj);

	// State machine
	switch (pctl_obj->state_machine)
	{
		// Wait user to enable the controller.
	case CTL_SM_PENDING:
		// close output
		controller_output_disable(pctl_obj);

		// call pending State machine routine
		if (ctl_nano_sm_pending_routine(pctl_obj))
		{
			// need to change to the next state
			pctl_obj->state_machine = CTL_SM_CALIBRATE;
		}
		break;

		// The following 3 states, user should define a state machine 
		// to determine the output is enable or disable 
	case CTL_SM_CALIBRATE:
		// call State machine routine
		if (pctl_obj->switch_calibrate_stage) // enable the calibrate stage
		{
			if (ctl_nano_sm_calibrate_routine(pctl_obj))
			{
				// need to change to the next state
				pctl_obj->state_machine = CTL_SM_READY;
			}
		}
		else
		{
			// need to change to the next state
			pctl_obj->state_machine = CTL_SM_READY;
		}

		break;
	case CTL_SM_READY:
		// call State machine routine
		if (ctl_nano_sm_ready_routine(pctl_obj))
		{
			pctl_obj->state_machine = CTL_SM_RUNUP;
		}

		break;
	case CTL_SM_RUNUP:
		// call State machine routine
		if (ctl_nano_sm_runup_routine(pctl_obj))
		{
			pctl_obj->state_machine = CTL_SM_ONLINE;
		}

		break;

		// The following state mast ensure output is enable,
		// And every thing is ready.
	case CTL_SM_ONLINE:
		// call State machine routine
		ctl_nano_sm_online_routine(pctl_obj);

		controller_output_enable(pctl_obj);
		break;

		// Fault -> Close Output right now.
	case CTL_SM_FAULT:
		controller_output_disable(pctl_obj);

		// call State machine routine
		ctl_nano_sm_fault_routine(pctl_obj);
		break;

		// Error State Machine -> close PWM.
		// and transfer to Fault state.
	default:
		// Error State Machine occurred:
		controller_output_disable(pctl_obj);
		pctl_obj->state_machine = CTL_SM_FAULT;
		break;
	}

	// Monitor module caller
	if (ctl_step_divider(&pctl_obj->div_monitor))
	{
		controller_monitor_routine(pctl_obj);
	}
}


// Security Check routine
// This function should be called in your controller initialization routine.
// This function may check all the controller parameters.
uint32_t controller_inspection(ctl_object_nano_t* pctl_obj)
{
	return 0;
}





//////////////////////////////////////////////////////////////////////////
// Other functions

// init controller object
// User should call this function in init process 
// BEFORE all the other control object is inited. 
void ctl_init_nano_header(ctl_object_nano_t* ctl_obj)
{
	ctl_obj->isr_tick = 0;

	ctl_obj->state_machine = CTL_SM_PENDING;
	//	ctl_obj->state_machine = CTL_SM_READY;
	ctl_obj->switch_calibrate_stage = 1;
	ctl_obj->switch_runup_stage = 0;
	ctl_obj->switch_security_routine = 1;

	// finally set the endorsement
	ctl_obj->security_endorse = GMP_CTL_ENDORSE;

	ctl_obj->control_law_CPU_usage_tick = 0;
	ctl_obj->mainloop_CPU_usage_tick = 0;

	// Monitor divider
	ctl_init_divider(&ctl_obj->div_monitor);
	ctl_setup_divider(&ctl_obj->div_monitor, 20);

	return;
}

void ctl_setup_nano_header(ctl_object_nano_t* ctl_obj,
	uint32_t ctrl_freq    // the frequency of the control law, unit Hz
)
{
	ctl_obj->ctrl_freq = ctrl_freq;

	return;
}

void ctl_sm_nano_trnasfer(ctl_object_nano_t* ctl_obj,
	ctl_nano_state_machine target_state
)
{

}

void ctl_force_nona_header_online(ctl_object_nano_t* ctl_obj)
{
	ctl_obj->state_machine = CTL_SM_ONLINE;
}
