/*
 * io.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef IO_H_
#define IO_H_ 1

#include <stdio.h>

#include <thread.h>

long get_file_size (FILE *fp);

long read_file (char *file, unsigned char *buffer, size_t read_max);

int open_pipe_to_executable (char *command, po_thrd o_thread);
int read_from_pipe (char *buffer, FILE *pipe);

int file_exists (char *file);
int dir_exists (char *dir);

int write_file
( 	char *data,
		FILE *fh,
		int rof,
		int f);

int exec_and_wait_for_output (po_thrd ptr, char *command);

#define	READ_MAX	0x2000

int enum_dir
(	char *dir,
	void *cb,
	void *arg,
	int f);

#endif /* IO_H_ */
