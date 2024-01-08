
#include <ctrl/ctrl_config.h>
#include <ctrl/component/common/gmp_math.h>

#ifndef _FILE_PID_H_
#define _FILE_PID_H_

// common PID controller
class pid_reg_t
{
public:
	// output
	gmp_math_t out;

public:
	// parameters
	gmp_math_t kp;
	gmp_math_t ki;
	gmp_math_t kd;

public:
	gmp_math_t out_min;
	gmp_math_t out_max;

	// intermediate variables
	gmp_math_t sn; // sum-up of N
	gmp_math_t dn; // difference of N

public:
	inline void exec(gmp_math_t erro)
	{
		sn += gmp_math_sat(erro * ki, out_min, out_max);
		out = erro * kp + sn + (erro - dn) * kd;
		out = gmp_math_sat(out, out_min, out_max);
		dn = erro;
	}

	inline void exec(gmp_math_t target, gmp_math_t feedback)
	{
		exec(target - feedback);
	}

public:
	// utilities
	inline void set_min_limit(gmp_math_t out_min)
	{
		this->out_min = out_min;
	}

	inline void set_max_limit(gmp_math_t out_max)
	{
		this->out_max = out_max;
	}

	// clear all the middle parameters
	inline void clear()
	{
		this->out = 0;
		this->sn = 0;
		this->dn = 0;
	}

	inline void set_kp(gmp_math_t kp)
	{
		this->kp = kp;
	}

	inline void set_ki(gmp_math_t ki)
	{
		this->ki = ki;
	}
	inline void set_kd(gmp_math_t kd)
	{
		this->kd = kd;
	}
};

// This type of	PID is easy for user to tuning PID parameters
class var_pid_reg_t
	:public pid_reg_t
{
	// public:
	// 	// output
	// 	gmp_math_t out;
	// 
	// public:
	// 	// shadow parameters for user
	// 	gmp_math_t kp;
	// 	gmp_math_t ki;
	// 	gmp_math_t kd;
	// 
	// 	gmp_math_t out_min;
	// 	gmp_math_t out_max;
private:
	// real parameters
	gmp_math_t kp_calc;
	gmp_math_t ki_calc;
	gmp_math_t kd_calc;

public:
	// intermediate variables
// 	gmp_math_t sn; // sum-up of N
// 	gmp_math_t dn; // difference of N

public:
	inline void exec(gmp_math_t erro)
	{
		// move user parameter to calculating parameters
		kp_calc = kp;
		// keep same integral result
		if (ki_calc != ki)
			sn = gmp_math_sat(sn * ki_calc / ki, out_min, out_max);
		ki_calc = ki;
		kd_calc = kd;

		// calculate PID regulation output
		sn += gmp_math_sat(erro * ki_calc, out_min, out_max);
		out = erro * kp_calc + sn + (erro - dn) * kd_calc;
		out = gmp_math_sat(out, out_min, out_max);
		dn = erro;
	}

	inline void exec(gmp_math_t target, gmp_math_t feedback)
	{
		exec(target - feedback);
	}


	// 	inline void exec(gmp_math_t target, gmp_math_t feedback)
	// 	{
	// 		exec(target - feedback);
	// 	}
	// 
	// public:
	// 	// utilities
	// 	inline void set_min_limit(gmp_math_t out_min)
	// 	{
	// 		this->out_min = out_min;
	// 	}
	// 
	// 	inline void set_max_limit(gmp_math_t out_max)
	// 	{
	// 		this->out_max = out_max;
	// 	}
	// 
	// 	// clear all the middle parameters
	// 	inline void clear()
	// 	{
	// 		this->out = 0;
	// 		this->sn = 0;
	// 		this->dn = 0;
	// 	}
};



#endif // _FILE_PID_H_
