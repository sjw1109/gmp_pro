
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>


//////////////////////////////////////////////////////////////////////////
// SVPWM module

#include <ctl/component/motor/svpwm.h>

void init_svpwm(svpwm_channel_t* svpwm)
{
	svpwm->Ualpha = 0;
	svpwm->Ubeta = 0;

	svpwm->pwm_period = 0;

	for (int i = 0; i < 3;++i)
	{
		svpwm->T[i] = 0;
		svpwm->pwm_cmp[i] = 0;
	}
}

void setup_svpwm(svpwm_channel_t* svpwm, pwm_gt pwm_period)
{
	svpwm->pwm_period = pwm_period;

	// re-calculate SVPWM comparator
	svpwm_gen_calc_cmp(svpwm);
}

void svpwm_set_Uab(svpwm_channel_t* svpwm, ctl_vector3_t Uab)
{
	svpwm->Ualpha = Uab.dat[0];
	svpwm->Ubeta = Uab.dat[1];

}
