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


#define a64                             ((ulint64_t) 1)
#define a32                             ((uint32_t) 1)

#include <inttypes.h>

#ifndef _STDINT_H
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
#ifndef __uint32_t_defined
typedef unsigned int uint32_t;
# define __uint32_t_defined
#endif
#if __WORDSIZE == 64
typedef unsigned long int uint64_t;
#else
__extension__
typedef unsigned long long int uint64_t;
#endif
#endif

#if __x86_64__ || __ppc64__
#define uintaa_t uint64_t
#define ENV_64
#define __STR_ARCH      "x86_64"
#define __AA_SPFH       "%.16X"
#else
#define uintaa_t uint32_t
#define ENV_32
#define __STR_ARCH      "i686"
#define __AA_SPFH       "%.8X"
#endif




void catch_sig (int sig);

int mem_init (void);

int mem_clean (void);

int initialize_mutex (pthread_mutex_t *mutex);

void exit_program(int code);

struct tm *get_time (void);

time_t mclock_n (void);

void catch_sig (int sig);

FILE	*fd_log;
DIR 	*dp;

char	CONFIG_FILE[256];
char 	basepath[256];

void 	*oref_t[8192];

#define		DEFAULT_LOGFILE	  	"/var/log/whoisd.log"

#define	THREAD_ID_NET_WORKER 	1

#endif /* COMMON_H_ */
