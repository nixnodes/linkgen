/*
 * register.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef REGISTER_H_
#define REGISTER_H_ 1

#include <generic.h>

#define REG_DEFAULT_MAX 	0x500

void *new_entry_link (void *g_obj, po_gh ptr);

void *register_object (void *object, void *array, size_t *limit, size_t block_size);
void unregister_object (po_gh ptr, p_goa object_array);

void *search_object (void *param, size_t param_length, void *array, size_t block_size, size_t block_offset);
void *find_open_array_slot (void *base, size_t limit, size_t block_size);

int init_register
(	p_goa reg,
		size_t count,
		size_t osz);

int free_register
(	p_goa reg );

int allocate_object_generic_buffer
(	void *ptr,
	size_t *psptr,
	size_t sptr);

#endif /* REGISTER_H_ */
