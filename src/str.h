/*
 * str.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef STR_H_
#define STR_H_ 1

#define EMPTY_STRING	""

#define	MAX_OUTPUT_SIZE 	2048

#define	FLAG_OUTPUT_TIME	0x0001
#define	FLAG_OUTPUT_LOG		0x0002
#define	FLAG_OUTPUT_NL_B	0x0004

int get_string_offset (char *, char, size_t);
int prune_string_alpha_numerical (char *input, char *output, int len);
int trim_string (char *input, char *output, char match, int len);

int print_str (unsigned short flags, char *buf, ...);

int val_as_bin_str (char *out, uint64 val);

pthread_mutex_t  tse;

#include <memory_int.h>

int
split_string(char *line, char dl, pmda output_t);

#endif /* STR_H_ */
