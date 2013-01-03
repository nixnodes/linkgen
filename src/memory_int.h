/*
 * memory.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef MEMORY_H_
#define MEMORY_H_ 1

#include <common.h>
#include <pthread.h>

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#ifdef IS_BIG_ENDIAN
	#define BS <<=
#else
	#define BS >>=
#endif

#define	STATE_TIMEOUT 60

pthread_mutex_t  tso, tsw;

int thread_safe_write (void *output, void *input, size_t length);

int thread_safe_write_32bit_integer (int *integer, int value);

int wait_for_state (int *state_object, int match_value, time_t timeout);

void *match_data_to_array_block (unsigned char *param, size_t param_length,
				 unsigned char *array_block, size_t block_size, int arb_offset);

int reverse_byte_array
(	byte *buf,
 	size_t len);

uint64 bit_fill
(int count);

uint64 reverse_bits
(uint64 in);

#define FLAG_MALLOC_ONCE 0x00000001

#endif /* MEMORY_H_ */
