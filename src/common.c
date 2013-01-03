/*
 * common.c
 *
 *  Created on: Sep 25, 2012
 *      Author: reboot
 */
#include <time.h>
#include <common.h>
#include <str.h>
#include <signal.h>
#include <errno.h>

void exit_program
(int code)
{
	print_str
		(FLAG_OUTPUT_TIME|FLAG_OUTPUT_LOG,
				"exiting program, code"
				" %d: \x1b[1m\x1b[3m'%s'\x1b[0m\n", code,
				strerror_int(code));

	if ( fd_log ) {
		fflush (fd_log);
		fclose (fd_log);
	}

	mem_clean();

	errno = code;

	exit (code);
}

void catch_sig
(int sig)
{
	if ( sig == SIGINT ) {
		print_str
			(FLAG_OUTPUT_TIME|FLAG_OUTPUT_LOG,
				"caught SIGINT: quitting..\n");

		exit_program (0);
	}
	if ( sig == SIGTERM || sig == SIGQUIT ) {
		print_str
			(FLAG_OUTPUT_TIME|FLAG_OUTPUT_LOG,
				"caught SIGTERM|SIGQUIT: exiting here\n");

		exit (sig);
	}

	return;
}
