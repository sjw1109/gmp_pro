
#include <core/gmp_core.h>
#include <core/util/ds/data_ring_buffer.h>


gmp_stat_t init_ring_buffer(
	gmp_ring_buffer_t** buffer,
	gmp_size_t length
)
{
	// check variables
	assert(buffer);

	*buffer = gmp_malloc(length * sizeof(gmp_data_t) + sizeof(buffer));

	// initialize ring buffer
	(*buffer)->buffer = (gmp_data_t*)((*buffer) + 1);
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

gmp_size_t rb_size(
	gmp_ring_buffer_t* buf
)
{
	if (buf->write_pos >= buf->read_pos)
		return buf->write_pos - buf->read_pos;
	else
		return buf->read_pos + buf->length - buf->write_pos;
}

gmp_size_t rb_capacity(
	gmp_ring_buffer_t* buf
)
{
	return buf->length;
}

gmp_stat_t rb_push(
	gmp_ring_buffer_t* buf,
	gmp_data_t dat
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

gmp_data_t rb_peek(
	gmp_ring_buffer_t* buf
)
{
	return buf->buffer[buf->read_pos];
}

gmp_data_t rb_pop(
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

gmp_size_t rb_extract(
	gmp_ring_buffer_t* buf,
	void* dst,
	gmp_size_t cap
)
{
	gmp_size_t size = rb_size(buf);
	gmp_size_t cur_pos;
	gmp_size_t cnt;

	if (size == 0)
		return 0;

	if (buf->read_pos < buf->write_pos)
	{
		for (cur_pos = buf->read_pos, cnt = 0;
			cur_pos < buf->length && cnt < cap && cur_pos < buf->write_pos;
			++cur_pos, ++cnt)
		{
			((gmp_data_t*)dst)[cnt] = buf->buffer[cur_pos];
		}
		
		buf->read_pos = cur_pos;
	}
	else
	{
		for (cur_pos = buf->read_pos, cnt = 0;
			cur_pos < buf->length && cnt < cap;
			++cur_pos, ++cnt)
		{
			((gmp_data_t*)dst)[cnt] = buf->buffer[cur_pos];
		}

		for (cur_pos = 0;
			cur_pos < buf->write_pos && cnt < cap;
			++cur_pos, ++cnt)
		{
			((gmp_data_t*)dst)[cnt] = buf->buffer[cur_pos];
		}

		buf->read_pos = cur_pos;
	}

	return cnt;
}
