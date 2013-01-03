#include <string.h>
#include <common.h>
#include <options.h>
#include <io.h>
#include <str.h>

#include <lparse.h>

#include <option_functions.h>

int proc_opt_logfile
(char *arg,
		int len)
{
	memset (LOG_FILE, 0x0, 256);
	memcpy (LOG_FILE, arg, 256);

	errno = 0;

	fd_log = fopen (LOG_FILE, "a");

	return errno;
}


int proc_export_json
(char *arg,
		int len)
{
	memset (export_json_dir, 0x0, 256);
	memcpy (export_json_dir, arg, 256);

	return 0;
}

int proc_export_links
(char *arg,
		int len)
{
	memset (export_links_path, 0x0, 256);
	memcpy (export_links_path, arg, 256);

	return 0;
}

int proc_export_aslist
(char *arg,
		int len)
{
	memset (export_aslist_path, 0x0, 256);
	memcpy (export_aslist_path, arg, 256);

	return 0;
}

int proc_opt_sourcedir
(char *arg,
		int len)
{
	memset (source_dir, 0x0, 256);
	memcpy (source_dir, arg, 256);

	return 0;
}


int proc_opt_daemon
(char *arg,
		int len)
{
	DAEMON = atoi (arg);

	if ( DAEMON < 0 )
		return 1043;

	return 0;
}

#define	O_REF_ARRAY_COUNT ((sizeof(oref_t) / sizeof(struct option_reference_array)) - 1)

int num_critical_options
(p_ora reference_array)
{
	int c;
	for ( c = 0; reference_array->option &&
		reference_array->function; reference_array++  ) {
		if ( reference_array->critical )
			c++;		
	}
	return c;
}

char 	LOG_FILE[256]
     	         = { "/var/log/lparse.log" };
char 	CONFIG_FILE[256]
     	            = { "/etc/lparse.conf" };


char 	source_dir[256] = {0},
		export_json_dir[256] = {0},
		export_links_path[256] = {0},
		export_aslist_path[256] = {0};


void *oref_t[] = {

	"log_file", proc_opt_logfile, (void*) 0,
	"daemon", proc_opt_daemon, (void*) 0,
	"source_dir", proc_opt_sourcedir, (void*) 1,
	"export_json", proc_export_json, (void*) 0,
	"export_links", proc_export_links, (void*) 0,
	"export_aslist", proc_export_aslist, (void*) 0,
	NULL
};
