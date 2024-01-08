
#ifndef FILE_PZ_COMPENSATOR_H_
#define FILE_PZ_COMPENSATOR_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

//tex:
// Bilinear transform:
// $$s = \frac{2}{T} \frac{1-z^{-1}}{1+z^{-1}}$$ 
// 
// $$z = \frac{1+sT/2}{1-sT/2}$$

// 1 pole 1 zero controller
// 1-order IIR filter

// Programmer View
// u(n) = a_1 * u(n-1) + b_1 * e(n-1)
//      + b_0 * e(n);

// Math View
//tex:
// Recursion Formula:
// $$u_{(n)} = a_1 u_{(n-1)} + b_1 e_{(n-1)} + b_0 e_{(n)} $$
// Complex Frequency Domain Transform Function:
// $$\frac{U(z)}{E(z)} = \frac{b_1 z^{-1}+b_0}{1-a_1 z^{-1}}$$

typedef struct _tag_ctrl_1p1z_t
{
	// u(n)
	gmp_math_t output;

	// coef_a->a_1
	gmp_math_t coef_a;
	// coef_b[0]->b_0, coef_b[1]->b_1
	gmp_math_t coef_b[2];
	// resp->u(n-1)
	gmp_math_t resp;      // response buffer
	// input->e(n), exct->e(n-1)
	gmp_math_t exct;      // excitation buffer

	gmp_math_t out_max;
	gmp_math_t out_sto_min;  // generally is -0.9
	gmp_math_t out_min;

}ctrl_1p1z_t;

inline void gmp_1p1z(ctrl_1p1z_t* c, gmp_math_t input)
{
	c->output = c->coef_a * resp + coef_b[1] * exct + coef[0] * input;

	c->exct = input;
	c->resp = gmp_math_sat(c->output, c->out_sto_min, c->out_max);

	c->output = gmp_math_sat(c->output, c->out_min, c->out_max);
}

// 2 poles 2 zeros controller
// 2-order IIR filter

// Programmer View
// u(n) = a_1 * u(n-1) + a_2 * u(n-2) 
//      + b_1 * e(n-1) + b_2 * e(n-2)
//      + b_0 * e(n);

// Math View
//tex:
// Recursion 
// $$u_{(n)} = a_1 u_{(n-1)} + a_2 u_{(n-2)} 
//+ b_0 e_{(n)} + b_1 e_{(n-1)} + b_2 e_{(n-2)}$$
// Complex Frequency Domain Transform Function:
// $$\frac{U(z)}{E(z)} = \frac{b_2z^{-2} + b_1z^{-1} + b_0}
//{1-a_2z^{-2} - a_1z^{-1}}$$


typedef struct _tag_ctrl_2p2z_t
{
	// u(n)
	gmp_math_t output;

	// coef_a[0]->a_1, coef_a[0]->a_2
	gmp_math_t coef_a[2];
	// coef_b[0]->b_0, coef_b[1]->b_1, coef_b[2]->b_2
	gmp_math_t coef_b[3];
	// resp[0]->u(n-1), resp[1]->u(n-2)
	gmp_math_t resp[2];      // response buffer
	// input->e(n), exct[0]->e(n-1), exct[1]->e(n-2)
	gmp_math_t exct[2];      // excitation buffer

	gmp_math_t out_max;
	gmp_math_t out_sto_min;  // generally is -0.9
	gmp_math_t out_min;

}ctrl_2p2z_t;

inline void gmp_2p2z(ctrl_2p2z_t* c, gmp_math_t input)
{
	c->output = c->coef_a[0] * c->resp[0] + c->coef_a[1] * c->resp[1];
	c->output += c->coef_b[1] * c->exct[0] + c->coef_b[2] * c->exct[1];
	c->output += c->coef_b[0] * input;

	c->exct[1] = c->exct[0];
	c->exct[0] = input;

	c->resp[1] = c->resp[0];
	c->resp[0] = gmp_math_sat(c->output, c->out_sto_min, c->out_max);

	c->output = gmp_math_sat(c->output, c->out_min, c->out_max);
}

// 3 poles 3 zeros controller
// 3-order IIR filter

// Programmer View
// u(n) = a_1 * u(n-1) + a_2 * u(n-2) + a_3 * u(n-3)
//      + b_1 * e(n-1) + b_2 * e(n-2) + b_3 * e(n-3)
//      + b_0 * e(n);

// Math View
//tex:
// Recursion Formula:
// $$u_{(n)} = a_1 u_{(n-1)} + a_2 u_{(n-2)} + a_3 u_{(n-3)} 
//+ b_1 e_{(n-1)} + b_2 e_{(n-2)} + b_3 e_{(n-3)} + b_0 e_{(n)} $$
// Complex Frequency Domain Transform Function:
// $$\frac{U(z)}{E(z)} = \frac{b_3z^{-3} + b_2z^{-2} + b_1z^{-1} + b_0}
//{1 - a_3z^{-3} - a_2z^{-2} - a_1z^{-1}}$$

typedef struct _tag_ctrl_3p3z_t
{
	// u(n)
	gmp_math_t output;

	// coef_a->a_1
	gmp_math_t coef_a[3];
	// coef_b[0]->b_0, coef_b[1]->b_1
	gmp_math_t coef_b[4];
	// resp->u(n-1)
	gmp_math_t resp[3];		// response buffer
	// input->e(n), exct->e(n-1)
	gmp_math_t exct[3];		// excitation buffer

	gmp_math_t out_max;
	gmp_math_t out_sto_min;  // generally is -0.9
	gmp_math_t out_min;

}ctrl_3p3z_t;

inline void gmp_3p3z(ctrl_3p3z_t* c, gmp_math_t input)
{

	c->output = c->coef_a[0] * c->resp[0] + c->coef_a[1] * c->resp[1]
		+ c->coef_a[2] * c->resp[2];
	c->output += c->coef_b[1] * c->exct[0] + c->coef_b[2] * c->exct[1]
		+ c->coef_b[3] * c->exct[2];
	c->output += c->coef_b[0] * input;

	c->exct[2] = c->exct[1];
	c->exct[1] = c->exct[0];
	c->exct[0] = input;

	c->resp[2] = c->resp[1];
	c->resp[1] = c->resp[0];
	c->resp[0] = gmp_math_sat(c->output, c->out_sto_min, c->out_max);

	c->output = gmp_math_sat(c->output, c->out_min, c->out_max);
}



#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FILE_PZ_COMPENSATOR_H_
