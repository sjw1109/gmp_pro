/**
 * @file pmsm_speed_closeloop.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief his is an example of PMSM controller via Controller Nano framework
 * @version 0.1
 * @date 2024-09-28
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/framework/ctl_nano.h>
#include <ctl/suite/pmsm/pmsm.ctl.h>

#ifndef _FILE_PMSM_SPEED_CLOSELOOP_H_
#define _FILE_PMSM_SPEED_CLOSELOOP_H_

#if defined __cplusplus
extern "C"
{
#endif // __cplusplus

    /**
     * @brief PMSM controller top module
     *
     */
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

    } pmsm_ctl_object_t;

    //////////////////////////////////////////////////////////////////////////
    // The following functions should implemented by user
    // copy from <ctl_nano.h>

    /**
     * @brief deal with all the ADC, Motor Encoder and other input messages
     *
     * @param pctl_obj controller objects
     */
    void ctl_input_stage_routine(ctl_object_nano_t *pctl_obj);

    /**
     * @brief implement all the controller routine
     *
     * @param pctl_obj controller objects
     */
    void ctl_core_stage_routine(ctl_object_nano_t *pctl_obj);

    /**
     * @brief output all the PWM and other digital or analog signal
     *
     * @param pctl_obj controller objects
     */
    void ctl_output_stage_routine(ctl_object_nano_t *pctl_obj);

    // request other information via peripheral, for instance SPI.
    /**
     * @brief output all the PWM and other digital or analog signal
     *
     * @param pctl_obj controller objects
     */
    void ctl_request_stage_routine(ctl_object_nano_t *pctl_obj);

    /**
     * @brief enable PWM output
     *
     * @param pctl_obj controller objects
     */
    void controller_output_enable(ctl_object_nano_t *pctl_obj);

    /**
     * @brief disable PWM output
     *
     * @param pctl_obj controller objects
     */
    void controller_output_disable(ctl_object_nano_t *pctl_obj);

    /**
     * @brief controller monitor routine
     *
     * @param pctl_obj controller objects
     */
    void controller_monitor_routine(ctl_object_nano_t *pctl_obj);

#if defined __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PMSM_SPEED_CLOSELOOP_H_
