/*
 * register.c
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */
#include <string.h>
#include <pthread.h>
#include <common.h>
#include <memory_int.h>

#include <register.h>

void *new_entry_link
(	void *g_obj,
		po_gh ptr)
{
	p_goa object = (p_goa) g_obj;

	if ( object->first == NULL ) {
		object->first = ptr;
		object->last = ptr;
	}
	else {
		((po_gh)object->last)->next = ptr;
		ptr->prev = object->last;
		object->last = ptr;
	}
	return (void*) object;
}

int allocate_object_generic_buffer
(	void *ptr,
	size_t *psptr,
	size_t sptr)
{
	if ( !sptr )
		return 1;

	if ( ptr == NULL )
		ptr = calloc (sptr, 1);
	else if ( *psptr
			!= sptr ) {
		free (ptr);
		ptr = calloc (sptr, 1);
	}

	*psptr = sptr;
	return 0;
}

void *register_object
(	void *object,
		void *array,
		size_t *limit,
		size_t block_size)
{
	p_goa g_obj = (p_goa) array;
	po_gh ptr;

	errno = 0;

	pthread_mutex_lock
		(&g_obj->mtx);

	sp:

	if ( (ptr = find_open_array_slot
			((void*)g_obj->m_data,
				*limit, block_size))
				== NULL) {
		pthread_mutex_unlock
			(&g_obj->mtx);
		//printf("fdfd - %u\n", *limit * block_size);
		/**limit += 3;
		byte *tm = calloc(*limit,block_size);
		memcpy (tm, g_obj->m_data,(*limit ) * block_size);
		free(g_obj->m_data);
		g_obj->m_data = calloc(*limit,block_size);
		memcpy (g_obj->m_data, tm, (*limit  ) * block_size);
		free (tm);
		printf("fdfd - %u\n", *limit * block_size);
		goto sp;*/
		errno = 1050;
		return NULL;
	}

	pthread_mutex_lock
				(&ptr->l);

	bzero
		(&((unsigned char*) ptr)[O_GENH_DHO],
			block_size - O_GENH_DHO);

	memcpy (&((unsigned char*) ptr)[O_GENH_DHO],
			 &((unsigned char*) object)[O_GENH_DHO],
				block_size - O_GENH_DHO);

	new_entry_link
		(g_obj, (po_gh) ptr);

	ptr->active = 1;


	pthread_mutex_unlock
				(&ptr->l);

	pthread_mutex_unlock
		(&g_obj->mtx);

	return (void*)ptr;
}

void unregister_object
(	po_gh ptr,
		p_goa object_array)
{

	pthread_mutex_lock
		(&object_array->mtx);

	ptr->active = 0;

	if ( ptr == object_array->first )
		object_array->first = ptr->next;

	if ( ptr == object_array->last )
			object_array->last = ptr->prev;

	if ( ptr->prev )
		((po_gh)ptr->prev)->next
				= ptr->next;

	if ( ptr->next )
		((po_gh)ptr->next)->prev
				= ptr->prev;

	ptr->next = NULL;
	ptr->prev = NULL;

	pthread_mutex_unlock
		(&object_array->mtx);
}


void *search_object
(	void *param,
		size_t param_length,
		void *array,
		size_t block_size,
		size_t block_offset)
{
	p_goa g_obj = (p_goa) array;
	po_gh ptr = (po_gh) g_obj->first;
	void *ret;

	pthread_mutex_lock
			(&g_obj->mtx);

	while ( ptr ) {
		ret = match_data_to_array_block
				((unsigned char*) param,
					param_length,
					(unsigned char*) ptr,
					block_size,
					block_offset);
		if ( ret ) {
			pthread_mutex_unlock
				(&g_obj->mtx);

			return ptr;
		}
		ptr = ptr->next;
	}

	pthread_mutex_unlock
		(&g_obj->mtx);

	return NULL;
}

void *find_open_array_slot
(	void *base, size_t limit,
		size_t block_size)
{
	size_t i;
	unsigned char *ptr
			= (unsigned char*) base;

	for ( i = 0; i < limit; i++, ptr+=block_size ) {
		if ( !((po_gh) ptr)->active ) {
			return (void*) ptr;
		}
	}
	return NULL;
}

int init_register
(	p_goa reg,
		size_t count,
		size_t osz)
{
	if ( reg ) {
		reg->m_data = calloc(count,osz);
		return 0;
	}
	return 1685;
}

int free_register
(	p_goa reg )
{
	if ( reg->m_data ) {
		free(reg->m_data);
		return 0;
	}
	return 1;
}
