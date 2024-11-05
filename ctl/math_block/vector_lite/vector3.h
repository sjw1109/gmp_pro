
#ifndef _FILE_CTL_VECTOR3_H_
#define _FILE_CTL_VECTOR3_H_

typedef struct _tag_ctl_vector3_t
{
    ctrl_gt dat[3];
} ctl_vector3_t;

GMP_STATIC_INLINE
void ctl_clear_vector3(ctl_vector3_t *vec)
{
    vec->dat[0] = 0;
    vec->dat[1] = 0;
    vec->dat[2] = 0;
}

#endif // _FILE_CTL_VECTOR3_H_
