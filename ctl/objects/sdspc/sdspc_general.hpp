// single digital switching power controller

#include <assert.h>
#include <math.h>

#include <core/std/typedef.h>

#include <ctrl/ctrl_config.h>

#include <ctrl/component/common/pid.hpp>
#include <ctrl/component/common/divider.hpp>
#include <ctrl/component/common/trajectory.hpp>

#include <core/dev/pwm_device.h>

#include <core/workflow/workflow.hpp>

#ifndef _FILE_SDSPC_GENERAL_HPP_
#define _FILE_SDSPC_GENERAL_HPP_


//////////////////////////////////////////////////////////////////////////
// general type of SDSPC intrinsic

class sdspc_intrinsic_plate_gt
{
public:
	gmp_ctrl_param_t pwm_freq_kHz;		// PWM frequency, (kHz)
	gmp_ctrl_param_t ctrl_freq_kHz;		// controller frequency, (kHz)

	gmp_ctrl_param_t Pn_kW;				// rated power, SI (kW)
	gmp_ctrl_param_t Un_V;				// rated voltage, SI (V)
	gmp_ctrl_param_t In_A;				// rated current, SI (A)
};

class sdspc_intrinsic_buck_gt
	: public sdspc_intrinsic_plate_gt
{
public:
	// buck circuit intrinsic variables
	gmp_ctrl_param_t R_load;			// SI (Ohm)
	gmp_ctrl_param_t L;					// inductance, SI (mH, Henry)
	gmp_ctrl_param_t RL_esr;			// equivalent series resistance for inductance, SI (Ohm)
	gmp_ctrl_param_t C;					// capacitor, SI (uF)
	gmp_ctrl_param_t RC_esr;			// equivalent series resistance for capacitor, SI (Ohm)
	gmp_ctrl_param_t VD_on;				// forward threshold voltage, SI (V), 
	//  you may ignore this value, if directly set it to 0
};

class sdspc_adc_atleast_t
{
public:
	sdspc_adc_atleast_t()
		:Vin(0), Vout(0), Iin(0), Iout(0),
		Vin_sf(1), Vout_sf(1), Iin_sf(1), Iout_sf(1),
		Vin_bias(0), Vout_bias(0), Iin_bias(0), Iout_bias(0)
	{

	}

public:
	// ADC input variables, after p.u.
	gmp_math_t Vin;
	gmp_math_t Vout;
	gmp_math_t Iin;
	gmp_math_t Iout;

	// auxiliary scale factor
	// The following four factors would be calculated by intrinsic variables
	gmp_math_t Vin_sf;
	gmp_math_t Vout_sf;
	gmp_math_t Iin_sf;
	gmp_math_t Iout_sf;

	// The following four factors would be calculated by intrinsic variables of sampling circuit
	gmp_math_t Vin_bias;
	gmp_math_t Vout_bias;
	gmp_math_t Iin_bias;
	gmp_math_t Iout_bias;

public:
	// sampling
	void sampling()
	{
		// Call ADC to get the sampling result
		Vin -= Vin_bias;
		Vout -= Vout_bias;
		Iin -= Iin_bias;
		Iout -= Iout_bias;

		// auto scale factor
		Vin *= Vin_sf;
		Vout *= Vout_sf;
		Iin *= Iin_sf;
		Iout *= Iout_sf;
	}

};


//////////////////////////////////////////////////////////////////////////
// interface type
class sdspc_interface_gt
{
public:
	sdspc_interface_gt()
		:duty(0), current(0), voltage(0)
	{}

public:
	// output PWM duty, p.u.
	gmp_math_t duty;

	// output current, p.u.
	gmp_math_t current;

	// output voltage, p.u.
	gmp_math_t voltage;
};

//////////////////////////////////////////////////////////////////////////
// SDSPC monitor type of motor
class sdspc_monitor_gt
{
public:
	// ctor & dtor
	sdspc_monitor_gt()
		: ctrl_temp_centideg(0)
	{}

public:
	int16_t ctrl_temp_centideg;			// converter temperature sensor, unit 0.01 Celsius degree

public:
	// utilities
};

