
#ifndef _FILE_CTL_INTERFACE_H_
#define _FILE_CTL_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // device related functions

#ifdef SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

    void ctl_fmif_input_stage_routine(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_output_stage_routine(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_request_stage_routine(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_output_enable(ctl_object_nano_t *pctl_obj)
    {
    }

    void ctl_fmif_output_disable(ctl_object_nano_t *pctl_obj)
    {
    }

    fast_gt ctl_fmif_security_routine(ctl_object_nano_t *pctl_obj)
    {
        // not implement
        return GMP_EC_OK;
    }

#endif // SPECIFY_ENABLE_CTL_FRAMEWORK_NANO

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_INTERFACE_H_
