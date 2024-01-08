#ifndef _FILE_MC_FEEDFORWARD_H_
#define _FILE_MC_FEEDFORWARD_H_

typedef struct _tag_mc_std_ff_t
{
	// out
	gmp_math_t out;

	// variables
	// given static friction 
	gmp_math_t Ks;
	// given velocity
	gmp_math_t Kv;
	// given acceleration or torque 
	gmp_math_t Kt;
}mc_std_ff_t;

inline void gmp_mc_std_ff_t(
	mc_std_ff_t *ff,
	gmp_math_t spd,		// given speed
	gmp_math_t torq		// given torque
)
{
	ff->out = ff->Ks * gmp_sgn(spd);
	ff->out += ff->Kv * spd + ff->Kt * torq;
};

#endif // _FILE_FEED_FORWARD_H_
