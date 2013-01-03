/*
 * lparse.h
 *
 *  Created on: Jan 3, 2013
 *      Author: reboot
 */

#ifndef LPARSE_H_
#define LPARSE_H_

int 	LINKDB_SIZE;
int 	ASDB_SIZE;


#include <generic.h>

typedef struct link_object
{
	o_gh o;
	uint s, d;
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


#endif /* LPARSE_H_ */
