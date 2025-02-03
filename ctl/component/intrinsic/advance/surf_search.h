
// This file implement a list of surface search algorithm.

#ifndef _FILE_SURF_SEARCH_H_
#define _FILE_SURF_SEARCH_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    //////////////////////////////////////////////////////////////////////////
    // linear Look up table
    //

    // lut_content is a strictly monotonically increasing sequence
    typedef struct _tag_linear_lut_t
    {
        // length of LUT size
        size_gt lut_size;

        // LUT content
        ctrl_gt *lut_content;

    } linear_lut_t;

    // init a linear LUT object
    void ctl_init_linear_lut(linear_lut_t *lut);

    // setup a LUT object based on specified content.
    void ctl_setup_linear_lut(linear_lut_t *lut, ctrl_gt *content, size_gt lut_size);

    // LUT content is
    //
    // | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | ... | last_index | lut_size     |
    // | a | b | c | d | e | f | g | h | i | j | ... | z          | out of range |
    //                       ^
    // return position = 5   |      when looking for target is equal to f
    // return position = -1         when target is less than `a`
    // return position = last_index when target is greater than 'z'
    // return position  = 5         when target is in [f, g) range.
    //
    // return position  = -2        when unknown error happened.
    GMP_STATIC_INLINE
    diff_gt ctl_get_lut_position_index(linear_lut_t *lut, ctrl_gt target)
    {
        size_gt left = 0;
        size_gt right = lut->lut_size - 1;
        size_gt mid = right / 2;

        // special case, leftest
        if (target < lut->lut_content[0])
            return -1;

        // special case, rightest
        if (target > lut->lut_content[right])
            return right;

        // binary search
        do
        {

            if (lut->lut_content[mid] > target)
            {
                // right = mid;

                // special case no left node exists
                if (mid == 0)
                {
                    return -1;
                }

                // judge if mid is a approximate solution
                if (lut->lut_content[mid - 1] < target)
                {
                    return mid - 1;
                }
                // accelerate the searching process
                else
                {
                    right = mid - 1;
                }
            }
            else if (lut->lut_content[mid] < target)
            {
                // left = mid;

                // special case no right node exists
                // last node index
                if (mid == lut->lut_size - 1)
                {
                    return lut->lut_size - 1;
                }

                // judge if mid is a approximate solution
                if (lut->lut_content[mid + 1] > target)
                {
                    return mid;
                }
                else
                {
                    left = mid + 1;
                }
            }
            // find correct result
            else
            {
                return mid;
            }

            // update mid new
            mid = left + (right - left) / 2;

        } while (left < right);

        // this branch will not meet.
        return -2;
    }

    // calculate weight to linear interpolate the result.
    // target & index is based on the get lut position function.
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_lut_position_weight(linear_lut_t *lut, ctrl_gt target, diff_gt index)
    {
        // special case leftest & rightest
        // operator `<=` and `>=` is error tolerance expression
        if (index <= -1)
            return float2ctrl(1.0f);

        if ((size_gt)index >= lut->lut_size - 1)
            return 0;

        // get left node and right node
        ctrl_gt left = lut->lut_content[index];
        ctrl_gt right = lut->lut_content[index + 1];

        ctrl_gt weight = ctl_div(target - left, right - left);

        return weight;
    }

    //////////////////////////////////////////////////////////////////////////
    // Surface 2D look up table
    //

    // monotonous increasing surface 2D look up table.
    // user may provide two nonlinear monotonous increasing scale as x & y axises.
    // then user may give a point (x, y) than the output function may tells the
    // surface(x,y) is based on the nonlinear discrete scale.
    //
    // A motor may be controlled based on LUT 2d algorithm, reference
    // https://ww2.mathworks.cn/help/mcb/ug/pmsm-characteristics-constraint-curves.html
    // https://ww2.mathworks.cn/help/mcb/gs/fwc-with-mtpa-of-non-linear-pmsm-using-lut.html
    //
    typedef struct _tag_surf_lut_t
    {
        // monotonous increasing axis 1 - x
        linear_lut_t dim1_lut;
        // monotonous increasing axis 2 - y
        linear_lut_t dim2_lut;

        // surface: s = surface(x, y);
        ctrl_gt **surface;

    } surf_lut_t;

    void ctl_init_surf_lut(surf_lut_t *lut);

    void ctl_setup_surf_lut(surf_lut_t *lut, ctrl_gt *asix_x, size_gt size_x, ctrl_gt *asix_y, size_gt size_y,
                            ctrl_gt **surface);

    // Obtain the value of the bottom left corner closest to the target point
    GMP_STATIC_INLINE
    ctrl_gt ctl_surf_lut_get_result(surf_lut_t *lut, ctrl_gt dim1_target, ctrl_gt dim2_target)
    {
        // get target index and weight
        diff_gt dim1_index = ctl_get_lut_position_index(&lut->dim1_lut, dim1_target);
        diff_gt dim2_index = ctl_get_lut_position_index(&lut->dim2_lut, dim2_target);

        if (dim1_index < 0)
            dim1_index = 0;
        if ((size_gt)dim1_index > lut->dim1_lut.lut_size - 1)
            dim1_index = lut->dim1_lut.lut_size - 1;
        if (dim2_index <= 0)
            dim2_index = 0;
        if ((size_gt)dim2_index > lut->dim2_lut.lut_size - 1)
            dim2_index = lut->dim2_lut.lut_size - 1;

        return lut->surface[dim1_index][dim2_index];
    }

    // get the target value based on surrounding 4 points interpolation algorithm
    // y^
    //  |
    //  + (12)          + (22)
    //
    //     .(target point)
    //
    //  + (O)          + (21) -> x
    //
    // tex:
    //$$ \mu_1 = \frac{x_t - x_0}{x_{21} - x_0} $$
    //$$ \mu_2 = \frac{y_t - y_0}{y_{12} - y_0} $$
    //$$ p_{target} = \mu_1\mu_2p_0 + (1-\mu_1)\mu_2 x_{21} + \mu_1(1-\mu_2)x_{12} + (1-\mu_1)(1-\mu_2)x_{22} $$
    //

    GMP_STATIC_INLINE
    ctrl_gt ctl_surf_lut_interp2(surf_lut_t *lut, ctrl_gt dim1_target, ctrl_gt dim2_target)
    {
        // get target index and weight
        diff_gt dim1_index = ctl_get_lut_position_index(&lut->dim1_lut, dim1_target);
        diff_gt dim2_index = ctl_get_lut_position_index(&lut->dim2_lut, dim2_target);

        ctrl_gt dim1_weight = ctl_get_lut_position_weight(&lut->dim1_lut, dim1_target, dim1_index);
        ctrl_gt dim2_weight = ctl_get_lut_position_weight(&lut->dim2_lut, dim2_target, dim2_index);

        // special cases, vertex condition
        if (dim1_index == -1 && dim2_index == -1)
        {
            return lut->surface[0][0];
        }
        else if ((dim1_index == lut->dim1_lut.lut_size - 1) && (dim2_index == -1))
        {
            return lut->surface[lut->dim1_lut.lut_size - 1][ 0];
        }
        else if ((dim1_index == -1) && (dim2_index == lut->dim2_lut.lut_size - 1))
        {
            return lut->surface[0][ lut->dim1_lut.lut_size - 1];
        }
        else if ((dim1_index == lut->dim1_lut.lut_size - 1) && (dim2_index == lut->dim2_lut.lut_size - 1))
        {
            return lut->surface[lut->dim1_lut.lut_size - 1][ lut->dim2_lut.lut_size - 1];
        }

        ctrl_gt original;
        ctrl_gt dim1_next;
        ctrl_gt dim2_next;
        ctrl_gt dim12_next;

        ctrl_gt output;

        // Marginal condition
        if (dim1_index == -1)
        {
            original = lut->surface[0][dim2_index];
            dim2_next = lut->surface[0][dim2_index + 1];

            output = ctl_mul(original, dim2_weight) + ctl_mul(dim2_next, (float2ctrl(1.0) - dim2_weight));
        }
        else if (dim1_index == lut->dim1_lut.lut_size - 1)
        {
            original = lut->surface[lut->dim1_lut.lut_size - 1][dim2_index];
            dim2_next = lut->surface[lut->dim1_lut.lut_size - 1][dim2_index + 1];

            output = ctl_mul(original, dim2_weight) + ctl_mul(dim2_next, (float2ctrl(1.0) - dim2_weight));
        }
        else if (dim2_index == -1)
        {
            original = lut->surface[dim1_index][0];
            dim1_next = lut->surface[dim1_index + 1][0];

            output = ctl_mul(original, dim1_weight) + ctl_mul(dim1_next, (float2ctrl(1.0) - dim1_weight));
        }
        else if (dim2_index == lut->dim2_lut.lut_size - 1)
        {
            original = lut->surface[dim1_index][lut->dim2_lut.lut_size - 1];
            dim1_next = lut->surface[dim1_index + 1][lut->dim2_lut.lut_size - 1];

            output = ctl_mul(original, dim1_weight) + ctl_mul(dim1_next, (float2ctrl(1.0) - dim1_weight));
        }

        // normal condition
        else
        {
            original = lut->surface[dim1_index][dim2_index];
            dim1_next = lut->surface[dim1_index + 1][dim2_index];
            dim2_next = lut->surface[dim1_index][dim2_index + 1];
            dim12_next = lut->surface[dim1_index + 1][dim2_index + 1];

            // calculate interpolation based on weight
            ctrl_gt weight_11 = ctl_mul(dim1_weight, dim2_weight);
            ctrl_gt weight_21 = ctl_mul((float2ctrl(1.0) - dim1_weight), dim2_weight);
            ctrl_gt weight_12 = ctl_mul(dim1_weight, (float2ctrl(1.0) - dim2_weight));
            ctrl_gt weight_22 = ctl_mul((float2ctrl(1.0) - dim1_weight), (float2ctrl(1.0) - dim2_weight));

            // get result
            output = ctl_mul(original, weight_11);
            output += ctl_mul(weight_21, dim1_next);
            output += ctl_mul(weight_12, dim2_next);
            output += ctl_mul(weight_22, dim12_next);
        }

        return output;
    }

    //////////////////////////////////////////////////////////////////////////
    // Uniformly divided surface
    //

    typedef struct _tag_uniform_surf_lut_t
    {
        ctrl_gt x_inf;      // infimum
        ctrl_gt x_sup;      // supremum
        size_gt x_sub;      // sub-dividing number
        ctrl_gt x_sub_coef; // sub-dividing coeffient

        ctrl_gt y_inf;      // infimum
        ctrl_gt y_sup;      // supremum
        size_gt y_sub;      // sub-dividing number
        ctrl_gt y_sub_coef; // sub-dividing coeffient

        ctrl_gt **surface; // point to a `surface[x_sub+1][y_sub+1];` array
    } uniform_surf_lut_t;

    void ctl_init_uniform_surf_lut(uniform_surf_lut_t *lut);

    void ctl_setup_uniform_surf_lut(uniform_surf_lut_t *lut, ctrl_gt x_inf, ctrl_gt x_sup, size_gt x_sub, ctrl_gt y_inf,
                                    ctrl_gt y_sup, size_gt y_sub, ctrl_gt **surface);

    // get uniform surface LUT result
    GMP_STATIC_INLINE
    ctrl_gt ctl_uniform_surf_lut_get_reuslt(uniform_surf_lut_t *lut, ctrl_gt coord_x, ctrl_gt coord_y)
    {
        diff_gt index_x = (size_gt)ctl_mul(lut->x_sub_coef, (coord_x - lut->x_inf));
        diff_gt index_y = (size_gt)ctl_mul(lut->y_sub_coef, (coord_y - lut->y_inf));

        if ((size_gt)index_x > lut->x_sub)
            index_x = lut->x_sub;
        else if (index_x < 0)
            index_x = 0;

        if ((size_gt)index_y > lut->y_sub)
            index_y = lut->y_sub;
        else if (index_y < 0)
            index_y = 0;

        return lut->surface[index_x][index_y];
    }

    //// minimum requirement of surface mesh
    //// dim1 and dim2 is a strictly monotonically increasing sequence
    // typedef struct _tag_lut_2d_t
    //{
    //     // specfy the first dimension division
    //     size_gt dim1_div;
    //
    //     // speicfy the second dimension division
    //     size_gt dim2_div;
    //
    //     // point to the surface to be seached
    //     ctrl_gt **surface;
    //
    //     // point to the the first dimension mash points
    //     ctrl_gt *dim1;
    //
    //     // point to the second dimension mash points
    //     ctrl_gt *dim2;
    //
    //     // input target point
    //     ctrl_gt coor1;
    //     ctrl_gt coor2;
    //
    //     // output target point coordinate
    //     // means the index of the target point
    //     diff_gt lut_coor1;
    //     diff_gt lut_coor2;
    //
    //     // surface is described by
    //     // tex:
    //     // $$result = surface(dim1^{-1}(coor1), dim2^{-1}(coor2));$$
    //
    //     // return the result of the search process.
    //     ctrl_gt target_result;
    //
    // } ctl_lut_2d_t;
    //
    // GMP_STATIC_INLINE
    // ctrl_gt get_2d_lookup_table(ctl_lut_2d_t *lut)
    //{
    //     assert(!lut);
    //     assert(!lut->surface);
    //     assert(!lut->dim1);
    //     assert(!lut->dim2);
    // }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_SURF_SEARCH_H_
