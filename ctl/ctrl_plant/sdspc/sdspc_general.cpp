#include <ctrl/ctrl_plant/sdspc/sdspc_general.hpp>
// 
// template<
// 	typename adc_type,
// 	typename intrinsic_type,
// 	typename cur_ctrl_type,
// 	typename vol_ctrl_type,
// 	typename monitor_type
// >
// void sdspc_gt::clear()
// 



//tex:
// Buck transfer function: 
// $$G_{vd}(s)=V_g\frac{1+s/\omega_{zc}}{1+s/Q\omega_0+s^2/\omega_0^2}$$
// parameters in the equation:
// $$\omega_0 = \frac{1}{\sqrt{LC}},Q=\frac{1}{\omega_0\left(L/R+(R_L+R_C)C\right)} $$
// $$\omega_{zL}=\frac{R_L}{L},\omega_{zC}=\frac{1}{R_CC},\omega_{zp}=\frac{1}{RC}$$	 


gmp_stat_t current_loop_pid_adaptive(sdspc_intrinsic_buck_gt* param, pid_reg_t* cur_pid)
{
	//tex: $$\omega_0,k\;rad/s$$
	gmp_param_t omega_0_kHz = (gmp_param_t)1e6 / sqrt(param->C * param->L);
	gmp_param_t Q_omega_0_inv = param->L / param->R_load + (param->RL_esr + param->RC_esr) * param->C;

	return GMP_STAT_OK;
}

//////////////////////////////////////////////////////////////////////////
// Standard SDSPC workflow

void quadri_sect_ctl_wf_pack::wf_init(sdspc_buck_ctrl_t* sdspc_obj)
{
	// assign sdspc object
	this->sdspc_obj = sdspc_obj;

	// trans_init workflow object

}

gmp_wf_node_base_gt* quadri_sect_ctl_wf_pack::wfr_disabled()
{
	// set intrinsic parameters
	sdspc_obj->intrinsic.pwm_freq_kHz = 10.0;
	sdspc_obj->intrinsic.ctrl_freq_kHz = 10.0;
	sdspc_obj->intrinsic.Pn_kW = 0.05;
	sdspc_obj->intrinsic.Un_V = 24;
	sdspc_obj->intrinsic.In_A = 10;

	// set controller parameters
	sdspc_obj->cur_ctl.set_kp(1.0);
	sdspc_obj->cur_ctl.set_ki(0.01);
	sdspc_obj->cur_ctl.set_kd(0);

	sdspc_obj->cur_div.set_div(0);
	sdspc_obj->cur_traj.set_slope(0.1, 0.1);

	sdspc_obj->vol_ctl.set_kp(1.0);
	sdspc_obj->vol_ctl.set_ki(0.01);
	sdspc_obj->vol_ctl.set_kd(0);

	sdspc_obj->vol_div.set_div(9);
	sdspc_obj->vol_traj.set_slope(0.05, 0.05);

	// clear controller 
	sdspc_obj->clear();

	return nullptr;
}

gmp_wf_node_base_gt* quadri_sect_ctl_wf_pack::wfr_ready()
{
	// clear controller
	sdspc_obj->clear();
	return nullptr;
}

gmp_wf_node_base_gt* quadri_sect_ctl_wf_pack::wfr_poweron()
{
	return nullptr;
}

gmp_wf_node_base_gt* quadri_sect_ctl_wf_pack::wfr_quick_stop()
{
	return nullptr;
}

gmp_wf_node_base_gt* quadri_sect_ctl_wf_pack::wfr_fault()
{
	return nullptr;
}

void quadri_sect_ctl_wf_pack::trans_init()
{
}

void quadri_sect_ctl_wf_pack::trans_setup()
{
}

void quadri_sect_ctl_wf_pack::trans_release()
{

}

void quadri_sect_ctl_wf_pack::trans_poweron()
{
}

void quadri_sect_ctl_wf_pack::trans_poweroff()
{
}

void quadri_sect_ctl_wf_pack::trans_quickstop()
{
}

void quadri_sect_ctl_wf_pack::trans_oops()
{
}


void quadri_sect_ctl_wf_pack::trans_reset()
{

}

