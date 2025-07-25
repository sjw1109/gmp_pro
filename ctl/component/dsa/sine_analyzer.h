// This file provide a struct for calculate RMS, average and frequency of Sine wave.
// It is recommended to use this module when ctr_gt type is floating point, otherwise it may cause calculation errors
//

#ifndef _FILE_SINE_ANALYZER_H_
#define _FILE_SINE_ANALYZER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef struct _tag_sine_analyzer
{
    //
    // input variables
    //

    //
    // output
    //

    // RMS Value
    ctrl_gt wave_rms;

    // Average Value
    ctrl_gt wave_avg;

    // Signal Freq
    ctrl_gt ac_freq_pu;

    // Zero cross detected
    fast_gt zero_cross;

    //
    // parameters
    //

    // Zero crossing criterion
    ctrl_gt threshold;

    //
    // intermediate variables
    //

    // absolute voltage
    ctrl_gt wave_norm;

    // frequency scale factor
    ctrl_gt freq_sf;

    // previous sign of wave, to detect zero cross
    fast_gt prev_sign;

    // current sign of wave, to detect zero cross
    fast_gt curr_sign;

    // input sum up
    ctrl_gt sumup;

    // input square sum up
    ctrl_gt sumup_sqr;

    // inverse sample
    ctrl_gt inv_index;

    // inverse sample square
    ctrl_gt inv_index_sqrt;

    // current sample index
    fast32_gt sample_index;

    // sample range min
    fast32_gt sample_min;

    // sample range max
    fast32_gt sample_max;

    // jitter counter
    fast32_gt jitter_counter;

} sine_analyzer_t;

GMP_STATIC_INLINE
void ctl_sine_clear_analyzer(sine_analyzer_t *sine)
{
    sine->zero_cross = 0;
    sine->wave_avg = 0;
    sine->wave_rms = 0;
    sine->ac_freq_pu = 0;

    sine->sample_index = 0;
    sine->sumup = 0;
    sine->sumup_sqr = 0;
}

GMP_STATIC_INLINE
ctrl_gt ctl_step_sine_analyzer(sine_analyzer_t *sine, ctrl_gt wave_input)
{
    sine->wave_norm = ctl_abs(wave_input);

    sine->curr_sign = (wave_input > sine->threshold) ? 1 : 0;
    sine->sample_index++;
    sine->sumup = sine->sumup + sine->wave_norm;
    sine->sumup_sqr = sine->sumup_sqr + (sine->wave_norm * sine->wave_norm);
    sine->zero_cross = 0;

    // update last sign
    sine->prev_sign = sine->curr_sign;

    // judge if a whole cycle is complete
    if (sine->prev_sign != sine->curr_sign && sine->curr_sign == 1)
    {
        // a whole cycle
        if (sine->sample_index > sine->sample_min)
        {
            sine->zero_cross = 1;

            sine->inv_index = float2ctrl(1.0f / sine->sample_index);
            sine->inv_index_sqrt = ctl_sqrt(sine->inv_index);

            sine->wave_avg = ctl_mul(sine->inv_index, sine->sumup);
            sine->wave_rms = ctl_mul(ctl_sqrt(sine->sumup_sqr), sine->inv_index_sqrt);
            sine->ac_freq_pu = ctl_mul(sine->freq_sf, sine->inv_index);

            sine->sample_index = 0;
            sine->sumup = 0;
            sine->sumup_sqr = 0;
        }

        // jitter case
        else
        {
            if (sine->jitter_counter <= 30)
            {
                sine->jitter_counter += 1;
            }

            sine->sample_index = 0;
        }
    }

    // Special case: no valid wave is detected.
    // protection scene
    if (sine->sample_index > sine->sample_max || sine->jitter_counter > 20)
    {
        ctl_sine_clear_analyzer(sine);
    }

    // update prev sign
    sine->prev_sign = sine->curr_sign;

    return sine->wave_rms;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_sa_rms(sine_analyzer_t *sine)
{
    return sine->wave_rms;
}

GMP_STATIC_INLINE
ctrl_gt ctl_get_sa_avg(sine_analyzer_t *sine)
{
    return sine->wave_avg;
}

void ctl_init_sine_analyzer(
    // handle of sine analyzer
    sine_analyzer_t *sine,
    // zero cross detect threshold
    parameter_gt zcd_threshold,
    // grid maximum frequency, grid minimum frequency
    parameter_gt min_freq, parameter_gt max_freq,
    // rated frequency
    parameter_gt rated_freq,
    // controller frequency
    parameter_gt f_ctrl);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _FILE_SINE_ANALYZER_H_
