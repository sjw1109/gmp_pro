// This is an example of peripheral.c

// GMP basic core header
#include <gmp_core.h>
#include <ctl/ctl.config.h>

// user main header
#include "user_main.h"

#include <ctl/component/interface/adc_ptr_channel.h>

#include <ctl/component/motor_control/basic/std_sil_motor_interface.h>

// SIL standard port for Motor control

// buffer for rx & tx
extern gmp_pc_simulink_rx_buffer_t simulink_rx_buffer;
extern gmp_pc_simulink_tx_buffer_t simulink_tx_buffer;


tri_ptr_adc_channel_t uabc;
tri_ptr_adc_channel_t iabc;

ptr_adc_channel_t udc;
ptr_adc_channel_t idc;

//ctl_pos_encoder_t pos_enc;



////////////////////////////////////////////////////////////////////////////
//// Devices on the peripheral

// User should setup all the peripheral in this function.
// This function has been completed by syscfg
void setup_peripheral(void)
{
    //ctl_init_ptr_adc_channel(idc, simulink_rx_buffer.)
}



//////////////////////////////////////////////////////////////////////////
// interrupt functions and callback functions here
//
