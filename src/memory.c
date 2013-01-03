/*
 * memory.c
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <common.h>
#include <memory_int.h>
#include <thread.h>

pthread_mutex_t tsw
		= PTHREAD_MUTEX_INITIALIZER;

void * alloc_mem
(	size_t num,
	size_t size,
	void *ptr,
	byte flags)
{
	errno = 0;

	if ( (flags & FLAG_MALLOC_ONCE)
			&& ptr ) {
		errno = 1062;
		return NULL;
	}

	return calloc (num, size);
}

int thread_safe_write
(void *output, void *input,
		size_t length)
{
	if ( !output || ! input )
		return 0x1;

	pthread_mutex_lock (&tsw);

	memcpy (output, input, length);

	pthread_mutex_unlock (&tsw);

	return 0;
}

int thread_safe_write_32bit_integer
(int *integer, int value)
{
	if ( !integer )
		return 1;

	pthread_mutex_lock (&tsw);

	*integer = value;

	pthread_mutex_unlock (&tsw);

	return 0;
}

int wait_for_state
(int *state_object,
	int match_value,
	time_t timeout)
{
	time_t s = 0, e = 0;
	for ( s = mclock_s(); e - s < timeout;
			e = mclock_s(), usleep(1000) ) {
		if ( *(state_object)
				== match_value ) {
			return 0;
		}
	}

	return 1;
}

void *match_data_to_array_block
(unsigned char *param,
	size_t param_length,
	unsigned char *array_block,
	size_t block_size, int arb_offset)
{
	size_t i, c;

	array_block +=  O_GENH_DHO;

	if ( arb_offset >= 0 ) {
		array_block += arb_offset;
	}

	for ( i = 0; i < (block_size
			- param_length); i++ ) {
		for ( c = 0;
				c < param_length; c++ ) {
			if ( array_block[i + c]
			                 != param[c] ) {
				break;
			}
		}

		if ( c == param_length  ) {
			return (void*) &array_block[i];
		}

		if ( arb_offset >= 0 ) {
			break;
		}
	}
	return 0;
}

int reverse_byte_array
(	byte *buf,
 	size_t len)
{
	int i;
	byte c;

	for ( i = 0; i < len / 2; i++ ) {
		c = buf[i];
		buf[i] = buf[len - i - 1];
		buf[len - i - 1] = c;

	}

	return 0;
}

uint64 reverse_bits
(uint64 in)
{
	uint64 out = 0;
	int i;

	for (i = 0; i < sizeof (uint64) * 8; i++) {
		out <<= 1;
		out |= (in & 1);
		in >>= 1;
	}

	return out;
}

uint64 bit_fill
(int count)
{
	uint64 o = 0;
	int i;

	for ( i = 0; i < count ; i++ ) {
			o |= 1;
			if ( i < (count - 1) )
				o BS 1;
	}
	return o;
}
