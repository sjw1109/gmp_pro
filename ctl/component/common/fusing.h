#ifndef _FILE_FUSING_H_
#define _FILE_FUSING_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	// if two fusing event happened 
#define CTL_FUSING_ISR_LIMIT ((10))

	typedef struct _tag_bipolar_fusing_t
	{
		// The object to be monitored
		ctrl_gt* target_object;

		// upper bound
		ctrl_gt upper_bound;

		// lower bound
		ctrl_gt lower_bound;

		// parameter last out-of-range ISR tick
		uint32_t last_tick;

		// parameter when fusing event happened, 
		// the error code may assigned to controller 
		gmp_stat_t error_code;

	}bipolar_fusing_t;

	static inline 
		void ctl_check_fusing(
		ctl_object_nano_t* pctl_obj,
		bipolar_fusing_t* fusing
	)
	{
		uint32_t delta_tick;

		// If no items are assigned to the fusing object
		// just ignore.
			if (!fusing->target_object)
			return;

		// Check Bound
		if (*fusing->target_object > fusing->upper_bound ||
			*fusing->target_object < fusing->lower_bound)
		{
			// Calculate two fusing event 
			if (pctl_obj->isr_tick > fusing->last_tick)
			{
				delta_tick = pctl_obj->isr_tick - fusing->last_tick;
			}
			else
			{
				delta_tick = UINT32_MAX - fusing->last_tick + pctl_obj->isr_tick;
			}

			// Fault condition
			if (delta_tick <= CTL_FUSING_ISR_LIMIT)
			{
				pctl_obj->state_machine = CTL_SM_FAULT;
				pctl_obj->error_code = fusing->error_code;
			}

			// Log the fusing event
			fusing->last_tick = pctl_obj->isr_tick;
		}
		return;
	}

	void ctl_init_bipolar_fusing(bipolar_fusing_t* fusing);

	void ctl_bipolar_fusing_bind(bipolar_fusing_t* obj,
		ctrl_gt* target, gmp_stat_t error_code);

	void ctl_set_bipolar_fusing_bound(bipolar_fusing_t* obj,
		ctrl_gt lower_bound, ctrl_gt upper_bound);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_FUSING_H_
