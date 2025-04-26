#ifndef __MATH_FUNC_H__
#define __MATH_FUNC_H__

#include "driver/inc/coproc.h"

// 正弦余弦查表计算
extern void Silan_Sin_Cos_PU(_iq Theta, _iq *Sin, _iq *Cos); 

// 开根号查表计算
extern _iq Silan_Sqrt(_iq A);                                

#endif
