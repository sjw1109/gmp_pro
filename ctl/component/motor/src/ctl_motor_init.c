
#include <core/gmp_core.h>
#include <ctl/ctl_core.h>


//////////////////////////////////////////////////////////////////////////
// SVPWM module

#include <ctl/component/motor/svpwm.h>

void ctl_init_svpwm(svpwm_channel_t* svpwm)
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

void ctl_setup_svpwm(svpwm_channel_t* svpwm, pwm_gt pwm_period)
{
	svpwm->pwm_period = pwm_period;

	// re-calculate SVPWM comparator
	svpwm_gen_calc_cmp(svpwm);
}

//void ctl_svpwm_set_Uab(svpwm_channel_t* svpwm, ctl_vector3_t Uab)
//{
//	svpwm->Ualpha = Uab.dat[0];
//	svpwm->Ubeta = Uab.dat[1];
//
//}

//////////////////////////////////////////////////////////////////////////
// Encoder module

#include <ctl/component/motor/encoder.h>

void ctl_init_pos_encoder(ctl_pos_encoder_t* pos_encoder)
{
	pos_encoder->position = 0;
	pos_encoder->offset = 0;
	pos_encoder->poles = 0;
}

void ctl_init_spd_encoder(ctl_spd_encoder_t* spd_encoder)
{
	spd_encoder->speed_base = 3000;
	spd_encoder->speed = 0;
	spd_encoder->speed_krpm = 0;
}

