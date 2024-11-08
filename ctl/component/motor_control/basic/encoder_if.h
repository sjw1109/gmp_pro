
#ifndef _FILE_ENCODER_INTERFACE_H_
#define _FILE_ENCODER_INTERFACE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // Motor rotation encoder
    typedef struct _tag_rotation_encoder_t
    {
        // All the rotation encoder should start with the following one items

        // output: position of the encoder, unit, p.u.
        ctrl_gt position;

        // output: electrical position output, unit p.u.
        ctrl_gt elec_position;

    } ctl_rotation_encif_t;

    // Get encoder position
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_encoder_position(ctl_rotation_encif_t *enc)
    {
        return enc->position;
    }

    // Get encoder position of electric
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_encoder_elec_postion(ctl_rotation_encif_t *enc)
    {
        return enc->elec_position;
    }

    // speed encoder
    typedef struct _tag_speed_encoder_t
    {
        // All the speed encoder should start with the following one items.

        // output: mechanical speed output, unit p.u.
        ctrl_gt speed;

    } ctl_speed_encif_t;

    // Get encoder speed
    GMP_STATIC_INLINE
    ctrl_gt ctl_get_encoder_speed(ctl_speed_encif_t *enc)
    {
        return enc->speed;
    }

    // Type transfer

    // position encoder
#define CTL_POSITION_IF(X) ((ctl_rotation_encif_t *)X)

    // velocity encoder
#define CTL_SPEED_IF(X) ((ctl_speed_encif_t *)X)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_ENCODER_INTERFACE_H_
