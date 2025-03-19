/**
 * @file pid.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#include <ctl/math_block/gmp_math.h>

#ifndef _FILE_PI_DISCRETE_H_
#define _FILE_PI_DISCRETE_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    // PID transfer function 
    //tex:
    // $$ G(s) = k_p + \frac{k_i}{s} + k_d s $$
    // where, $$ T_i = \frac{k_p}{k_i} $$ ,
    // $$T_d = \frac{k_d}{k_p}$$ 

    // using Tustin/Trapezoidal Transformation
    //tex:
    // $$ s = 2f_s\frac{1-z^{-1}}{1+z^{-1}}$$ 

    // for integral item use backward Euler Transformation
    //tex:
    // $$ s = f_s (1-z^{-1})$$

    typedef struct _tag_discrete_pid_tuning_t
    {

        parameter_gt kp;
        parameter_gt Ti;
        parameter_gt Td;
        parameter_gt fs;
    };



    typedef struct _tag_discrete_pid_t
    {
        ctrl_gt b2, b1, b0;
        ctrl_gt input;
        ctrl_gt input_1;
        ctrl_gt input_2;

        ctrl_gt output;
        ctrl_gt output_1;

        ctrl_gt output_max;
        ctrl_gt output_min;
    }discrete_pid_t;

    void ctl_init_discrete_pid(discrete_pid_t *pid, parameter_gt kp, parameter_gt Ti, parameter_gt Td, parameter_gt fs);
    



    // PID discrete structure

    // discrete PID with bilinear transform
    // tex:
    // $$\frac{U(z)}{E(z)} = \left. K_p + K_I\frac{1}{s} + K_ds\right|_{s = \frac{2}{T}\frac{1-z^{-1}}{1+z^{-1}}}\\
    //=\frac{1}{2T(1-z^{-2})} \cdot
    //\left((4K_d - 2TK_p + T^2K_I)z^{-2} + (-8K_d+2T^2K_I)z^{-1} + (4K_d + 2TK_p + T^2K_I)\right)\\
    //=\frac{1}{2f(1-z^{-2})} \cdot
    //\left((4f^2K_d - 2fK_p + K_I)z^{-2} + (-8f^2K_d+2K_I)z^{-1} + (4f^2K_d + 2fK_p + K_I)\right)\\
    //=\frac{1}{1-z^{-2}} \cdot
    //\left((2fK_d)z^{-2} + (-4fK_d)z^{-1} + (2fK_d)\right),f\gg 1, \rm{for\; PID}\\
    //=\frac{1}{1-z^{-2}} \cdot
    //\left((-Kp)z^{-2} + (1/f\cdot K_I(\approx 0))z^{-1} + (Kp)\right),f\gg 1, \rm{for\; PI}$$

    // discrete PID with basic transform
    // tex:
    // $$\frac{U(z)}{E(z)} = \left. K_p + K_I\frac{1}{s} + K_ds\right|_{s = \frac{z-1}{T}}$$

    // typedef struct _tag_discrete_pid_t
    //{
    //	// output
    //	ctrl_gt out;
    //
    //	// parameters
    //
    //
    //	// intrinsic variables
    //	gmp_ctrl_param_t Kp;
    //	gmp_ctrl_param_t Ki;
    //	gmp_ctrl_param_t Kd;
    //
    //
    // }disc_pid_t;

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _FILE_PI_H_
