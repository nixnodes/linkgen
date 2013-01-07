/*
 * time.c
 *
 *  Created on: Sep 19, 2012
 *      Author: reboot
 */
#include <time.h>

#include <common.h>



time_t mclock_s
(void)
{
	struct timespec ts = { 0 };
	if( !clock_gettime(CLOCK_MONOTONIC,&ts) ) {
		return ts.tv_sec;
	}

	return 0;
}



time_t mclock_n
(void)
{
	struct timespec ts_n = { 0 };
	if( !clock_gettime(CLOCK_MONOTONIC,&ts_n) ) {
		return ts_n.tv_nsec;
	}

	return 0;
}

struct tm *get_time
(void)
{
	time_t t = time(NULL);
	return localtime(&t);
}
