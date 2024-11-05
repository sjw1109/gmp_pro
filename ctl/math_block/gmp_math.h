/**
 * @file gmp_math.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief
 * @version 0.1
 * @date 2024-09-30
 *
 * @copyright Copyright GMP(c) 2024
 *
 */

// invoke all the GMP features
#include <gmp_core.h>

#ifndef _FILE_GMP_CTL_MATH_H_
#define _FILE_GMP_CTL_MATH_H_

// FIX controller configuration
#if SPECIFY_CTRL_GT_TYPE == USING_FIXED_TI_IQ_LIBRARY
// IQ MATH library

#include <ctl/math_block/ctrl_gt/iqmath_macros.h>

#define ctl_mod_1(A) ((A & ((1 << GLOBAL_Q) - 1)))

#elif SPECIFY_CTRL_GT_TYPE == USING_FIXED_ARM_CMSIS_Q_LIBRARY

// This header has not implemented
#include <ctl/math_block/ctrl_gt/arm_cmsis_macros.h>

#elif SPECIFY_CTRL_GT_TYPE == USING_FLOAT_FPU

#include <ctl/math_block/ctrl_gt/float_macros.h>

#elif SPECIFY_CTRL_GT_TYPE == USING_DOUBLE_FPU

#include <ctl/math_block/ctrl_gt/double_macros.h>

#elif SPECIFY_CTRL_GT_TYPE == USING_QFPLIB_FLOAT

#include <ctl/math_block/ctrl_gt/qfp_float_macros.h>

#else

// FPU controller configuration
#include <ctl/math_block/ctrl_gt/float_macros.h>

#endif

//////////////////////////////////////////////////////////////////////////
// Extension Types
#include <ctl/math_block/vector_lite/vector2.h>
#include <ctl/math_block/vector_lite/vector3.h>

#include <ctl/math_block/matrix_lite/matrix2.h>

// Const variables
#include <ctl/math_block/const/math_ctrl_const.h>
#include <ctl/math_block/const/math_param_const.h>

// Coordinate transform
#include <ctl/math_block/coordinate/coord_trans.h>

#endif // _FILE_GMP_CTL_MATH_H_
