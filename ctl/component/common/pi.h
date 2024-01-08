
#ifndef _FILE_PI_H_
#define _FILE_PI_H_


// PI regular intermediate variables
typedef struct _tag_pid_regular_inter_t
{
	// output
	gmp_math_t out;

	// parameters
	gmp_math_t kp;
	gmp_math_t ki;
	gmp_math_t kd;

	gmp_math_t out_min;
	gmp_math_t out_max;

	// intermediate variables
	gmp_math_t sn; // sum-up of N
	gmp_math_t dn; // difference of N
}pid_regular_t;

// paralleling PID model
inline void gmp_pid_par(pid_regular_t& hpid, gmp_math_t input)
{
	hpid->sn += gmp_math_sat(input * (pid_obj.ki), hpid->out_min, hpid->out_max);
	hpid->out = input * hpid->kp + hpid->sn + (input - hpid->dn) * hpid->kd;
	hpid->out = gmp_math_sat(hpid->out, hpid->out_min, hpid->out_max);
	hpid->dn = input;
}

// Series PID model
inline void gmp_pid_ser(pid_regular_t& hpid, gmp_math_t input)
{
	hpid->out = input * hpid->kp;
	hpid->sn += gmp_math_sat(hpid->out * hpid->ki, hpid->out_min, hpid->out_max);
	hpid->out += hpid->sn + (input - hpid->dn) * pid_obj.kd;
	hpid->out = gmp_math_sat(hpid->out, hpid->out_min, hpid->out_max);
	hpid->dn = input;
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


typedef struct _tag_discrete_pid_t
{
	// output
	gmp_math_t out;

	// parameters


	// intrinsic variables
	gmp_ctrl_param_t Kp;
	gmp_ctrl_param_t Ki;
	gmp_ctrl_param_t Kd;


}disc_pid_t;

#endif // _FILE_PI_H_

