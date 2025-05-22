
#ifndef _FILE_CTL_DISPATCH_H_
#define _FILE_CTL_DISPATCH_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

// If only one interrupt will complete the control task,
// This function should be called by
GMP_STATIC_INLINE
void ctl_fm_periodic_dispatch(ctl_object_nano_t *pctl_obj)
{
    // Step 0: ISR tick update
    pctl_obj->isr_tick = pctl_obj->isr_tick + 1;

#ifndef GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK
    if (pctl_obj->security_endorse == GMP_CTL_ENDORSE)
    {
#endif // GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK

#ifndef GMP_CTL_FRM_NANO_INPUT_STANDALONE
        // Step I: input stage
        // All the analog input and digital input and peripheral input should be processed here.
        // + Analog ADC input
        // + digital GPIO input
        // + peripheral input, such as, encoder, and etc.
        ctl_fmif_input_stage_routine(pctl_obj);
#endif // GMP_CTL_FRM_NANO_INPUT_STANDALONE

#ifndef GMP_CTL_FRM_NANO_CORE_STANDALONE
        // Step II: controller core stage
        // All the control law should be implemented here.
        ctl_fmif_core_stage_routine(pctl_obj);
#endif // GMP_CTL_FRM_NANO_CORE_STANDALONE

#ifndef GMP_CTL_FRM_NANO_OUTPUT_STANDALONE
        // Step III: output stage
        // All the output routine should be implemented here.
        // + PWM output
        // + analog output
        // + GPIO output
        // + peripheral request, such as, request encoder routine.
        ctl_fmif_output_stage_routine(pctl_obj);
#endif // GMP_CTL_FRM_NANO_OUTPUT_STANDALONE

#ifndef GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK
    }
#endif // GMP_DISABLE_CTL_OBJ_ENDORSE_CHECK

#ifndef GMP_CTL_FRM_NANO_REQUEST_STANDALONE
    // Step IV: request stage
    // All the real time request stage.
    ctl_fmif_request_stage_routine(pctl_obj);
#endif // GMP_CTL_FRM_NANO_REQUEST_STANDALONE
}

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

//
// This function is kernel of GMP CTL module.
// This function should be invoked by user in Main ISR
//
GMP_STATIC_INLINE
void gmp_base_ctl_step(void)
{
#ifdef SPECIFY_ENABLE_GMP_CTL
#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifndef SPECIFY_CALL_PERIODIC_DISPATCH_MANUALLY

    // Call periodic dispatch
    ctl_fm_periodic_dispatch(ctl_nano_handle);

#endif // SPECIFY_CALL_PERIODIC_DISPATCH_MANUALLY
    ctl_dispatch();
#else
    // input stage
    ctl_input_callback();

    // call user controller user defined ISR
    ctl_dispatch();

    // output stage
    ctl_output_callback();

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO
#endif // SPECIFY_ENABLE_GMP_CTL
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_DISPATCH_H_
