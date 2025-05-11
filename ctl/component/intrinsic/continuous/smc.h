// Slide Model Controller

#ifndef _FILE_SMC_H_
#define _FILE_SMC_H_

typedef struct _tag_smc_t_
{
    // input

    // output
    ctrl_gt output;

    // parameters
    ctrl_gt eta11;
    ctrl_gt eta12;
    ctrl_gt eta21;
    ctrl_gt eta22;
    ctrl_gt rho;

    ctrl_gt lambda;

    // intermediate
    ctrl_gt slide;

} ctl_smc_t;

void ctl_init_smc(
    // SMC controller
    ctl_smc_t *smc,
    // SMC controller parameters
    ctrl_gt eta11, ctrl_gt eta12, ctrl_gt eta21, ctrl_gt eta22, ctrl_gt rho,
    // slide model bandwidth
    ctrl_gt lambda);

GMP_STATIC_INLINE
ctrl_gt ctl_step_smc(ctl_smc_t *smc, ctrl_gt input, ctrl_gt input_diff)
{
    ctrl_gt etax1;
    ctrl_gt etax2;
    ctrl_gt rhos;

    smc->slide = ctl_mul(smc->lambda, input) + input_diff;

    if (smc->slide > 0)
    {
        if (input > 0)
            etax1 = ctl_mul(smc->eta11, input);
        else
            etax1 = ctl_mul(smc->eta12, input);

        if (input_diff > 0)
            etax2 = ctl_mul(smc->eta21, input_diff);
        else
            etax2 = ctl_mul(smc->eta22, input_diff);

        rhos = smc->rho;
    }
    else
    {
        if (input > 0)
            etax1 = ctl_mul(smc->eta12, input);
        else
            etax1 = ctl_mul(smc->eta11, input);

        if (input_diff > 0)
            etax2 = ctl_mul(smc->eta22, input_diff);
        else
            etax2 = ctl_mul(smc->eta21, input_diff);

        rhos = -smc->rho;
    }

    smc->output = etax1 + etax2 + rhos;

    return smc->output;
}

#endif // _FILE_SMC_H_
