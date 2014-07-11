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

pthread_mutex_t tsw = PTHREAD_MUTEX_INITIALIZER;

void *
alloc_mem(size_t num, size_t size, void *ptr,
byte flags)
{
  errno = 0;

  if ((flags & FLAG_MALLOC_ONCE) && ptr)
    {
      errno = 1062;
      return NULL;
    }

  return calloc(num, size);
}

int
thread_safe_write(void *output, void *input, size_t length)
{
  if (!output || !input)
    return 0x1;

  pthread_mutex_lock(&tsw);

  memcpy(output, input, length);

  pthread_mutex_unlock(&tsw);

  return 0;
}

int
thread_safe_write_32bit_integer(int *integer, int value)
{
  if (!integer)
    return 1;

  pthread_mutex_lock(&tsw);

  *integer = value;

  pthread_mutex_unlock(&tsw);

  return 0;
}

int
wait_for_state(int *state_object, int match_value, time_t timeout)
{
  time_t s = 0, e = 0;
  for (s = mclock_s(); e - s < timeout; e = mclock_s(), usleep(1000))
    {
      if (*(state_object) == match_value)
        {
          return 0;
        }
    }

  return 1;
}

void *
match_data_to_array_block(unsigned char *param, size_t param_length,
    unsigned char *array_block, size_t block_size, int arb_offset)
{
  size_t i, c;

  array_block += O_GENH_DHO;

  if (arb_offset >= 0)
    {
      array_block += arb_offset;
    }

  for (i = 0; i < (block_size - param_length); i++)
    {
      for (c = 0; c < param_length; c++)
        {
          if (array_block[i + c] != param[c])
            {
              break;
            }
        }

      if (c == param_length)
        {
          return (void*) &array_block[i];
        }

      if (arb_offset >= 0)
        {
          break;
        }
    }
  return 0;
}

int
reverse_byte_array( byte *buf, size_t len)
{
  int i;
  byte c;

  for (i = 0; i < len / 2; i++)
    {
      c = buf[i];
      buf[i] = buf[len - i - 1];
      buf[len - i - 1] = c;

    }

  return 0;
}

uint64
reverse_bits(uint64 in)
{
  uint64 out = 0;
  int i;

  for (i = 0; i < sizeof(uint64) * 8; i++)
    {
      out <<= 1;
      out |= (in & 1);
      in >>= 1;
    }

  return out;
}

uint64
bit_fill(int count)
{
  uint64 o = 0;
  int i;

  for (i = 0; i < count; i++)
    {
      o |= 1;
      if (i < (count - 1))
        o BS 1;
    }
  return o;
}

int
md_init(pmda md, int nm)
{
  if (!md || md->objects)
    {
      return 1;
    }
  bzero(md, sizeof(mda));
  md->objects = calloc(nm, sizeof(md_obj));
  md->count = nm;
  md->pos = md->objects;
  md->r_pos = md->objects;
  md->first = md->objects;
  return 0;
}

int
md_g_free(pmda md)
{
  if (!md || !md->objects)
    return 1;

  if (!(md->flags & F_MDA_REFPTR))
    {
      p_md_obj ptr = md_first(md), ptr_s;
      while (ptr)
        {
          ptr_s = ptr->next;
          if (ptr->ptr)
            {
              free(ptr->ptr);
              ptr->ptr = NULL;
            }
          ptr = ptr_s;
        }
    }

  free(md->objects);
  bzero(md, sizeof(mda));

  return 0;
}

int
md_g_free_cb(pmda md, int
(*cb)(void *))
{
  if (!md || !md->objects)
    return 1;

  if (!(md->flags & F_MDA_REFPTR))
    {
      p_md_obj ptr = md_first(md), ptr_s;
      while (ptr)
        {
          cb(ptr->ptr);
          ptr_s = ptr->next;
          if (ptr->ptr)
            {
              free(ptr->ptr);
              ptr->ptr = NULL;
            }
          ptr = ptr_s;
        }
    }

  free(md->objects);
  bzero(md, sizeof(mda));

  return 0;
}

p_md_obj
md_first(pmda md)
{

  /*if (md->first && md->first != md->objects) {
   if (md->first->ptr) {
   return md->first;
   }
   }*/

  off_t off = 0;
  p_md_obj ptr = md->objects;

  for (off = 0; off < md->count; off++, ptr++)
    {
      if (ptr->ptr)
        {
          return ptr;
        }
    }

  return NULL;
}

p_md_obj
md_last(pmda md)
{
  p_md_obj ptr = md_first(md);

  if (!ptr)
    {
      return ptr;
    }

  while (ptr->next)
    {
      ptr = ptr->next;
    }

  return ptr;
}

