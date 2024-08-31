#include <core/gmp_core.hpp>
#include <ctl/ctl_core.h>
#include <ctl/ctrl_plant/dspc/buck_boost_2Ch/buck_boost_2Ch_ctl.h>

#include <ctl/component/motor/coord_trans.h>
#include <ctl/component/common/signal_generator.h>
#include <ctl/component/motor/svpwm.h>

#include <math.h>

enum MOTOR_PHASE
{
	PHASE_U = 0,
	PHASE_V = 1,
	PHASE_W = 2
};

// Model Interface: transmit package
// Controller Interface: transmit package
#pragma pack(1)
typedef struct _tag_transmit_package_t
{
	double scope[4];
	uint32_t duty[3];

	uint32_t enable;
}transmit_package_t;

// Model Interface: receive package
// Controller Interface: transmit package
#pragma pack(1)
typedef struct _tag_receive_package_t
{

	double time;

	uint32_t adc_current[3];
	uint32_t adc_voltage[3];

	uint32_t motor_encoder;

}receive_package_t;




transmit_package_t tx_pak;
receive_package_t rx_pak;


#define ADC_BIT_RESOLUTION ((16))
#define CURRENT_BASE       ((10.0))
#define CURRENT_ADC_FULL_SCALE ((20.0)) // ADC Reference Current
#define CURRENT_ADC_BIAS ((5.0))        // ADC bias current
#define CURRENT_ADC_GAIN ((0.5))		// ADC current gain

#define ADC_CURRENT_GAIN   ((CURRENT_ADC_FULL_SCALE/CURRENT_BASE/CURRENT_ADC_GAIN))
#define ADC_CURRENT_BIAS   ((CURRENT_ADC_BIAS / CURRENT_ADC_FULL_SCALE))


#define VOLTAGE_BASE       (())


#define ADC_VOLTAGE_GAIN (20)
#define ADC_VOLTAGE_BIAS (10)

adc_channel_t current_adc[3];
adc_channel_t voltage_adc[3];


ctl_src_rg_t rg;

ctl_vector2_t phasor;
ctl_vector3_t current_abc;
ctl_vector3_t voltage_abc;

ctl_vector3_t voltage_ab;
ctl_vector3_t voltage_dq_out;
svpwm_channel_t svpwm;


// CTL initialize routine
void ctl_init()
{
	for (int i = 0; i < 4; ++i)
		tx_pak.scope[i] = 0;

	for (int i = 0; i < 3; ++i)
	{
		tx_pak.duty[i] = 0;

		init_adc_channel(&current_adc[i]);
		init_adc_channel(&voltage_adc[i]);

		setup_adc_channel(&current_adc[i], CTRL_T(ADC_CURRENT_GAIN), CTRL_T(ADC_CURRENT_BIAS),
			ADC_BIT_RESOLUTION, ADC_BIT_RESOLUTION); // 16 BIT resolution & right align

		setup_adc_channel(&voltage_adc[i], CTRL_T(ADC_VOLTAGE_GAIN), CTRL_T(ADC_VOLTAGE_BIAS),
			ADC_BIT_RESOLUTION, ADC_BIT_RESOLUTION); // 16 BIT resolution & right align
	}

	voltage_dq_out.dat[0] = CTRL_T(0.1);
	voltage_dq_out.dat[1] = CTRL_T(0.35);
	voltage_dq_out.dat[2] = 0;

	init_ramp_gen(&rg);
	setup_ramp_gen_with_amp_freq(&rg, 10e3, 20, 2 * PI, 0);


	init_svpwm(&svpwm);
	setup_svpwm(&svpwm, 10000);

#ifdef USING_SIMULINK_UDP_SIMULATE

	udp_svr_obj.recv_buf = &rx_pak;
	udp_svr_obj.send_buf = &tx_pak;
	udp_svr_obj.recv_buf_len = sizeof(rx_pak);
	udp_svr_obj.send_buf_len = sizeof(tx_pak);
#endif // USING_SIMULINK_UDP_SIMULATE
}



// CTL loop routine
void ctl_dispatch(void)
{
	tx_pak.enable = 1;
	// User Controller logic here.

	ramp_step(&rg);

	angle2phasor(rg.current, &phasor);

	//phasor.dat[0] = ctrl_sin(rg.current);
	//phasor.dat[1] = ctrl_cos(rg.current);

	



	dq2ab(&voltage_dq_out, &phasor, &voltage_ab);


	attach_svpwm_ab(&svpwm, &voltage_ab);


	tx_pak.scope[0] = voltage_ab.dat[0];
	tx_pak.scope[1] = voltage_ab.dat[1];
	tx_pak.scope[2] = voltage_ab.dat[2];
	
	svpwm_gen_calc2(&svpwm);
	//svpwm_gen_calc_time(&svpwm);

	tx_pak.scope[0] = svpwm.T[0];
	tx_pak.scope[1] = svpwm.T[1];
	tx_pak.scope[2] = svpwm.T[2];
	tx_pak.scope[3] = rg.current;

	svpwm_gen_calc_cmp(&svpwm);

	tx_pak.enable = 1;

	for (int i = 0; i < 3; ++i)
		tx_pak.duty[i] = svpwm.pwm_cmp[i];



}

//void ctl_input_stage_routine(ctl_object_nano_t* pctl_obj)
//{
//
//}
//
//void ctl_output_stage_routine(ctl_object_nano_t* pctl_obj)
//{
//
//}
//
//
//void ctl_request_stage_routine(ctl_object_nano_t* pctl_obj)
//{
//	// Noting to do
//}