//////////////////////////////////////////////////////////////////////////
// controller pure value
class sdspc_gt_base
{
public:
	sdspc_gt_base()
		: close_loop_level(POWER_DISABLE), flag_run(0)
	{}

public:
	// controller type
	typedef enum //_tag_gmp_sdspc_closeloop_t
	{
		// Controller is disable
		POWER_DISABLE = 0,

		// Controller will wfr_running in open-sloop mode
		OPENLOOP = 16,

		// Controller will wfr_running in current-loop mode
		CURRENTLOOP = 64,

		// Controller will wfr_running in voltage-loop mode
		VOLTAGELOOP = 128

	}gmp_sdspc_closeloop_t;


public:
	// output variables
//	gmp_math_t duty;

public:
	// pointer to PWM output device
	pwm_dev_t* pwm;			// a pointer of PWM device
	uint32_t pwm_chn;		// a number of PWM channel index

public:
	// close loop level
	gmp_sdspc_closeloop_t close_loop_level;

	// controller wfr_running switch
	uint8_t flag_run;

public:
	// bind specify PWM channel to the class
	gmp_stat_t bind(pwm_dev_t* pwm, uint32_t pwm_chn)
	{
		if (flag_run != 0)
		{
			return GMP_STAT_MODULE_LAUNCHED;
		}

		this->pwm = pwm;
		this->pwm_chn = pwm_chn;

		if (pwm == NULL)
		{
			return GMP_STAT_ZERO_PTR;
		}

		// Disable PWM output
		disable_pwm(this->pwm->handle);

		// clear PWM cmp reg
		gmp_write_pwm_cmp(this->pwm->handle, this->pwm_chn, 0);


		return GMP_STAT_OK;
	}

	// change close loop level
	void set_close_level(gmp_sdspc_closeloop_t closeloop)
	{
		close_loop_level = closeloop;
	}

	// safely set flag_run flag
	gmp_stat_t set_flag_run_s()
	{
		// check parameters

		// refresh intermediate quantity

		// set flag
		flag_run = 1;
		return GMP_STAT_OK;
	}

	// reset flag_run flag
	inline void reset_flag_run()
	{
		flag_run = 0;
	}
};

//////////////////////////////////////////////////////////////////////////
// controller 
template<
	typename adc_type,
	typename intrinsic_type,
	typename cur_ctrl_type,
	typename vol_ctrl_type,
	//	typename pwm_output_type,
	typename monitor_type
