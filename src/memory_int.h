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

pthread_mutex_t tso, tsw;

int
thread_safe_write(void *output, void *input, size_t length);

int
thread_safe_write_32bit_integer(int *integer, int value);

int
wait_for_state(int *state_object, int match_value, time_t timeout);

void *
match_data_to_array_block(unsigned char *param, size_t param_length,
    unsigned char *array_block, size_t block_size, int arb_offset);

int
reverse_byte_array( byte *buf, size_t len);

uint64
bit_fill(int count);

uint64
reverse_bits(uint64 in);

#define FLAG_MALLOC_ONCE 0x00000001

#define F_MDA_REFPTR                    (a32 << 1)
#define F_MDA_FREE                      (a32 << 2)
#define F_MDA_REUSE                     (a32 << 3)
#define F_MDA_WAS_REUSED                (a32 << 4)
#define F_MDA_EOF                       (a32 << 5)
#define F_MDA_FIRST_REUSED              (a32 << 6)
#define F_MDA_ARR_DIST                  (a32 << 7)

#define MDA_MDALLOC_RE                  (a32 << 1)

typedef struct mda_object
{
  void *ptr;
  void *next;
  void *prev;
}*p_md_obj, md_obj;

typedef struct mda_header
{
  p_md_obj objects; /* holds references */
  p_md_obj pos, r_pos, c_pos, first, last;
  off_t offset, r_offset, count, hitcnt, rescnt;
  uint32_t flags;
  void *lref_ptr;
} mda, *pmda;

int
md_init(pmda md, int nm);
p_md_obj
md_first(pmda md);
int
md_g_free(pmda md);
int
md_g_free_cb(pmda md, int
(*cb)(void *));
uintaa_t
md_relink(pmda md);
uintaa_t
md_relink_n(pmda md, off_t base);
p_md_obj
md_first(pmda md);
p_md_obj
md_last(pmda md);
void *
md_swap_s(pmda md, p_md_obj md_o1, p_md_obj md_o2);
void *
md_swap(pmda md, p_md_obj md_o1, p_md_obj md_o2);
void *
md_unlink(pmda md, p_md_obj md_o);
void *
md_alloc(pmda md, int b);
int
md_copy(pmda source, pmda dest, size_t block_sz);
int
is_memregion_null(void *addr, size_t size);

#endif /* MEMORY_H_ */
