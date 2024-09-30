/**
 * @file pid.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
#include <ctl/component/common/gmp_math.h>

#ifndef _FILE_PI_H_
#define _FILE_PI_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	// PI regular intermediate variables
	typedef struct _tag_pid_regular_inter_t
	{
		// output
		ctrl_gt out;

		// parameters
		ctrl_gt kp;
		ctrl_gt ki;
		ctrl_gt kd;

		ctrl_gt out_min;
		ctrl_gt out_max;

		// intermediate variables
		ctrl_gt sn; // sum-up of N
		ctrl_gt dn; // difference of N
	}pid_regular_t, ctl_pid_t;

	// paralleling PID model
	GMP_STATIC_INLINE
	void ctl_step_pid_par(pid_regular_t* hpid, ctrl_gt input)
	{
		// I sum up
		// Bug fix: hpid->sn may overflow
		hpid->sn = ctrl_sat(hpid->sn + ctrl_mpy(input, (hpid->ki)),
			hpid->out_max, hpid->out_min);

		// output = P item + I item  + D item
		hpid->out = ctrl_mpy(input, hpid->kp)
			+ hpid->sn
			+ ctrl_mpy((input - hpid->dn), hpid->kd);

		// output saturation
		hpid->out = ctrl_sat(hpid->out, hpid->out_max, hpid->out_min);

		// record input param
		hpid->dn = input;
	}

	// Series PID model
	GMP_STATIC_INLINE
	void ctl_step_pid_ser(pid_regular_t* hpid, ctrl_gt input)
	{
		// Kp gain firstly, out = P item
		hpid->out = ctrl_mpy(input, hpid->kp);

		// I sum up
		// Bug fix: hpid->sn may overflow
		hpid->sn = ctrl_sat(hpid->sn + ctrl_mpy(hpid->out, hpid->ki),
			hpid->out_max,hpid->out_min);

		// output += I item + D item
		hpid->out += hpid->sn + ctrl_mpy((input - hpid->dn), hpid->kd);

		// output saturation
		hpid->out = ctrl_sat(hpid->out, hpid->out_max, hpid->out_min );

		// record input param
		hpid->dn = input;
	}

	// initialize function
	void ctl_init_pid(pid_regular_t* hpid);

	void ctl_setup_pid(pid_regular_t* hpid,
		ctrl_gt kp, ctrl_gt ki, ctrl_gt kd,
		ctrl_gt out_min, ctrl_gt out_max);

	void ctl_set_pid_parameter(
		pid_regular_t* hpid,
		ctrl_gt kp, ctrl_gt ki, ctrl_gt kd
	);

	void ctl_set_pid_limit(
		pid_regular_t* hpid,
		ctrl_gt out_min, ctrl_gt out_max
	);
	
	GMP_STATIC_INLINE
	void ctl_clear_pid(
		pid_regular_t* hpid
	)
	{
		hpid->dn = 0;
		hpid->sn = 0;
	}

	// PID discrete structure

	// discrete PID with bilinear transform
	//tex:
	// $$\frac{U(z)}{E(z)} = \left. K_p + K_I\frac{1}{s} + K_ds\right|_{s = \frac{2}{T}\frac{1-z^{-1}}{1+z^{-1}}}\\
	//=\frac{1}{2T(1-z^{-2})} \cdot 
	//\left((4K_d - 2TK_p + T^2K_I)z^{-2} + (-8K_d+2T^2K_I)z^{-1} + (4K_d + 2TK_p + T^2K_I)\right)\\
	//=\frac{1}{2f(1-z^{-2})} \cdot 
	//\left((4f^2K_d - 2fK_p + K_I)z^{-2} + (-8f^2K_d+2K_I)z^{-1} + (4f^2K_d + 2fK_p + K_I)\right)\\
	//=\frac{1}{1-z^{-2}} \cdot 
	//\left((2fK_d)z^{-2} + (-4fK_d)z^{-1} + (2fK_d)\right),f\gg 1, \rm{for\; PID}\\
	//=\frac{1}{1-z^{-2}} \cdot 
	//\left((-Kp)z^{-2} + (1/f\cdot K_I(\approx 0))z^{-1} + (Kp)\right),f\gg 1, \rm{for\; PI}$$


	// discrete PID with basic transform
	//tex:
	// $$\frac{U(z)}{E(z)} = \left. K_p + K_I\frac{1}{s} + K_ds\right|_{s = \frac{z-1}{T}}$$


	//typedef struct _tag_discrete_pid_t
	//{
	//	// output
	//	ctrl_gt out;
	//
	//	// parameters
	//
	//
	//	// intrinsic variables
	//	gmp_ctrl_param_t Kp;
	//	gmp_ctrl_param_t Ki;
	//	gmp_ctrl_param_t Kd;
	//
	//
	//}disc_pid_t;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_PI_H_

