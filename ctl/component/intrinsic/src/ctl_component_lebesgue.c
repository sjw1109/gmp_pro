/**
 * @file ctl_common_init.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-03-19
 *
 * @copyright Copyright GMP(c) 2024
 *
 */
#include <ctl/ctl_core.h>
#include <gmp_core.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////
// HCC regular

#include <ctl/component/intrinsic/lebesgue/hcc.h>

// ec_gt ctl_init_hcc(ctl_hcc_t *hcc)
//{
//     hcc->target = 0;
//     hcc->half_width = float2ctrl(0.5);
//     hcc->current = 0;
//     hcc->switch_out = 0;
//     hcc->flag_polarity = 1;
//
//     return GMP_EC_OK;
// }

void ctl_init_hcc(ctl_hcc_t *hcc, fast_gt flag_polarity, ctrl_gt half_width)
{
    hcc->target = 0;
    hcc->current = 0;
    hcc->switch_out = 0;
    hcc->flag_polarity = flag_polarity;
    hcc->half_width = half_width;
}
