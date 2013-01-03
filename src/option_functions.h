/*
 * option_functions.h
 *
 *  Created on: Sep 14, 2012
 *      Author: reboot
 */

#ifndef OPTION_FUNCTIONS_H_
#define OPTION_FUNCTIONS_H_ 1

#include <options.h>

char 	LOG_FILE[256];

int 		DAEMON;

char 	source_dir[256],
		export_json_dir[256],
		export_links_path[256],
		export_aslist_path[256];

int proc_opt_sourcedir
(char *arg,
		int len);

int proc_opt_logfile (char *arg, int len);

int proc_opt_daemon (char *arg, int len);

int num_critical_options (p_ora reference_array);

#define WHOIS_PORT "43"

void *oref_t[];

#endif /* OPTION_FUNCTIONS_H_ */
