/**
 * @file divider.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *       2024-10-25 standardization
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

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

} ctl_divider_t;

GMP_STATIC_INLINE
fast_gt ctl_step_divider(ctl_divider_t *obj)
{

    obj->counter += 1;

    if (obj->counter >= obj->target)
    {
        obj->counter = 0;

        // active the branch
        return 1;
    }

    return 0;
}

void ctl_init_divider(ctl_divider_t *obj, uint32_t counter_period);

GMP_STATIC_INLINE
void ctl_clear_divider(ctl_divider_t *obj)
{
    obj->counter = 0;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_FILE_DIVIDER_H_
