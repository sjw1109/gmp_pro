

//////////////////////////////////////////////////////////////////////////
// invoke headers

// system headers
#include <string.h>
#include <stdio.h>

// Core 
#include <core/gmp_core.hpp>

// extensions

// Controller Template Library
#include <ctl/ctl_core.h>

#include <ctl/suite/pmsm/pmsm_speed_closeloop/pmsm_speed_closeloop.h>


//////////////////////////////////////////////////////////////////////////
// global variables here

extern pmsm_ctl_object_t pmsm;


//////////////////////////////////////////////////////////////////////////
// initialize routine here
GMP_NO_OPT_PREFIX
void user_init(void)
GMP_NO_OPT_SUFFIX
{

}


//////////////////////////////////////////////////////////////////////////
// endless loop function here
void user_loop(void)
{
	//Sleep(1);
	controller_state_dispatch((ctl_object_nano_t*)&pmsm);
}
