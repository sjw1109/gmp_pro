// This file provide a basic divider for controller

#include <stdint.h>

#ifndef _FILE_DIVIDER_H_
#define _FILE_DIVIDER_H_

class ctrl_divider_t
{
public:
	// ctor & dtor
	ctrl_divider_t()
		:ctrl_cnt_div(0), ctrl_cnt(0)
	{}

	ctrl_divider_t(uint16_t scaler)
		:ctrl_cnt_div(scaler), ctrl_cnt(0)
	{}

public:
	// counter divider
	uint16_t ctrl_cnt;
	uint16_t ctrl_cnt_div;

public:
	// Main scaler function 
	inline uint8_t is_tick()
	{
		if (ctrl_cnt++ >= ctrl_cnt_div)
			return 1;
		return 0;
	}

	inline void clear()
	{
		ctrl_cnt = 0;
	}

	inline void set_div(uint16_t target_div)
	{
		ctrl_cnt_div = target_div;
	}

};


#endif // _FILE_DIVIDER_H_

