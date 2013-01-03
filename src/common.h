/*
 * common.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef COMMON_H_
#define COMMON_H_ 1

#include <settings.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>

#include <generic.h>

#include <unistd.h>
#include <thread.h>

#define	uint64		unsigned long long int
#define	int64		long long int
#define uint		unsigned int
#define uint16		unsigned short
#define byte		unsigned char

void catch_sig (int sig);

int mem_init (void);

int mem_clean (void);

int initialize_mutex (pthread_mutex_t *mutex);

void exit_program(int code);

struct tm *get_time (void);

time_t mclock_s (void);
__syscall_slong_t mclock_n (void);

void catch_sig (int sig);

FILE	*fd_log;
DIR 	*dp;

char	CONFIG_FILE[256];
char 	basepath[256];

void 	*oref_t[8192];

#define		DEFAULT_LOGFILE	  	"/var/log/whoisd.log"

#define	THREAD_ID_NET_WORKER 	1

#endif /* COMMON_H_ */
