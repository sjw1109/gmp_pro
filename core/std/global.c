
// Module dependences
#include <core/std/typedef.h>

// global definitions
#include <core/std/global.h>

// This function should be called in `main` after initialize routine.
// This function should implement by user to setup all the peripherals of the chip.
void gmp_entry(void)
{
	// initialize peripheral
	gmp_setup_peripheral();

	// initialize peripheral tree
	gmp_init_peripheral_tree();

	// user initialize section
	gmp_init();

	// GMP test bench setup
	gmp_test_bench();

	// take over `main loop`
#ifdef GMP_WINDOWS
	gmp_loop();
#else
	while (1) gmp_loop();
#endif
	return;
}


void gmp_init()
{
	// Setup GMP library


	// Call user initialize
	user_init();
}

void gmp_loop()
{
	user_loop();
}

void gmp_setup_peripheral()
{
}


void gmp_init_peripheral_tree()
{
	//gmp_peripheral_t tmp_dev = GMP_IIC_1;
	//// TODO check capacity
	//gmp_iic1_hdev = &gmp_iic_test_mem;
	//memcpy(gmp_iic1_hdev, &tmp_dev,sizeof(gmp_peripheral_t));
	// TODO implement extensions

}

WEAK_FUNCTION void gmp_test_bench()
{
	
}

