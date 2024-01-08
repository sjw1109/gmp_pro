
#ifndef _FILE_BUFFER_H_
#define _FILE_BUFFER_H_

// ring buffer
typedef struct _tag_gmp_ring_buffer_t
{
	gmp_data_t* buffer;		// ring of buffer
	gmp_size_t length;		// buffer length

	gmp_size_t write_pos;   // write position
	gmp_size_t read_pos;    // read position
}gmp_ring_buffer_t;



#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

	// initialization
	gmp_stat_t init_ring_buffer(
	gmp_ring_buffer_t** buffer,
	gmp_size_t length
	);

	// release the ring buffer
	void release_ring_buffer(
		gmp_ring_buffer_t* buf
	);

	gmp_size_t rb_size(
		gmp_ring_buffer_t* buf
	);

	gmp_size_t rb_capacity(
		gmp_ring_buffer_t* buf
	);

	gmp_stat_t rb_push(
		gmp_ring_buffer_t* buf,
		gmp_data_t dat
	);

	gmp_data_t rb_peek(
		gmp_ring_buffer_t* buf
	);

	gmp_data_t rb_pop(
		gmp_ring_buffer_t* buf
	);

	void rb_empty(
		gmp_ring_buffer_t* buf
	);

	gmp_size_t rb_extract(
		gmp_ring_buffer_t* buf,
		void* dst,
		gmp_size_t cap
	);
	

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_FILE_BUFFER_H_
