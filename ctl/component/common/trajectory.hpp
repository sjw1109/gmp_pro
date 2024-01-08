// trajectory module

#ifndef _FILE_TRAJECTORY_HPP_
#define _FILE_TRAJECTORY_HPP_

class traj_ramp_t
{
public:
	// output
	gmp_math_t out;

public:
	// slope setting
	gmp_math_t rising_slope;	// a positive value
	gmp_math_t falling_slope;	// a negative value

public:
	// input 
	gmp_math_t target;	 // target position
	gmp_math_t current;	 // current position

public:
	inline gmp_math_t get_next_position(gmp_math_t _target)
	{
		this->target = _target;
		gmp_math_t next_pos = current;
		next_pos += gmp_math_sat(target - current, falling_slope, rising_slope);
		return next_pos;
	}

public:
	// utilities
	// clear all the middle variables
	inline void clear()
	{
		this->target = 0;
		this->current = 0;
	}

	// Set rising and falling slope
	inline void set_slope(gmp_math_t rising, gmp_math_t falling)
	{
		rising_slope = rising;
		falling_slope = falling;
	}
};



#endif // _FILE_TRAJECTORY_HPP_
