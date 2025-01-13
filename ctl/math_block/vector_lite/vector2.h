
#ifndef _FILE_CTL_VECTOR2_H_
#define _FILE_CTL_VECTOR2_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    typedef struct _tag_ctl_vector2_t
    {
        ctrl_gt dat[2];
    } ctl_vector2_t;

    GMP_STATIC_INLINE
    void ctl_vector2_clear(ctl_vector2_t *vec)
    {
        vec->dat[0] = 0;
        vec->dat[1] = 0;
    }

    GMP_STATIC_INLINE
    void ctl_vector2_copy(ctl_vector2_t *dup, ctl_vector2_t *vec)
    {
        dup->dat[0] = vec->dat[0];
        dup->dat[1] = vec->dat[1];
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_CTL_VECTOR2_H_
