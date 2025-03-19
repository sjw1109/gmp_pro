/**
 * @file PZCompensator.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2025-03-19
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

#ifndef FILE_PZ_COMPENSATOR_H_
#define FILE_PZ_COMPENSATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus



    // tex:
    //  Bilinear transform:
    //  $$s = \frac{2}{T} \frac{1-z^{-1}}{1+z^{-1}}$$
    //
    //  $$z = \frac{1+sT/2}{1-sT/2}$$

    // 1 pole 1 zero controller
    // 1-order IIR filter

    // Programmer View
    // u(n) = a_1 * u(n-1) + b_1 * e(n-1)
    //      + b_0 * e(n);

    // Math View
    // tex:
    // Recursion Formula:
    // $$u_{(n)} = a_1 u_{(n-1)} + b_1 e_{(n-1)} + b_0 e_{(n)} $$
    // Complex Frequency Domain Transform Function:
    // $$\frac{U(z)}{E(z)} = \frac{b_1 z^{-1}+b_0}{1-a_1 z^{-1}}$$

    typedef struct _tag_ctrl_1p1z_t
    {
        // u(n)
        ctrl_gt output;

        // coef_a->a_1
        ctrl_gt coef_a;
        // coef_b[0]->b_0, coef_b[1]->b_1
        ctrl_gt coef_b[2];
        // resp->u(n-1)
        ctrl_gt resp; // response buffer
        // input->e(n), exct->e(n-1)
        ctrl_gt exct; // excitation buffer

        ctrl_gt out_max;
        ctrl_gt out_sto_min; // generally is -0.9
        ctrl_gt out_min;

    } ctrl_1p1z_t;

    GMP_STATIC_INLINE
    ctrl_gt ctl_step_1p1z(ctrl_1p1z_t *c, ctrl_gt input)
    {
        c->output = ctl_mul(c->coef_a, c->resp) + ctl_mul(c->coef_b[1], c->exct) + ctl_mul(c->coef_b[0], input);

        c->exct = input;
        c->resp = ctl_sat(c->output, c->out_sto_min, c->out_max);

        c->output = ctl_sat(c->output, c->out_min, c->out_max);

        return c->output;
    }

    // 2 poles 2 zeros controller
    // 2-order IIR filter

    // Programmer View
    // u(n) = a_1 * u(n-1) + a_2 * u(n-2)
    //      + b_1 * e(n-1) + b_2 * e(n-2)
    //      + b_0 * e(n);

    // Math View
    // tex:
    // Recursion
    // $$u_{(n)} = a_1 u_{(n-1)} + a_2 u_{(n-2)}
    //+ b_0 e_{(n)} + b_1 e_{(n-1)} + b_2 e_{(n-2)}$$
    // Complex Frequency Domain Transform Function:
    // $$\frac{U(z)}{E(z)} = \frac{b_2z^{-2} + b_1z^{-1} + b_0}
    //{1-a_2z^{-2} - a_1z^{-1}}$$

    typedef struct _tag_ctrl_2p2z_t
    {
        // u(n)
        ctrl_gt output;

        // coef_a[0]->a_1, coef_a[0]->a_2
        ctrl_gt coef_a[2];
        // coef_b[0]->b_0, coef_b[1]->b_1, coef_b[2]->b_2
        ctrl_gt coef_b[3];
        // resp[0]->u(n-1), resp[1]->u(n-2)
        ctrl_gt resp[2]; // response buffer
        // input->e(n), exct[0]->e(n-1), exct[1]->e(n-2)
        ctrl_gt exct[2]; // excitation buffer

        ctrl_gt out_max;
        ctrl_gt out_sto_min; // generally is -0.9
        ctrl_gt out_min;

    } ctrl_2p2z_t;

    // unit Hz
    void ctl_init_2p2z(ctrl_2p2z_t* ctrl, parameter_gt f_z0, parameter_gt f_z1, parameter_gt f_p1, parameter_gt fs)
    {
        parameter_gt z0 = f_z0 * 2 * pi;
        parameter_gt z1 = f_z1 * 2 * pi;
        parameter_gt p1 = f_p1 * 2 * pi;

        // discrete controller parameter 
        parameter_gt gain_discrete = gain * (1 / 2 / fs / (p1 + 2 * fs));
        parameter_gt b0 = (z1 + 2 * fs) * (z0 + 2 * fs);
        parameter_gt b1 = ((z0 + 2 * fs) * (z1 - 2 * fs) + (z1 + 2 * fs) * (z0 - 2 * fs));
        parameter_gt b2 = (z1 - 2 * fs) * (z0 - 2 * fs);
        parameter_gt a1 = -(4 * fs / (p1 + 2 * fs));
        parameter_gt a2 = (2 * fs - p1) / (2 * fs + p1);
    }

    GMP_STATIC_INLINE
    ctrl_gt ctl_step_2p2z(ctrl_2p2z_t *c, ctrl_gt input)
    {
        c->output = c->coef_a[0] * c->resp[0] + c->coef_a[1] * c->resp[1];
        c->output += c->coef_b[1] * c->exct[0] + c->coef_b[2] * c->exct[1];
        c->output += c->coef_b[0] * input;

        c->exct[1] = c->exct[0];
        c->exct[0] = input;

        c->resp[1] = c->resp[0];
        c->resp[0] = ctl_sat(c->output, c->out_sto_min, c->out_max);

        c->output = ctl_sat(c->output, c->out_min, c->out_max);

        return c->output;
    }

    // 3 poles 3 zeros controller
    // 3-order IIR filter

    // Programmer View
    // u(n) = a_1 * u(n-1) + a_2 * u(n-2) + a_3 * u(n-3)
    //      + b_1 * e(n-1) + b_2 * e(n-2) + b_3 * e(n-3)
    //      + b_0 * e(n);

    // Math View
    // tex:
    // Recursion Formula:
    // $$u_{(n)} = a_1 u_{(n-1)} + a_2 u_{(n-2)} + a_3 u_{(n-3)}
    //+ b_1 e_{(n-1)} + b_2 e_{(n-2)} + b_3 e_{(n-3)} + b_0 e_{(n)} $$
    // Complex Frequency Domain Transform Function:
    // $$\frac{U(z)}{E(z)} = \frac{b_3z^{-3} + b_2z^{-2} + b_1z^{-1} + b_0}
    //{1 - a_3z^{-3} - a_2z^{-2} - a_1z^{-1}}$$

    typedef struct _tag_ctrl_3p3z_t
    {
        // u(n)
        ctrl_gt output;

        // coef_a->a_1
        ctrl_gt coef_a[3];
        // coef_b[0]->b_0, coef_b[1]->b_1
        ctrl_gt coef_b[4];
        // resp->u(n-1)
        ctrl_gt resp[3]; // response buffer
        // input->e(n), exct->e(n-1)
        ctrl_gt exct[3]; // excitation buffer

        ctrl_gt out_max;
        ctrl_gt out_sto_min; // generally is -0.9
        ctrl_gt out_min;

    } ctrl_3p3z_t;

    GMP_STATIC_INLINE
    ctrl_gt ctl_step_3p3z(ctrl_3p3z_t *c, ctrl_gt input)
    {

        c->output = c->coef_a[0] * c->resp[0] + c->coef_a[1] * c->resp[1] + c->coef_a[2] * c->resp[2];
        c->output += c->coef_b[1] * c->exct[0] + c->coef_b[2] * c->exct[1] + c->coef_b[3] * c->exct[2];
        c->output += c->coef_b[0] * input;

        c->exct[2] = c->exct[1];
        c->exct[1] = c->exct[0];
        c->exct[0] = input;

        c->resp[2] = c->resp[1];
        c->resp[1] = c->resp[0];
        c->resp[0] = ctl_sat(c->output, c->out_sto_min, c->out_max);

        c->output = ctl_sat(c->output, c->out_min, c->out_max);

        return c->output;
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FILE_PZ_COMPENSATOR_H_
