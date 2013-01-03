/*
 * options.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_ 1


typedef struct option_reference_array
{
	char *option;
	void *function;
	void *critical;
} *p_ora;

#define		O_REF_ARRAY_OBJ_SIZE 	sizeof(struct option_reference_array)

int match_and_dispatch_option (void *reference_array, char *opt, char *arg);

int process_option (void *reference_array, char *opt);

#define 	MAX_CMD_LENGTH 	4096

int parse_options (char *cfg_f);

int  critical_options_passed;

#endif /* OPTIONS_H_ */
