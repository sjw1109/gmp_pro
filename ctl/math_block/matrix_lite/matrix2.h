// qc 补充
// TODO: 在做除法时应当做安全检查

#include <ctl/math_block/vector_lite/vector2.h>

#ifndef _FILE_GMP_CTL_MATRIX2_H_
#define _FILE_GMP_CTL_MATRIX2_H_

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct _tag_ctl_matrix2_t
    {
        ctrl_gt dat[4];
    } ctl_matrix2_t;

    // inv = src^{-1}
    void ctl_matrix2_inv(ctl_matrix2_t *inv, ctl_matrix2_t *src);

    // result = src1 * src2
    void ctl_matrix2_mul(ctl_matrix2_t *result, ctl_matrix2_t *src1, ctl_matrix2_t *src2);

    // result = mat * vec
    void ctl_matrix2_mul_vector(ctl_vector2_t *result, ctl_matrix2_t *mat, ctl_vector2_t *vec);

    // result = vec * mat
    // where, result and vec is a row vector
    void ctl_vector2_mul_mat(ctl_vector2_t *result, ctl_vector2_t *vec, ctl_matrix2_t *mat);

    // result = src^T
    void ctl_matrix2_trans(ctl_vector2_t *result, ctl_matrix2_t *src);

    // result = a + b
    void ctl_matrix2_add(ctl_matrix2_t *result, ctl_matrix2_t *a, ctl_matrix2_t *b);

    // result = a - b
    void ctl_matrix2_sub(ctl_matrix2_t *result, ctl_matrix2_t *a, ctl_matrix2_t *b);

    // result = a + b * I
    // I is unit matrix, b is a scale const
    void ctl_matrix2_add_eye(ctl_matrix2_t *result, ctl_matrix2_t *a, ctrl_gt b);

    // result = b * a, scalar multiplication
    // A is a matrix, b is a scale const
    void ctl_matrix2_gain(ctl_matrix2_t *result, ctl_matrix2_t *a, ctrl_gt b);

    // result = p^T * A * p
    // for matrix2 is,
    //tex:
    // $$ a_0p_0^2+a_0p_0p_2+a_1p_0p_1+a_3p_1p_2,\; a_0p_0p_2+a_1p_2^2+a_1p_0p_3+a_3p_2p_3 \\
    //    a_0p_0p_1+a_2p_0p_3+a_1p_1^2+a_3p_1p_3,\; a_0p_1p_2+a_2p_2p_3+a_1p_1p_3+a_3p_3^2
    // $$
    void ctl_matrix2_congruent(ctl_matrix2_t *result, ctl_matrix2_t *a, ctl_matrix2_t *p);

#ifdef __cplusplus
}
#endif

#endif // _FILE_GMP_CTL_MATRIX2_H_
