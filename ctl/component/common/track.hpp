// tracking controller
// This controller contains a general zero target controller,
// and the tracking controller will generate a error signal.


#include <ctrl/component/commmon/pid.hpp>

#ifndef _FILE_TRACK_HPP_
#define _FILE_TRACK_HPP_

// A basic tracking controller should be implemented by a general regulator,
// for instance PID regulator or type II controller; 
// and a feed forward controller.
// The inner feed back controller should contain a public method named `exec(error)`.
// and the inner feedback regulator must has an output value named `out`.
// The inner feed forward controller should contain a public method named `exec(target, feedback)`.
// and the inner feed forward regulator must has an output value named `out`.
template <typename fb_type,
	typename ff_type>
class tracking_ctrl_gt
{
public:
	// output
	gmp_math_t out;

public:
	// feedback controller
	fb_type fb;

	// feed forward
	ff_type ff;

	// bound
	gmp_math_t out_min;
	gmp_math_t out_max;

public:
	// default controller function
	inline void exec(gmp_math_t target, gmp_math_t feedback)
	{
		ff.exec(target, feedback);
		fb.exec(target - feedback);

		out = fb.out + ff.out;

		gmp_math_sat(out, out_min, out_max);

	}

public:
	// utility member functions
	// set tracking object limit
	void set_limit(gmp_math_t out_min, gmp_math_t out_max)
	{
		this->out_min = out_min;
		this->out_max = out_max;
	}
};


//////////////////////////////////////////////////////////////////////////
// export types
typedef tracking_ctrl_gt<pid_reg_t> tracking_pid_t;

#endif // _FILE_TRACK_HPP_
