/*
 * error.c
 *
 *  Created on: Sep 15, 2012
 *      Author: reboot
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <common.h>
#include <str.h>

#include <error_int.h>

char err_buffer [MAX_ERROR_BUFFER][MAX_ERR_MSG_LEN] = { {0} };

void *ec_ref[1024] = {
		(void*) 0000, "gracefull quit",
		(void*) 1001, "expected a string as option, but got nothing",
		(void*) 1002, "syntax error: expected '=' after option name",
		(void*) 1003, "empty option value",
		(void*) 1006, "input line too short",
		(void*) 1007, "empty input line",
		(void*) 1005, "critical options are missing from config",
		(void*) 1009, "invalid option, ignoring",
		(void*) 1201, "no config file found",
		(void*) 1043, "DAEMON valid optons are 0 (normal) and 1 (background)",
		(void*) 1050, "register full",
		(void*) 1351, "unable to register class",
		(void*) 1113, "cannot initialize mutexes",
		(void*) 1340, "unable to register new connection",
		(void*) 1342, "connection accepted",
		(void*) 1370, "connection shutting down gracefully",
		(void*) 1876, "can't write to json export file",
		(void*) 1736, "can't write to links export file",
		(void*) 1726, "can't write to AS list export file",
		(void*) 1721, "error writing to AS list export file",
		(void*) 1821, "error writing to json export file",
		(void*) 1731, "error writing to links export file",
		(void*) 2345, "\e[00;33m\x1b[1m\x1b[3mwarning: not all jobs suceeded\x1b[0m\e[00m",

		NULL, NULL
};

pthread_mutex_t mutex_err_buffer
				= PTHREAD_MUTEX_INITIALIZER;

char *strerror_int
(unsigned int errcode_int)
{
	int i, r;
	char *message = NULL;

	p_ecd ptr = (p_ecd) ec_ref;

	for ( i = 0, r = 0;
			i < MAX_ERROR_BUFFER && ptr;
				ptr++, i++ ) {
		if ( (size_t) ptr->code ==
				(size_t) errcode_int ) {
			message = ptr->message;
			r = 1;
			break;
		}
	}

	if ( !r ) {
		errno = 0;
		message = ERROR_MSG_DEFAULT; //strerror ((int) errcode_int);
		if ( !errno && message )
			r = 1;
	}

	if ( r )
		return message;
	else
		errno_int = 61;

	if ( i == MAX_ERROR_BUFFER )
		errno_int = 69;

	return ERROR_MSG_DEFAULT;
}

char *insert_string_to_free_buffer_slot
(char *string,
		size_t len)
{
	int i;

	if ( len > MAX_ERR_MSG_LEN ) {
		errno_int = 51;
		return NULL;
	}

	if ( !len ) {
		errno_int = 52;
		return NULL;
	}

	for ( i = 0 ;i < MAX_ERROR_BUFFER && err_buffer[i][0]; i++ ) {
	}

	pthread_mutex_lock(&mutex_err_buffer_lock);



	if ( i == MAX_ERROR_BUFFER ) {
		i = 0;
		bzero (err_buffer[i], MAX_ERR_MSG_LEN);
	}

	if ( (i < (MAX_ERROR_BUFFER - 1) ) && err_buffer[i + 1][0] ) {
		bzero (err_buffer[i + 1], MAX_ERR_MSG_LEN);
	}

	memcpy (err_buffer[i], string, len);

	pthread_mutex_unlock(&mutex_err_buffer_lock);

	return err_buffer[i];
}
