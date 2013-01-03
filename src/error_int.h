/*
 * error_int.h
 *
 *  Created on: Sep 15, 2012
 *      Author: reboot
 */

#ifndef ERROR_INT_H_
#define ERROR_INT_H_

#define MAX_ERROR_BUFFER 100
#define MAX_ERR_MSG_LEN	1024

#define ERROR_MSG_DEFAULT	"no description"

typedef struct errcode_descriptor
{
	void *code;
	char *message;
} ecd, *p_ecd;

char *strerror_int (unsigned int code);

pthread_mutex_t mutex_err_buffer_lock;

char *insert_string_to_free_buffer_slot (char *string, size_t len);

int errno_int;

void *ec_ref[1024];

#endif /* ERROR_INT_H_ */