void *
md_alloc(pmda md, int b)
{
  uint32_t flags = 0;

  if (md->offset >= md->count)
    {

      md->objects = realloc(md->objects, (md->count * sizeof(md_obj)) * 2);
      md->pos = md->objects;
      md->pos += md->count;
      bzero(md->pos, md->count * sizeof(md_obj));

      md->count *= 2;
      uintaa_t rlc;

      if (md->flags & F_MDA_ARR_DIST)
        {
          rlc = md_relink_n(md, 100);
        }
      else
        {
          rlc = md_relink(md);
        }
      flags |= MDA_MDALLOC_RE;

    }

  p_md_obj prev = md->pos;
  uintaa_t pcntr = 0;
  while (md->pos->ptr
      && (pcntr = ((md->pos - md->objects) / sizeof(md_obj))) < md->count)
    {
      md->pos++;
    }

  if (pcntr >= md->count)
    {
      return NULL;
    }

  if (md->pos > md->objects && !(md->pos - 1)->ptr)
    {
      flags |= MDA_MDALLOC_RE;
    }

  if (md->pos->ptr)
    return NULL;

  if (md->flags & F_MDA_REFPTR)
    {
      md->pos->ptr = md->lref_ptr;
    }
  else
    {
      md->pos->ptr = calloc(1, b);
    }

  if (prev != md->pos)
    {
      prev->next = md->pos;
      md->pos->prev = prev;
    }

  md->offset++;

  if (flags & MDA_MDALLOC_RE)
    {
      if (md->flags & F_MDA_ARR_DIST)
        {
          md_relink_n(md, 100);
        }
      else
        {
          md_relink(md);
        }
    }

  return md->pos->ptr;
}

void *
md_unlink(pmda md, p_md_obj md_o)
{
  if (!md_o)
    {
      return NULL;
    }

  p_md_obj c_ptr = NULL;

  if (md_o->prev)
    {
      ((p_md_obj) md_o->prev)->next = (p_md_obj) md_o->next;
      c_ptr = md_o->prev;
    }

  if (md_o->next)
    {
      ((p_md_obj) md_o->next)->prev = (p_md_obj) md_o->prev;
      c_ptr = md_o->next;
    }

  md->offset--;
  if (md->pos == md_o && c_ptr)
    {
      md->pos = c_ptr;
    }
  if (!(md->flags & F_MDA_REFPTR) && md_o->ptr)
    {
      free(md_o->ptr);
    }
  md_o->ptr = NULL;
  md_o->next = NULL;
  md_o->prev = NULL;

  return (void*) c_ptr;
}

uintaa_t
md_relink(pmda md)
{
  off_t off, l = 1;

  p_md_obj last = NULL, cur = md->objects;

  for (off = 0; off < md->count; off++)
    {
      if (cur->ptr)
        {
          if (last)
            {
              last->next = cur;
              cur->prev = last;
              l++;
            }
          else
            {
              md->first = cur;
            }
          last = cur;
        }
      cur++;
    }
  return l;
}

uintaa_t
md_relink_n(pmda md, off_t base)
{
  size_t off, l = 1;

  p_md_obj last = NULL, cur = md->objects;

  size_t tot = (size_t) md->objects + ((size_t) md->count * sizeof(md_obj));

  size_t totl = (size_t) md->count / base;

  size_t cp = 0, cnt = 0;

  for (cp = 0; cp < base; cp++)
    {
      for (off = 0; off <= totl; off++)
        {
          cur = md->objects + ((base * off) + cp);
          //cnt++;
          if ((size_t) cur < tot)
            {
              cnt++;
              if (cur->ptr)
                {
                  if (last)
                    {
                      last->next = cur;
                      cur->prev = last;
                      cur->next = NULL;
                      l++;
                    }
                  else
                    {
                      md->first = cur;
                    }
                  last = cur;
                }
            }
          else
            {
              break;
            }
        }
    }

  return l;
}

void *
md_swap(pmda md, p_md_obj md_o1, p_md_obj md_o2)
{
  if (!md_o1 || !md_o2)
    {
      return NULL;
    }

  void *ptr2_s;

  ptr2_s = md_o1->prev;
  md_o1->next = md_o2->next;
  md_o1->prev = md_o2;
  md_o2->next = md_o1;
  md_o2->prev = ptr2_s;

  if (md_o2->prev)
    {
      ((p_md_obj) md_o2->prev)->next = md_o2;
    }

  if (md_o1->next)
    {
      ((p_md_obj) md_o1->next)->prev = md_o1;
    }

  if (md->first == md_o1)
    {
      md->first = md_o2;
    }

  return md_o2->next;
}

void *
md_swap_s(pmda md, p_md_obj md_o1, p_md_obj md_o2)
{
  void *ptr = md_o1->ptr;
  md_o1->ptr = md_o2->ptr;
  md_o2->ptr = ptr;

  return md_o1->next;
}

int
md_copy(pmda source, pmda dest, size_t block_sz)
{
  if (!source || !dest)
    {
      return 1;
    }

  if (dest->count)
    {
      return 2;
    }
  int ret = 0;
  p_md_obj ptr = md_first(source);
  void *d_ptr;

  md_init(dest, source->count);

  while (ptr)
    {
      d_ptr = md_alloc(dest, block_sz);
      if (!d_ptr)
        {
          ret = 10;
          break;
        }
      memcpy(d_ptr, ptr->ptr, block_sz);
      ptr = ptr->next;
    }

  if (ret)
    {
      md_g_free(dest);
    }

  if (source->offset != dest->offset)
    {
      return 3;
    }

  return 0;
}

int
is_memregion_null(void *addr, size_t size)
{
  size_t i = size - 1;
  unsigned char *ptr = (unsigned char*) addr;
  while (!ptr[i] && i)
    {
      i--;
    }
  return i;
}
