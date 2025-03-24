

// This file implement a universal GMP standard interface

#include <ctl/component/interface/interface_base.h>

#ifndef _FILE_GMP_STANDARD_INTERFACE_H_
#define _FILE_GMP_STANDARD_INTERFACE_H_

typedef struct _tag_std_ctrl_tx_port
{
    // enable
    double enable;

    // 24 PWM channel
    uint32_t pwm_channel[24];

    // 24 DAC channel
    uint32_t dac_channel[24];

    // Communicate port
    double monitor_port[24];
};

typedef struct _tag_std_ctrl_rx_port
{
    // time
    double time;

    // 24 channel ADC result
    uint32_t adc_channel[24];

    // communicate port
    double sensor_port[24];
};


#endif // _FILE_GMP_STANDARD_INTERFACE_H_
