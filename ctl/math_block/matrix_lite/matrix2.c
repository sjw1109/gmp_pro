
#include <gmp_core.h>

#include <ctl/ctl_core.h>

#include <ctl/math_block/matrix_lite/matrix2.h>

void ctl_matrix2_inv(ctl_matrix2_t *inv, ctl_matrix2_t *src)
{
    ctrl_gt gain = ctl_div(ctrl2float(1.0), ctl_mul(src->dat[0], src->dat[3]) - ctl_mul(src->dat[1], src->dat[2]));

    inv->dat[0] = ctl_mul(gain, src->dat[3]);
    inv->dat[1] = ctl_mul(gain, -src->dat[2]);
    inv->dat[2] = ctl_mul(gain, -src->dat[1]);
    inv->dat[3] = ctl_mul(gain, src->dat[0]);
}

// result = src1 * src2
void ctl_matrix2_mul(ctl_matrix2_t *result, ctl_matrix2_t *src1, ctl_matrix2_t *src2)
{
    result->dat[0] = ctl_mul(src1->dat[0], src2->dat[0]) + ctl_mul(src1->dat[1], src2->dat[2]);
    result->dat[1] = ctl_mul(src1->dat[0], src2->dat[1]) + ctl_mul(src1->dat[1], src2->dat[3]);
    result->dat[2] = ctl_mul(src1->dat[2], src2->dat[0]) + ctl_mul(src1->dat[3], src2->dat[2]);
    result->dat[3] = ctl_mul(src1->dat[2], src2->dat[1]) + ctl_mul(src1->dat[3], src2->dat[3]);
}

// result = mat * vec
void ctl_matrix2_mul_vector(ctl_vector2_t *result, ctl_matrix2_t *mat, ctl_vector2_t *vec)
{
    result->dat[0] = ctl_mul(mat->dat[0], vec->dat[0]) + ctl_mul(mat->dat[1], vec->dat[1]);
    result->dat[1] = ctl_mul(mat->dat[2], vec->dat[0]) + ctl_mul(mat->dat[3], vec->dat[1]);
}

// result = vec * mat
// where, result and vec is a row vector
void ctl_vector2_mul_mat(ctl_vector2_t *result, ctl_vector2_t *vec, ctl_matrix2_t *mat)
{
    result->dat[0] = ctl_mul(mat->dat[0], vec->dat[0]) + ctl_mul(mat->dat[2], vec->dat[1]);
    result->dat[1] = ctl_mul(mat->dat[0], vec->dat[1]) + ctl_mul(mat->dat[3], vec->dat[1]);
}

// result = src^T
void ctl_matrix2_trans(ctl_vector2_t *result, ctl_matrix2_t *src)
{
    result->dat[0] = src->dat[0];
    result->dat[1] = src->dat[2];
    result->dat[2] = src->dat[1];
    result->dat[3] = src->dat[3];
}

// result = a + b
void ctl_matrix2_add(ctl_matrix2_t *result, ctl_matrix2_t *a, ctl_matrix2_t *b)
{
    result->dat[0] = a->dat[0] + b->dat[0];
    result->dat[1] = a->dat[1] + b->dat[1];
    result->dat[2] = a->dat[2] + b->dat[2];
    result->dat[3] = a->dat[3] + b->dat[3];
}

// result = a - b
void ctl_matrix2_sub(ctl_matrix2_t *result, ctl_matrix2_t *a, ctl_matrix2_t *b)
{
    result->dat[0] = a->dat[0] - b->dat[0];
    result->dat[1] = a->dat[1] - b->dat[1];
    result->dat[2] = a->dat[2] - b->dat[2];
    result->dat[3] = a->dat[3] - b->dat[3];
}

// result = a + b * I
// I is unit matrix, b is a scale const
void ctl_matrix2_add_eye(ctl_matrix2_t *result, ctl_matrix2_t *a, ctrl_gt b)
{
    result->dat[0] = a->dat[0] + b;
    result->dat[1] = a->dat[1] + b;
    result->dat[2] = a->dat[2] + b;
    result->dat[3] = a->dat[3] + b;
}

// result = b * a, scalar multiplication
// A is a matrix, b is a scale const
void ctl_matrix2_gain(ctl_matrix2_t *result, ctl_matrix2_t *a, ctrl_gt b)
{
    result->dat[0] = ctl_mul(a->dat[0], b);
    result->dat[1] = ctl_mul(a->dat[1], b);
    result->dat[2] = ctl_mul(a->dat[2], b);
    result->dat[3] = ctl_mul(a->dat[3], b);
}

// result = p^T * A * p
// for matrix2 is,
// tex:
// $$ a_0p_0^2+a_0p_0p_2+a_1p_0p_1+a_3p_1p_2,\; a_0p_0p_2+a_1p_2^2+a_1p_0p_3+a_3p_2p_3 \\
//    a_0p_0p_1+a_2p_0p_3+a_1p_1^2+a_3p_1p_3,\; a_0p_1p_2+a_2p_2p_3+a_1p_1p_3+a_3p_3^2
// $$
void ctl_matrix2_congruent(ctl_matrix2_t *result, ctl_matrix2_t *a, ctl_matrix2_t *p)
{
    ctrl_gt p0p0 = ctl_mul(p->dat[0], p->dat[0]);
    ctrl_gt p0p1 = ctl_mul(p->dat[0], p->dat[1]);
    ctrl_gt p0p2 = ctl_mul(p->dat[0], p->dat[2]);
    ctrl_gt p0p3 = ctl_mul(p->dat[0], p->dat[3]);
    ctrl_gt p1p1 = ctl_mul(p->dat[1], p->dat[1]);
    ctrl_gt p1p2 = ctl_mul(p->dat[1], p->dat[2]);
    ctrl_gt p1p3 = ctl_mul(p->dat[1], p->dat[3]);
    ctrl_gt p2p2 = ctl_mul(p->dat[2], p->dat[2]);
    ctrl_gt p2p3 = ctl_mul(p->dat[2], p->dat[3]);
    ctrl_gt p3p3 = ctl_mul(p->dat[3], p->dat[3]);

    result->dat[0] =
        ctl_mul(a->dat[0], p0p0) + ctl_mul(a->dat[0], p0p2) + ctl_mul(a->dat[1], p0p1) + ctl_mul(a->dat[1], p1p2);
    result->dat[1] =
        ctl_mul(a->dat[0], p0p2) + ctl_mul(a->dat[1], p2p2) + ctl_mul(a->dat[1], p0p3) + ctl_mul(a->dat[3], p2p3);
    result->dat[2] =
        ctl_mul(a->dat[0], p0p1) + ctl_mul(a->dat[2].p0p3) + ctl_mul(a->dat[1], p1p1) + ctl_mul(a->dat[3], p1p3);
    result->dat[3] =
        ctl_mul(a->dat[0], p1p2) + ctl_mul(a->dat[2], p2p3) + ctl_mul(a->dat[1], p1p3) + ctl_mul(a->dat[3], p3p3);
}
