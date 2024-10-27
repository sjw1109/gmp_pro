


//////////////////////////////////////////////////////////////////////////
// Fusing

#include <ctl/component/intrinsic/discrete/fusing.h>

void ctl_init_bipolar_fusing(bipolar_fusing_t *fusing)
{
    fusing->target_object = NULL;

    fusing->upper_bound = CTRL_T(1.0f);
    fusing->lower_bound = -CTRL_T(1.0f);

    fusing->last_tick = 0;

    fusing->error_code = 1;
}

void ctl_bind_bipolar_fusing(bipolar_fusing_t *obj, ctrl_gt *target, gmp_stat_t error_code)
{
    obj->target_object = target;
    obj->error_code = error_code;
}

void ctl_set_bipolar_fusing_bound(bipolar_fusing_t *obj, ctrl_gt lower_bound, ctrl_gt upper_bound)
{
    obj->lower_bound = lower_bound;
    obj->upper_bound = upper_bound;
}
