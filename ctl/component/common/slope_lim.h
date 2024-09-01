
#ifndef _FILE_SLOPE_LIM_H_
#define _FILE_SLOPE_LIM_H_


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef struct _tag_slope_limit_t
	{
		// output
		ctrl_gt out;

		// parameters

		// slope limit
		ctrl_gt slope_min;
		ctrl_gt slope_max;

		// output limit
		ctrl_gt out_min;
		ctrl_gt out_max;

	}slope_lim_t;

	GMP_STATIC_INLINE 
	void ctl_step_slope_limit(slope_lim_t* obj, ctrl_gt input)
	{
		// Calculate slope delta
		ctrl_gt delta = ctrl_sat(input - obj->out, obj->slope_max, obj->slope_min);

		// calculate output data
		obj->out = ctrl_sat(obj->out + delta, obj->out_max, obj->out_min);
	}

	void ctl_init_slope_limit(slope_lim_t* obj);

	void ctl_setup_slope_limit(
		slope_lim_t* obj,
		ctrl_gt slope_min, ctrl_gt slope_max,
		ctrl_gt out_min, ctrl_gt out_max
	);

	void ctl_set_sl_slope(
		slope_lim_t* obj,
		ctrl_gt slope_min, ctrl_gt slope_max
	);

	void ctl_set_sl_limit(
		slope_lim_t* obj,
		ctrl_gt out_min, ctrl_gt out_max
	);

	static inline void ctl_clear_limit_slope(
		slope_lim_t* obj
	)
	{
		obj->out = 0;
	}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_SLOPE_LIM_H_
