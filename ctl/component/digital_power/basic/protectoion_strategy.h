// This is the standard protection strategy for DC/DC converter


#ifndef _FILE_PROTECTION_STRATEGY_H_ 
#define _FILE_PROTECTION_STRATEGY_H_

// Standard GMP CTL protection has the following three stage
// + Brickwall overcurrent protection
//   When Io < ICL nothing happened.
//   When Io > ICL getting into foldback mode.
// + Foldback overcurrent protection
// 
// + overpower protection
// 
typedef struct _tag_std_current_protection_type
{
	
} std_current_protection_t;




#endif // _FILE_PROTECTION_STRATEGY_H_
