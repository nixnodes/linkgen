/*
 * lparse.h
 *
 *  Created on: Jan 3, 2013
 *      Author: reboot
 */

#ifndef LPARSE_H_
#define LPARSE_H_

#define NODES_HEADER 	"{\"nodes\":["
#define NODES_FOOTER 	"],\"links\":["
#define LINKS_FOOTER	"]}"
#define GROUP_TEMP1		"{\"group\":1,\"name\":\"%u\"},"
#define GROUP_TEMP2		"{\"group\":1,\"name\":\"%u\"}"
#define LINK_TEMP1		"{\"source\":%u,\"target\":%u,\"value\":1},"
#define LINK_TEMP2		"{\"source\":%u,\"target\":%u,\"value\":1}"

#define ERROR_STR_1 "\e[00;31m\x1b[1m\x1b[3merror: %s\x1b[0m\e[00m\n"

int 	LINKDB_SIZE;
int 	ASDB_SIZE;


#include <generic.h>

typedef struct link_object
{
	o_gh o;
	int64 s, d;
} obj_l, *p_obj_l;

typedef struct as_object
{
	o_gh o;
	uint as;
} obj_as, *p_obj_as;

int load_into_memory
(	char *data,
		unsigned char type,
		void *arg);

int parse_path_chain
(		char *line );

void *match_link
(	uint s,
		uint d);

void *match_as
( 	uint as );

int64 match_as_numindex
(	uint as);

int export_json
( void );

int load_as
( void);

int export_links
( void );

int export_aslist
( void );

goa linkdb,
	asdb;

#endif /* LPARSE_H_ */
