
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
	gmp_size_t length;
	prot_item_t* items;
}boundary_set_t;

#endif // _FILE_PROTECTION_H_
