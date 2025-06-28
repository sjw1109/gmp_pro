
// This file implement a universal GMP SIL simulation standard interface

#include <ctl/component/interface/interface_base.h>

#ifndef _FILE_GMP_SIL_STD_INTERFACE_H_
#define _FILE_GMP_SIL_STD_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_sil_std_if
{
    // enable
    double enable;

    // 24 PWM channel
    uint32_t pwm_channel[24];

    // 24 channel Data output
    double dout[24];

    // Communicate port
    double monitor_port[24];

} ctl_sil_std_ift;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_GMP_SIL_STD_INTERFACE_H_s