>
class sdspc_gt
	: public sdspc_gt_base
{
public:
	//input variables
	adc_type adc;

private:
	// intermediate variables
	sdspc_interface_gt target;

public:
	// user interface
	sdspc_interface_gt user_if;
	// lower limit
	sdspc_interface_gt user_ll;

public:
	// intrinsic variables
	intrinsic_type intrinsic;

public:
	// controllers
	ctrl_divider_t cur_div;
	cur_ctrl_type cur_ctl;
	traj_ramp_t cur_traj;

	ctrl_divider_t vol_div;
	vol_ctrl_type vol_ctl;
	traj_ramp_t vol_traj;

public:
	// monitor type
	monitor_type monitor;

public:
	// ctor & dtor
	sdspc_gt()
	{

	}

public:
	// controller main process
	// export general single digital switching power controller
	inline void gmp_sdspc_ctrl_g()
	{
		// validate parameters

		if (flag_run)
		{
			// assert, ensure the pointer is surely a meaningful parameter
			assert(pwm);

			// ADC sampling
			adc.sampling();

			// voltage controller stage
			if (close_loop_level >= VOLTAGELOOP
				&& vol_div.is_tick())
				// invoke voltage controller
			{
				// assign target value
				target.voltage = user_if.voltage;
				vol_ctl.set_min_limit(user_ll.current);
				vol_ctl.set_max_limit(user_if.current);

				// voltage controller
				vol_ctl.exec(vol_traj.get_next_position(target.voltage),
					adc.Vout);

				// assign inner loop target
				target.current = vol_ctl.out;
			}
			else
			{
				// assign inner loop target
				target.current = user_if.current;
			}

			// current controller
			if (close_loop_level >= CURRENTLOOP
				&& cur_div.is_tick())
				// invoke current controller
			{
				// assign target value
				cur_ctl.set_min_limit(user_ll.duty);
				cur_ctl.set_max_limit(user_if.duty);

				// current controller 
				cur_ctl.exec(cur_traj.get_next_position(target.current),
					adc.Iout);

				// assign output value
				target.duty = cur_ctl.out;
			}
			else
			{
				// assign output value
				target.duty = user_if.duty;
			}

			// duty output
			if (close_loop_level >= OPENLOOP)
				// invoke duty output stage
			{
				// deadtime compensate: coming soon


				// write PWM duty
				gmp_write_pwm_cmp(pwm->handle, pwm_chn,
					GMP_CALC_PWM_CMP_FUNC(pwm, target.duty));

				// enable PWM output
				enable_pwm(pwm->handle);
			}
			else
			{
				// Disable PWM output
				disable_pwm(pwm->handle);

				// Clear PWM duty
				target.duty = 0;
			}


		} // flag_run
	} // function gmp_sdspc_ctrl_g

public:
	// utilities
	void clear()
	{
		cur_div.clear();
		cur_traj.clear();
		cur_ctl.clear();

		vol_div.clear();
		vol_traj.clear();
		vol_ctl.clear();
	}

};

// export sdspc controller 
typedef sdspc_gt<sdspc_adc_atleast_t,
	sdspc_intrinsic_buck_gt,
	var_pid_reg_t,
	var_pid_reg_t,
	sdspc_monitor_gt>
	sdspc_buck_ctrl_t;

// Standard SDSPC work-flow
// four sections standard workflow for controller template library.
//template<typename controller_type>
class quadri_sect_ctl_wf_pack
{
public:
	// ctor & dtor
	quadri_sect_ctl_wf_pack()
	{
		wf.usr_param = this;

		sections[WF_QSC_SEC_DISABLED].transfer = [](gmp_workflow_t* wf)-> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			if (wf->get_last_node_id() == WF_QSC_SEC_FAULT)
			{
				pack->trans_reset();
			}
			else if (wf->get_last_node_id() == WF_QSC_SEC_READY)
			{
				pack->trans_release();
			}
			else if (wf->get_last_node_id() == WF_QSC_SEC_DISABLED)
			{
				pack->trans_init();
			}
			else
			{
				// Incorrect running steps
				// We should transfer the next function to 
				return &pack->sections[WF_QSC_SEC_FAULT];
			}
			return nullptr;

		};

		sections[WF_QSC_SEC_DISABLED].routine = [](gmp_workflow_t* wf) {
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);
			return pack->wfr_disabled();
		};

		sections[WF_QSC_SEC_READY].transfer = [](gmp_workflow_t* wf)-> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);
			if (wf->get_last_node_id() == WF_QSC_SEC_DISABLED)
			{
				pack->trans_setup();
			}
			else if (wf->get_last_node_id() == WF_QSC_SEC_POWERON)
			{
				pack->trans_poweroff();
			}
			else
			{
				// Incorrect running steps
				return &pack->sections[WF_QSC_SEC_FAULT];
			}
			return nullptr;

		};

		sections[WF_QSC_SEC_READY].routine = [](gmp_workflow_t* wf) {
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);
			return pack->wfr_ready();
		};

		sections[WF_QSC_SEC_POWERON].transfer = [](gmp_workflow_t* wf)-> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			if (wf->get_last_node_id() == WF_QSC_SEC_READY)
			{
				pack->trans_poweron();
			}
			else
			{
				// Incorrect running steps
				return &pack->sections[WF_QSC_SEC_FAULT];

			}
			return nullptr;

		};

		sections[WF_QSC_SEC_POWERON].routine = [](gmp_workflow_t* wf) -> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			return pack->wfr_poweron();
		};

		sections[WF_QSC_SEC_QUICKSTOP].transfer = [](gmp_workflow_t* wf) -> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			if (wf->get_last_node_id() == WF_QSC_SEC_POWERON)
			{
				pack->trans_quickstop();
			}
			else
			{
				// Incorrect running steps
				return &pack->sections[WF_QSC_SEC_FAULT];
			}
			return nullptr;

		};

		sections[WF_QSC_SEC_QUICKSTOP].routine = [](gmp_workflow_t* wf) -> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			return pack->wfr_quick_stop();
		};

		sections[WF_QSC_SEC_FAULT].transfer = [](gmp_workflow_t* wf)-> gmp_wf_node_base_gt*
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			pack->trans_oops();
			return nullptr;

		};

		sections[WF_QSC_SEC_FAULT].routine = [](gmp_workflow_t* wf)
		{
			quadri_sect_ctl_wf_pack* pack = reinterpret_cast<quadri_sect_ctl_wf_pack*>(wf->usr_param);

			return pack->wfr_fault();
		};

	}

