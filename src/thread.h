/*
 * thread.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef THREAD_H_
#define THREAD_H_ 1

#include <stdio.h>

#include <generic.h>
#include <pthread.h>

#define THREAD_MAX	50

#define	MAX_OBJ_GEN_WORKER_ARG	256

typedef struct gen_worker_arg
{
	int count;

} gwa, *p_gwa;

typedef struct object_gen_worker
{
	void	*call;
	int		a_count;
	void 	*arg;
} o_gw, *p_ogw;

typedef struct object_thrd
{
	o_gh 		o;
	pthread_t   pt;
	int 		id;
	int 		signal,
				running;
	time_t		s, e;
	FILE		*fd_pipe;
	o_gw		gw_data;
} o_thrd, *po_thrd;

pthread_t thread_create (void *call, int id);

int thread_destroy (pthread_t pt);

po_thrd thread_get_from_id (int id);

po_thrd thread_get_from_tid (pthread_t tid);

goa threads;

#endif /* THREAD_H_ */
