/**
 * @file hcc.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#ifndef _FILE_HCC_H_
#define _FILE_HCC_H_

#ifdef __cpluplus
extern "C"
{
#endif // __cpluplus

    typedef struct _tag_hcc_t
    {

        ctrl_gt target;
        ctrl_gt half_width;
        ctrl_gt current;

        fast_gt switch_out;

        // 1: positive,
        // 0: negative,
        fast_gt flag_polarity;
    } ctl_hcc_t;

    //ec_gt ctl_init_hcc(ctl_hcc_t *hcc);

    //ec_gt ctl_setup_hcc(ctl_hcc_t *hcc, fast_gt flag_polarity, ctrl_gt half_width);

    void ctl_init_hcc(ctl_hcc_t *hcc, fast_gt flag_polarity, ctrl_gt half_width);

    GMP_STATIC_INLINE
    fast_gt ctl_step_hcc(ctl_hcc_t *hcc, ctrl_gt input)
    {
        hcc->current = input;

        if (hcc->current >= hcc->target + hcc->half_width)
        {
            hcc->switch_out = hcc->flag_polarity;
        }
        else if (hcc->current <= hcc->target - hcc->half_width)
        {
            hcc->switch_out = 1 - hcc->flag_polarity;
        }

        return hcc->switch_out;
    }

    // Parameter config function

    GMP_STATIC_INLINE
    void ctl_set_hcc_target(ctl_hcc_t *hcc, ctrl_gt target)
    {
        hcc->target = target;
    }

    GMP_STATIC_INLINE
    fast_gt ctl_get_hcc_output(ctl_hcc_t *hcc)
    {
        return hcc->switch_out;
    }

#ifdef __cpluplus
}
#endif // __cpluplus

#endif //_FILE_HCC_H_
