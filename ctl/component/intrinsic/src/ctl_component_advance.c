
#include <gmp_core.h>

//////////////////////////////////////////////////////////////////////////
// surf_search, surface search function

#include <ctl/component/intrinsic/advance/surf_search.h>

void ctl_init_linear_lut(linear_lut_t *lut, ctrl_gt *content, size_gt lut_size)
{
    lut->lut_content = content;
    lut->lut_size = lut_size;
}

void ctl_init_surf_lut(surf_lut_t *lut, ctrl_gt *asix_x, size_gt size_x, ctrl_gt *asix_y, size_gt size_y,
                        ctrl_gt **surface)
{
    ctl_init_linear_lut(&lut->dim1_lut, asix_x, size_x);
    ctl_init_linear_lut(&lut->dim2_lut, asix_y, size_y);

    lut->surface = surface;
}

void ctl_init_uniform_surf_lut(uniform_surf_lut_t *lut, ctrl_gt x_inf, ctrl_gt x_sup, size_gt x_sub, ctrl_gt y_inf,
                                ctrl_gt y_sup, size_gt y_sub, ctrl_gt **surface)
{
    lut->x_inf = x_inf;
    lut->x_sup = x_sup;
    lut->x_sub = x_sub;

    lut->y_inf = y_inf;
    lut->y_sup = y_sup;
    lut->y_sub = y_sub;

    lut->surface = surface;

    ctrl_gt x_diff_coef = ctl_div(float2ctrl(1.0f), (x_sup - x_inf));
    lut->x_sub_coef = ctl_mul(x_diff_coef, lut->x_sub);

    ctrl_gt y_diff_coef = ctl_div(float2ctrl(1.0f), (y_sup - y_inf));
    lut->y_sub_coef = ctl_mul(y_diff_coef, lut->y_sub);
}
