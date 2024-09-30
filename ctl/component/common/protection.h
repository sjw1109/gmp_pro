/**
 * @file protection.h
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
#ifndef _FILE_PROTECTION_H_
#define _FILE_PROTECTION_H_

typedef struct _tag_protection_item_t
{
	gmp_math_t* items;
	gmp_math_t infimum;
	gmp_math_t supremum;
}prot_item_t;


typedef struct _tag_boundary_set_t
{
	size_gt length;
	prot_item_t* items;
}boundary_set_t;

#endif // _FILE_PROTECTION_H_
