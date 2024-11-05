
#include <ctl/ctl_core.h>
#include <gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// Fusing

#include <ctl/component/intrinsic/protection/fusing.h>

ec_gt ctl_init_bipolar_fusing(bipolar_fusing_t *fusing)
{
    fusing->target_object = NULL;

    fusing->upper_bound = float2ctrl(1.0f);
    fusing->lower_bound = -float2ctrl(1.0f);

    fusing->last_tick = 0;

    fusing->error_code = 1;

    return GMP_EC_OK;
}

void ctl_bind_bipolar_fusing(bipolar_fusing_t *obj, ctrl_gt *target, ec_gt error_code)
{
    obj->target_object = target;
    obj->error_code = error_code;
}

void ctl_set_bipolar_fusing_bound(bipolar_fusing_t *obj, ctrl_gt lower_bound, ctrl_gt upper_bound)
{
    obj->lower_bound = lower_bound;
    obj->upper_bound = upper_bound;
}
