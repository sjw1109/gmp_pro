

#ifndef _FILE_PROPORTIONAL_RESONANT_CONTROLLER_H_
#define _FILE_PROPORTIONAL_RESONANT_CONTROLLER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

// clang-format off
// 

// Proportional Resonant Control
//tex:
// $$
// G_{QPR} = K_p +K_r \frac{2s}{s^2 + \omega_r^2}
// $$

// Discrete PR controller
//
//tex:
// $$
// G_{PR} = k_p + k_r \frac{4f_s}{4f_s^2+\omega_r^2} \cdot \frac{1-z^{-2}}{1-2\frac{4f_s^2-\omega_r^2}{4f_s^2+\omega_r^2}z^{-1}+z^{-2}}
// $$
    
//
// clang-format on

typedef struct _tag_ctl_pr_controller
{
    //
    // input section
    //

    //
    // output section
    //

    // controller output
    ctrl_gt output;

    //
    // intermediate section
    //

    ctrl_gt input_1;

    ctrl_gt input_2;

    ctrl_gt output_1;

    ctrl_gt output_2;

    //
    // parameter section
    //

    // gain of whole controller
    ctrl_gt kpg;

    // gain of resonant frequency
    // this is discrete gain, not the continuous controller gain.
    ctrl_gt krg;

    // resonant coefficient
    ctrl_gt kr;

} pr_ctrl_t;

void ctl_init_pr_controller(
    // handle of PR controller
    pr_ctrl_t *pr,
    // Kp
    parameter_gt kp,
    // gain of resonant frequency
    parameter_gt kr,
    // resonant frequency, unit Hz
    parameter_gt freq_resonant,
    // controller frequency
    parameter_gt fs);

GMP_STATIC_INLINE
void ctl_clear_pr_controller(
    // handle of PR controller
    pr_ctrl_t *pr)
{
    pr->output = 0;
    pr->output_2 = 0;
    pr->output_1 = 0;

    pr->input_2 = 0;
    pr->input_1 = 0;
}

GMP_STATIC_INLINE
ctrl_gt ctl_step_pr_controller(
    // handle of PR controller
    pr_ctrl_t *pr,
    // input of PR controller
    ctrl_gt input)
{
    ctrl_gt output = 0;
    ctrl_gt input_diff = 0;

    // P controller for PR controller
    output = ctl_mul(pr->kpg, input);

    // R controller for PR controller
    input_diff = input - pr->input_2;
    output += ctl_mul(pr->krg, input_diff) + ctl_mul(pr->kr, pr->output_1) - pr->output_2;

    // move to next position
    pr->input_2 = pr->input_1;
    pr->input_1 = input;

    pr->output_2 = pr->output_1;
    pr->output_1 = output;
    pr->output = output;

    return output;
}

// clang-format off
// 

// Quasi-Proportional Resonant Control
//tex:
// $$
// G_{PR} = K_p +K_r \frac{2\omega_c s}{s^2 + 2\omega_c s + \omega_r^2}
// $$

// Discrete model of QPR controller

//tex:
//$$
//G_{QPR}=\mathrm{k}_{p}+k_{r}\frac{4\omega_{c}f_{s}}{4f_{s}^{2}+4\omega_{c}f_{s}
//+\omega_{r}^{2}}\frac{1-z^{-2}}{1-2\frac{4f_{s}^{2}-\omega_{r}^{2}}{4f_{s}^{2}
//+4\omega_{c}f_{s}+\omega_{r}^{2}}z^{-1}
//+\frac{4f_{s}^{2}-4\omega_{c}f_{s}+\omega_{r}^{2}}
//{4f_{s}^{2}+4\omega_{c}f_{s}+\omega_{r}^{2}}z^{-2}}
//$$
//

//
// clang-format on

typedef struct _tag_ctl_qpr_controller
{
    //
    // input section
    //

    //
    // output section
    //

    // controller output
    ctrl_gt output;

    //
    // intermediate section
    //

    ctrl_gt input_1;

    ctrl_gt input_2;

    ctrl_gt output_1;

    ctrl_gt output_2;

    //
    // parameter section
    //

    ctrl_gt kr;
    ctrl_gt kp;

    ctrl_gt b1;
    ctrl_gt b2;
    ctrl_gt a0;

} qpr_ctrl_t;

GMP_STATIC_INLINE
void ctl_clear_qpr_controller(
    // handle of QPR controller
    qpr_ctrl_t *qpr)
{
    qpr->output = 0;
    qpr->output_2 = 0;
    qpr->output_1 = 0;

    qpr->input_2 = 0;
    qpr->input_1 = 0;
}

void ctl_init_qpr_controller(
    // handle of QPR controller
    qpr_ctrl_t *qpr,
    // Kp
    parameter_gt kp,
    // gain of resonant frequency
    parameter_gt kr,
    // resonant frequency, unit Hz
    parameter_gt freq_resonant,
    // cut frequency, unit Hz
    parameter_gt freq_cut,
    // controller frequency
    parameter_gt fs);

GMP_STATIC_INLINE
ctrl_gt ctl_step_qpr_controller(
    // handle of QPR controller
    qpr_ctrl_t *pr,
    // input
    ctrl_gt input)
{
    ctrl_gt output = 0;
    ctrl_gt input_diff = 0;

    // P controller for PR controller
    output = ctl_mul(qpr->kpg, input);

    // R controller for QPR controller
    input_diff = input - qpr->input_2;
    output += ctl_mul(ctl_mul(qpr->a0, input_diff), qpr->kr);
    output += ctl_mul(qpr->b1, qpr->output_1);
    output -= ctl_mul(qpr->b2, qpr->output_2);

    // move to next position
    qpr->input_2 = qpr->input_1;
    qpr->input_1 = input;

    qpr->output_2 = qpr->output_1;
    qpr->output_1 = output;
    qpr->output = output;

    return output;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_PROPORTIONAL_RESONANT_CONTROLLER_H_
