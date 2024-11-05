
#ifndef _FILE_CTL_VECTOR2_H_
#define _FILE_CTL_VECTOR2_H_


typedef struct _tag_ctl_vector2_t
{
    ctrl_gt dat[2];
} ctl_vector2_t;

GMP_STATIC_INLINE
void ctl_clear_vector2(ctl_vector2_t* vec)
{
    vec->dat[0] = 0;
    vec->dat[1] = 0;
}

#endif // _FILE_CTL_VECTOR2_H_
