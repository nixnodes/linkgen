/*
 * generic.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef GENERIC_H_
#define GENERIC_H_ 1

#include <pthread.h>

typedef struct generic_object_header
{
	char 		active;
  	void 		*next;
  	void 		*prev;
  	pthread_mutex_t  l;
} o_gh, *po_gh;

typedef struct generic_object_array
{
	void 		*m_data ;
	void		*first;
	void 		*last;
	pthread_mutex_t  mtx, mtx2;
} goa, *p_goa;

#define	O_GENH_DHO 		(sizeof(o_gh))

#endif /* GENERIC_H_ */