public:
	// typedef

	// sections enum type
	typedef enum
	{
		WF_QSC_SEC_DISABLED = 0,
		WF_QSC_SEC_READY,
		WF_QSC_SEC_POWERON,
		WF_QSC_SEC_QUICKSTOP,
		WF_QSC_SEC_FAULT,
		WF_QSC_SEC_INVALID
	}section_enum_t;

	// section trans command type
	// QSC: quadri-section controller
	typedef enum
	{
		WF_QSC_CMD_OOPS = 0,
		WF_QSC_CMD_INIT,
		WF_QSC_CMD_SETUP,
		WF_QSC_CMD_POWERON,
		WF_QSC_CMD_POWEROFF,
		WF_QSC_CMD_QUICKSTOP,
		WF_QSC_CMD_RELEASE,
		WF_QSC_CMD_RESET,
		WF_QSC_CMD_INVALID
	}cmd_enum_t;


public:
	// utilities functions

	// trans_init the work-flow data structure
	void wf_init(sdspc_buck_ctrl_t* sdspc_obj);

	// command the work-flow to a specified state
	gmp_stat_t command(cmd_enum_t cmd);

public:
	// Basic 5 sections structure

	// The device is wfr_disabled, you should enable first.
	gmp_wf_node_base_gt* wfr_disabled();

	// The device has trans_setup, and wfr_ready to power on.
	gmp_wf_node_base_gt* wfr_ready();

	// The device is wfr_running, the controller is active
	gmp_wf_node_base_gt* wfr_poweron();

	// The device is doing stop quickly routine.
	gmp_wf_node_base_gt* wfr_quick_stop();

	// The device wfr_fault has occurred.
	// the device will stuck at this position, 
	// until reset command is invoked.
	gmp_wf_node_base_gt* wfr_fault();

	// Basic 5 sections transfer function

	// initialize the device, make the device disable
	void trans_init();

	// trans_setup the device, make the device is power-off and wfr_ready to power-on.
	void trans_setup();

	// release the controller
	void trans_release();

	// launch the device, make the device controller is poweron.
	void trans_poweron();

	// power off the device,  
	void trans_poweroff();

	// require quick stop routine
	void trans_quickstop();

	// The device meets error, and will clear here.
	void trans_oops();

	// Reset the device
	void trans_reset();





public:
	// work-flow is a member
	gmp_workflow_t wf;

	// reinforcement friendship
	friend class gmp_workflow_t;

private:
	// All the section is saved
	gmp_wf_node_gt sections[5];

private:
	cmd_enum_t cmd;

public:
	// Contain a sdspc_object
	sdspc_buck_ctrl_t* sdspc_obj;
	//	controller_type* ctrl_obj;
};



//////////////////////////////////////////////////////////////////////////
// Parameter Self-Adaptive

// Buck model
gmp_stat_t current_loop_pid_adaptive(sdspc_intrinsic_buck_gt* param, pid_reg_t* cur_pid);


#endif

