
// module dependences
#include <core/std/typedef.h>


// Memory Management of Block 
#ifndef _FILE_BLOCK_MEM_H_
#define _FILE_BLOCK_MEM_H_

#define GMP_MEM_MAGIC_NUMBER	(0xFC6A)
#define GMP_MEM_BLOCK_SIZE		(0x0010)
#define GMP_BASIC_SIZE_PER_BITS (8)  // Basic size of data_element_t


//const uint16_t area_head_size = sizeof(gmp_mem_area_head)/ block_size_unit

typedef struct _tag_gmp_mem_block_head
{
	uint_least16_t magic_number;	// magic number
	gmp_length_t block_index;           // the first block index
	gmp_length_t block_size;            // per block_size_unit    
	//uint32_t block_label;
	struct _tag_gmp_mem_block_head* next;
}gmp_mem_block_head;


typedef struct _tag_gmp_mem_area_head
{
	void* entry;							// memory entry
	gmp_length_t block_size_unit;			// block size per sizeof
	gmp_length_t capacity;					// memory are separated into these parts,per block_size_unit
	gmp_length_t used;						// per block_size_unit
	uint32_t memory_state;					// memory state
	//	gmp_data_element_t* used_flag;		// memory block used flag
	struct _tag_gmp_mem_area_head* next;	// next area
	struct _tag_gmp_mem_block_head* head;	// the first item of mem-block head
	gmp_data_element_t assigned_flag;
}gmp_mem_area_head;



// global variables
extern gmp_stat_t gmp_mem_block_last_errors;

// interface
gmp_mem_area_head* gmp_mem_setup( // return the memory area handle
	void* memory_entry,
	uint32_t memory_size,		    // bytes
	gmp_length_t block_size_unit// = GMP_MEM_BLOCK_SIZE
);


// alloc function 
void* gmp_block_alloc(gmp_mem_area_head* handle,
	gmp_length_t length
);

void gmp_block_free(gmp_mem_area_head* handle, 
	void* ptr
);

#endif  // _FILE_BLOCK_MEM_H_


