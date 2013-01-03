/*
 ============================================================================
 Name        : lparse.c
 Author      : nixnodes
 Version     :
 Copyright   : free for all
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <common.h>
#include <signal.h>

#include <memory.h>
#include <io.h>
#include <option_functions.h>
#include <str.h>

#include <lparse.h>

goa linkdb = {0};
goa asdb = {0};

int LINKDB_SIZE = 2500000;
int ASDB_SIZE = 1000000;

int main(void) {
	int r;
	time_t cs, ce;

	signal (SIGINT, catch_sig);
	signal (SIGTERM, catch_sig);
	signal (SIGQUIT, catch_sig);

	print_str (FLAG_OUTPUT_TIME,
				"\e[00;32m\x1b[3m\x1b[1mlinkgen-%d.%d-%d  /nixnodes ~ 2013\x1b[0m\e[00m\n",
				VER_MAJOR, VER_MINOR, VER_REVISION);

	print_str(FLAG_OUTPUT_TIME,
				"loading: initializing memory ..\n");

	if ( (r = init_register(&linkdb,
			LINKDB_SIZE,
			sizeof (obj_l))) )
		exit_program(r);

	if ( (r = init_register(&asdb,
			ASDB_SIZE,
			sizeof (obj_as))) )
			exit_program(r);

	if ( (r = parse_options (CONFIG_FILE)) )
		exit_program(r);

	print_str(FLAG_OUTPUT_TIME,
			"loading: working out as/link data.. \n");

	cs = mclock_s();

	if ( (r = enum_dir(source_dir,
			load_into_memory,
			NULL, 1)) )
			exit_program(r);

	if ( (r = load_as()) )
			exit_program(r);

	ce = mclock_s() - cs;

	print_str(FLAG_OUTPUT_TIME,
				"loading: working out as/link data: complete (%ds)\n",
				ce);

	int rc = 0;

	if ( strlen(export_json_dir)
			&& (r = export_json()) ) {
			print_str (FLAG_OUTPUT_TIME,
					ERROR_STR_1,
					strerror_int(r));
			rc++;
	}

	if ( strlen(export_aslist_path)
			&& (r = export_aslist()) ) {
			print_str (FLAG_OUTPUT_TIME,
					ERROR_STR_1,
					strerror_int(r));
			rc++;
	}

	if ( strlen(export_links_path)
			&& (r = export_links()) ) {
			print_str (FLAG_OUTPUT_TIME,
					ERROR_STR_1,
					strerror_int(r));
			rc++;
	}

	if ( rc )
		exit_program(2345);
	else
		exit_program(0);
}

int export_aslist
( void )
{
	int r = 0;
	char b[32] = {0};
	time_t cs,ce=0;

	print_str(FLAG_OUTPUT_TIME,
			"exporting: AS list to '%s'.. \n", export_aslist_path);

	FILE *fh = fopen(export_aslist_path, "w");

	if ( !fh )
		return 1726;

	cs = mclock_n();

	p_obj_as ptr = (p_obj_as) asdb.first;

	while ( ptr ) {
		sprintf(b,"%u\n", ptr->as);
		if ( (r=write_file(b,fh,1721,1)) )
				return r;

		ptr = ptr->o.next;
	}

	ce = mclock_n() - cs;

	fflush(fh);

	print_str(FLAG_OUTPUT_TIME,
			"exporting: AS list: completed (%.4fs)\n",
					(double)ce / 1000000);

	fclose (fh);

	return r;
}

int export_links
( void )
{
	int r = 0;
	char b[32] = {0};
	time_t cs,ce=0;

	print_str(FLAG_OUTPUT_TIME,
			"exporting: AS links '%s'.. \n", export_links_path);

	FILE *fh = fopen(export_links_path, "w");

	if ( !fh )
		return 1736;

	cs = mclock_n();

	p_obj_l ptr = (p_obj_l) linkdb.first;

	while ( ptr ) {
		sprintf(b,"%u-%u\n", ptr->s, ptr->d);
		if ( (r=write_file(b,fh,1731,1)) )
				return r;

		ptr = ptr->o.next;
	}

	ce = mclock_n() - cs;

	fflush(fh);

	print_str(FLAG_OUTPUT_TIME,
			"exporting: AS links: completed (%.4fs)\n",
					(double)ce / 1000000);

	fclose (fh);

	return r;
}

int export_json
( void )
{
	int ir, qr = 0;
	char b[256] = {0};
	uint i=0;
	int64 r,s,d;
	time_t cs,ce=0, at, lt;

	print_str(FLAG_OUTPUT_TIME,
			"exporting: D3js .json format to '%s'.. \n"
			,export_json_dir);

	FILE *fh = fopen(export_json_dir, "w");

	if ( !fh )
		return 1876;

	cs = mclock_n();

	if ( (qr=write_file(NODES_HEADER,fh,1821,1)) )
			return qr;

	p_obj_as ptra = (p_obj_as) asdb.first;

	while ( ptra ) {
		if ( ptra->o.next )
			sprintf(b, GROUP_TEMP1, ptra->as);
		else
			sprintf(b, GROUP_TEMP2, ptra->as);

		if ( (qr=write_file(b,fh,1821,1)) )
					return qr;

		ptra = ptra->o.next;
	}

	if ( (qr=write_file(NODES_FOOTER,fh,1821,1)) )
			return qr;

	p_obj_l ptrb = (p_obj_l) linkdb.first;

	while ( ptrb ) {
		if ( (r = match_as_numindex(ptrb->s)) > -1 )
			s=r;
		if ( (r = match_as_numindex(ptrb->d)) > -1 )
			d=r;

		if ( !(s > -1
				&& d > -1)) {
			qr=1878;
			break;
		}

		if ( ptrb->o.next )
			sprintf(b, LINK_TEMP1, s, d);
		else
			sprintf(b, LINK_TEMP2, s, d);

		if ( (qr=write_file(b,fh,1821,1)) )
			return qr;

		ptrb = ptrb->o.next;
	}

	if ( (qr=write_file(LINKS_FOOTER,fh,1821,1)) )
			return qr;

	fflush(fh);

	at = mclock_n() - cs;

	print_str(FLAG_OUTPUT_TIME,
			"exporting: D3js .json format: completed (%.4fs)\n",
			(double)at / 1000000);

	fclose (fh);

	return qr;
}

int64 match_as_numindex
(	uint as)
{
	int i = 0;
	p_obj_as ptr = (p_obj_as) asdb.first;
	while ( ptr ) {
		if ( as == ptr->as )
			return i;
		ptr = ptr->o.next; i++;
	}
	return -1;
}

int load_as
( void )
{
	int r = 0;
	obj_as as = {0};
	p_obj_l ptr = (p_obj_l) linkdb.first;

	while ( ptr ) {
		if ( !match_as(ptr->s) ) {
			as.as = ptr->s;
			register_object(&as, &asdb, &ASDB_SIZE, sizeof (obj_as));
			if ( errno )
				return errno;
			r++;
		}
		if ( !match_as(ptr->d) ) {
			as.as = ptr->d;
			register_object(&as, &asdb, &ASDB_SIZE, sizeof (obj_as));
			if ( errno )
				return errno;
			r++;
		}
		ptr = ptr->o.next;
	}

	if ( !r )
		return 1561;

	return 0;
}

int load_into_memory
(	char *data,
		unsigned char type,
		void *arg)
{
	FILE *fh;
	int r = 0;
	char abp[255] = {0};
	char alb[16384] = {0};

	if ( reg_match("[0-9]{2,8}\\.txt",data) ) {
		return 0;
	}
	sprintf(abp, "%s/%s", source_dir, data);

	fh = fopen (abp, "r");
	if ( !fh ) {
		return 1691;
	}

	while ( fgets(alb, 16384, fh)  ) {
		if ( (r = parse_path_chain (alb)) )
			break;
	}

	fclose (fh);
	return r;
}

int parse_path_chain
(	char *line)
{
	char *loo[1024*8] = {0};
	obj_l l;

	int r = split_string(line,0x20,loo,499);
	int i;
	for ( i = 0; i < r - 1; i++ ) {
		memset (&l,0x0,sizeof(obj_l));
		l.s = (uint)atoi((char*)&loo[i]);
		l.d = (uint)atoi((char*)&loo[i+1]);

		if ( l.s != 23456 && l.d != 23456 )
			if ( !match_link (l.s, l.d)
					&& l.s != l.d ) {
				register_object(&l, &linkdb, &LINKDB_SIZE, sizeof (obj_l));
				if ( errno )
					return errno;
			}
	}

	return 0;
}

void *match_link
(	uint s,
		uint d)
{
	p_obj_l ptr = (p_obj_l) linkdb.first;

	while ( ptr ) {
		if ( ( s == ptr->s && d == ptr->d ) ||
			 ( s == ptr->d && d == ptr->s ) ) {
			return ptr;
		}
		ptr = ptr->o.next;
	}
	return NULL;
}

void *match_as
( uint as )
{
	p_obj_as ptr = (p_obj_as) asdb.first;

	while ( ptr ) {
		if ( as == ptr->as  ) {
			return ptr;
		}
		ptr = ptr->o.next;
	}
	return NULL;
}
