
// Memory Management
#include <core/mm/block_mem.h>

#ifndef _FILE_MEM_H_
#define _FILE_MEM_H_


extern gmp_mem_area_head* default_mem_heap;

static inline void* gmp_malloc(gmp_length_t size) // byte
{
	return gmp_block_alloc(default_mem_heap, size);
}

static inline void gmp_free(void* ptr)
{
	gmp_block_free(default_mem_heap, ptr);
}

#endif
