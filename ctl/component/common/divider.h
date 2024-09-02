#ifndef _FILE_DIVIDER_H_
#define _FILE_DIVIDER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_divider_t 
{
	// Counter for divider
	uint32_t counter;

	// Counter target
	//+ target == 0
	// output 
	//+ target >= 1
	// output frequency = real frequency / target;
	uint32_t target;

	// bypass switch
	// 1: disable the block
	// 0: enable the block
	fast_gt flag_bypass;

}ctl_divider_t;

static inline fast_gt ctl_step_divider(ctl_divider_t* obj)
{
	// flag_bypass == 1, skip the branch
	if (obj->flag_bypass)
	{
		return 0;
	}
	else
	{
		obj->counter += 1;

		if (obj->counter >= obj->target)
		{
			obj->counter = 0;

			// active the branch
			return 1;
		}

		// skip the branch temporary
		return 0;
	}
}

void ctl_init_divider(ctl_divider_t* obj);

void ctl_set_divider(ctl_divider_t* obj, uint32_t counter_period);

static inline void ctl_clear_divider(ctl_divider_t* obj)
{
	obj->counter = 0;
}


#ifdef __cplusplus
}
#endif //__cplusplus

#endif//_FILE_DIVIDER_H_
