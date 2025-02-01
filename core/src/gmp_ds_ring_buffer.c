/**
 * @file data_ring_buffer.c
 * @author Javnson (javnson@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2024-09-30
 * 
 * @copyright Copyright GMP(c) 2024
 * 
 */
 
#include <gmp_core.h>
#include <core/util/ds/data_ring_buffer.h>


gmp_stat_t init_ring_buffer(
	gmp_ring_buffer_t** buffer,
	size_gt length
)
{
	// check variables
	assert(buffer);

	*buffer = gmp_malloc(length * sizeof(data_gt) + sizeof(buffer));

	// initialize ring buffer
	(*buffer)->buffer = (data_gt*)((*buffer) + 1);
	(*buffer)->length = length;
	(*buffer)->write_pos = 0;
	(*buffer)->read_pos = 0;

	// return malloc state
	return gmp_mem_block_last_errors;
}

void release_ring_buffer(
	gmp_ring_buffer_t* buf
)
{
	gmp_free(buf->buffer);
}

size_gt rb_size(
	gmp_ring_buffer_t* buf
)
{
	if (buf->write_pos >= buf->read_pos)
		return buf->write_pos - buf->read_pos;
	else
		return buf->read_pos + buf->length - buf->write_pos;
}

size_gt rb_capacity(
	gmp_ring_buffer_t* buf
)
{
	return buf->length;
}

gmp_stat_t rb_push(
	gmp_ring_buffer_t* buf,
	data_gt dat
)
{
	buf->buffer[buf->write_pos++] = dat;
	if (buf->write_pos == buf->length)
		buf->write_pos = 0;

	// overflow
	if (buf->write_pos == buf->read_pos)
	{
		buf->read_pos += 1;
		return GMP_STAT_OVERFLOW;
	}

	return GMP_STAT_OK;
}

data_gt rb_peek(
	gmp_ring_buffer_t* buf
)
{
	return buf->buffer[buf->read_pos];
}

data_gt rb_pop(
	gmp_ring_buffer_t* buf
)
{
	if (buf->read_pos != buf->write_pos)
	{
		if (buf->read_pos == buf->length)
			buf->read_pos = 0;
		else
			buf->read_pos += 1;
	}
	return buf->buffer[buf->read_pos];
}

void rb_empty(
	gmp_ring_buffer_t* buf
)
{
	buf->write_pos = 0;
	buf->read_pos = 0;
}

size_gt rb_extract(
	gmp_ring_buffer_t* buf,
	void* dst,
	size_gt cap
)
{
	size_gt size = rb_size(buf);
	size_gt cur_pos;
	size_gt cnt;

	if (size == 0)
		return 0;

	if (buf->read_pos < buf->write_pos)
	{
		for (cur_pos = buf->read_pos, cnt = 0;
			cur_pos < buf->length && cnt < cap && cur_pos < buf->write_pos;
			++cur_pos, ++cnt)
		{
			((data_gt*)dst)[cnt] = buf->buffer[cur_pos];
		}
		
		buf->read_pos = cur_pos;
	}
	else
	{
		for (cur_pos = buf->read_pos, cnt = 0;
			cur_pos < buf->length && cnt < cap;
			++cur_pos, ++cnt)
		{
			((data_gt*)dst)[cnt] = buf->buffer[cur_pos];
		}

		for (cur_pos = 0;
			cur_pos < buf->write_pos && cnt < cap;
			++cur_pos, ++cnt)
		{
			((data_gt*)dst)[cnt] = buf->buffer[cur_pos];
		}

		buf->read_pos = cur_pos;
	}

	return cnt;
}
