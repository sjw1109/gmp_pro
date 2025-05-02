#ifndef _FILE_BUCKBOOST_4CH_H_
#define _FILE_BUCKBOOST_4CH_H_

#ifdef __cplusplus
extenr "C"
{
#endif // __cplusplus

#define D_buck_max float2ctrl(0.8)

    typedef enum _tag_buckboost_phase
    {
        buck_phase = 0,
        boost_phase = 1
    } ctl_buckboost_phases_t;

    // define a function to determine how to set 2 duty of two switches.
    // buckboost controller has divided into 4 section
    // buck section: ratio range [0, D_buck_max] only buck controller is running.
    // buck-boost section: ratio range [ D_buck_max, 1] boost controller is raising.
    // buck-boost section: ratio range [ 1, 1/D_buck_max] buck controller is decreasing.
    // boost-section: ratio range [1/D_buck_max, infty] only boost controller is running.
    GMP_STATIC_INLINE void ctl_buckboost_duty_preset(ctrl_gt ratio, ctl_vector2_t * buck_boost_duty)
    {
        if (ratio <= D_buck_max)
        {
            buck_boost_duty->dat[buck_phase] = ratio;
            buck_boost_duty->dat[boost_phase] = 0;
        }
        else if (ratio <= float2ctrl(1))
        {
            buck_boost_duty->dat[buck_phase] = D_buck_max;
            buck_boost_duty->dat[boost_phase] = ratio - D_buck_max;
        }
        else if (ratio <= ctl_div(float2ctrl(1), D_buck_max))
        {
            buck_boost_duty->dat[buck_phase] = D_buck_max;
            buck_boost_duty->dat[boost_phase] = (float2ctrl(1) - D_buck_max);
        }
        else
        {
            buck_boost_duty->dat[buck_phase] = 0;
            buck_boost_duty->dat[boost_phase] = ctl_div(ratio - float2ctrl(1), ratio);
        }
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_BUCKBOOST_4CH_H_
