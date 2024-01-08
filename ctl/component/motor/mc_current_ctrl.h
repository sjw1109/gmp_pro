#include <ctrl/component/common/pi.h>

#ifndef _FILE_MC_CURRENT_CTRL_H_
#define _FILE_MC_CURRENT_CTRL_H_

//////////////////////////////////////////////////////////////////////////
// standard PID controller
typedef struct _tag_mc_std_current_ctrl_t
{
	pid_regular_t pid;

}mc_std_current_ctrl_t;



#endif // _FILE_MC_CURRENT_CTRL_H_
