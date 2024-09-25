// This file provide the coordinate transformations

#include <ctl/component/common/gmp_math.h>

#ifndef _FILE_COORD_TRANS_H_
#define _FILE_COORD_TRANS_H_


#define GMP_CTL_OUTPUT_TAG

enum ABC_ASIX_ENUM
{
	phase_U = 0,
	phase_V = 1,
	phase_W = 2
};

enum DQ_ASIC_ENUM
{
	phase_D = 0,
	phase_Q = 1
};


// phasor
// dat[0]: sin
// dat[1]: cos


// ABC
// dat[0]: A
// dat[1]: B
// dat[2]: C

// AB
// dat[0]: alpha
// dat[1]: beta
// dat[2]: 0

// dq0
// dat[0]: d
// dat[1]: q
// dat[2]: 0

// 

//extern CORDIC_HandleTypeDef hcordic;

// angle to 2-d phasor
GMP_STATIC_INLINE
void ctl_set_phasor_via_angle(ctrl_gt angle, GMP_CTL_OUTPUT_TAG ctl_vector2_t* phasor)
{
	phasor->dat[0] = ctrl_sin(angle);
	phasor->dat[1] = ctrl_cos(angle);


	//ctrl_gt theta = ctrl_mpy(theta, GMP_CONST_1_OVER_2PI);

	//hcordic.Instance->WDATA = theta << (31 - 24);
	//phasor->dat[0] = (int32_t)(hcordic.Instance->RDATA) >> (31 - 24);
	//phasor->dat[1] = (int32_t)(hcordic.Instance->RDATA) >> (31 - 24);

}

// clark coordinate axes transform
// ABC to alpha-beta
// Constant amplitude transformation
GMP_STATIC_INLINE
void ctl_ct_clark(ctl_vector3_t* abc, GMP_CTL_OUTPUT_TAG ctl_vector3_t* ab)
{
	ctrl_gt k_alpha = GMP_CONST_ABC2AB_ALPHA; // 2/3
	ctrl_gt k_beta = GMP_CONST_ABC2AB_BETA;   // 1/sqrt(3)
	ctrl_gt k_gamma = GMP_CONST_ABC2AB_GAMMA; // 1/3

	//tex:
	// $$i_\alpha = 2/3\times i_a - 1/3 \times i_b - 1/3 \times i_c $$
	ab->dat[0] = ctrl_mpy(k_alpha, abc->dat[0] - ctrl_div2(abc->dat[1] + abc->dat[2]));
	//tex:
	// $$i_\beta = \sqrt{3}/3\times i_b - \sqrt{3}/3\times i_c $$
	ab->dat[1] = ctrl_mpy(k_beta, (abc->dat[1] - abc->dat[2]));
	//tex:
	// $$i_\beta = 1/3\times i_a + 1/3\times i_b + 1/3\times i_c $$
	ab->dat[2] = ctrl_mpy(k_gamma, abc->dat[0] + abc->dat[1] + abc->dat[2]);
}

// clark coordinate axes transform
// AB0(0 means C doesn't measure)
GMP_STATIC_INLINE
void ctl_ct_clark_2ph(ctl_vector3_t* ab0, GMP_CTL_OUTPUT_TAG ctl_vector3_t* ab)
{
	ctrl_gt k_alpha = GMP_CONST_AB02AB_ALPHA;

	//tex:
	// $$i_\alpha = i_a $$
	ab->dat[0] = ab0->dat[0];
	//tex:
	// $$i_\beta = 2/\sqrt{3}\times (i_a/2 + i_b) $$
	ab->dat[1] = ctrl_mpy(ctrl_div2(ab0->dat[0]) + ab0->dat[1], k_alpha);
	//tex:
	// $$ i_0 = 0$$
	// Restricted degrees of freedom
	ab->dat[2] = CTRL_T(0.0f);

}

// park coordinate axes transform
// alpha-beta to dq0
GMP_STATIC_INLINE
void ctl_ct_park(ctl_vector3_t* ab, ctl_vector2_t* phasor, GMP_CTL_OUTPUT_TAG ctl_vector3_t* dq0)
{
	//tex:
	// $$i_d = i_\alpha \times cos\;(\theta) + i_\beta \times sin\;(\theta) $$
	dq0->dat[0] = ctrl_mpy(ab->dat[0], phasor->dat[1]) + ctrl_mpy(ab->dat[1], phasor->dat[0]);
	//tex:
	//$$i_q = - i_\alpha \times sin\;(\theta) + i_\beta \times cos\;(\theta)$$
	dq0->dat[1] = -ctrl_mpy(ab->dat[0], phasor->dat[0]) + ctrl_mpy(ab->dat[1], phasor->dat[1]);
	//tex:
	//$$i_0 = i_0$$
	dq0->dat[2] = ab->dat[2];
}


// ipark coordinate axes transform
// DQ to alpha_beta
GMP_STATIC_INLINE
void ctl_ct_ipark(ctl_vector3_t* dq0, ctl_vector2_t* phasor, GMP_CTL_OUTPUT_TAG ctl_vector3_t* ab)
{
	//tex:
	//$$i_\alpha = i_d \times \cos\;(\theta) - i_q \times \sin\;(\theta) $$
	ab->dat[0] = ctrl_mpy(dq0->dat[0], phasor->dat[1]) - ctrl_mpy(dq0->dat[1], phasor->dat[0]);
	//tex:
	// $$ i_\beta = i_d \times \sin\;(\theta) + i_q \times \cos\;(\theta) $$
	ab->dat[1] = ctrl_mpy(dq0->dat[0], phasor->dat[0]) + ctrl_mpy(dq0->dat[1], phasor->dat[1]);
	//tex:
	//$$i_0 = i_0$$
	ab->dat[2] = dq0->dat[2];
}



#endif // _FILE_COORD_TRANS_H_


