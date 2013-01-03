#include <string.h>
#include <errno.h>
#include <common.h>
#include <str.h>
#include <error_int.h>
#include <option_functions.h>

#include <options.h>

int 	critical_options_passed ;

int match_and_dispatch_option
(void *reference_array,
		char *opt, char *arg)
{
	int (*proc_opt_generic)
			(char *arg, int len);
	int r;

	p_ora ora = (p_ora) reference_array;

	int str_len = strlen(arg);

	if ( !str_len ) {
		return 1003;
	}

	while ( ora->option &&
				ora->function ) {
	 	if ( !strcmp
	 			(ora->option, opt) ) {
			proc_opt_generic =
					ora->function;
			
			if ( (r = proc_opt_generic
						(arg, str_len)) )
				return r;
			
			if ( ora->critical ) 
				critical_options_passed++;

			return 	r;	
		}		
		ora++;
	}
	return 1009;
}

int process_option
(void *reference_array,
		char *opt)
{
	int r, str_len
			= strlen (opt);
	char buf[MAX_CMD_LENGTH] = {0},
	     option[256] = {0};
	     
	
	if ( !str_len )
		return 1001;

	if ( str_len <= 2 ) 
		return 1006;

	if ( opt[0] == 0x23 )
		return 1007;

	r = get_string_offset
			(opt, 0x3d, str_len);

	if ( r < 0 )
		return 1002;

	memcpy (buf, opt, r);

	prune_string_alpha_numerical
			(buf, option, 256);

	bzero (buf, MAX_CMD_LENGTH);

	memcpy (buf, &opt[r + 1],
			MAX_CMD_LENGTH - r - 2);

	trim_string (buf, buf,
			0x22, strlen(buf) );

	//printf ("%s - %s\n", option, buf);
	
	return match_and_dispatch_option
			(reference_array, option, buf);
}


int parse_options
(char *cfg_f)
{
	int line = 0, ok = 0, r;
	char buf[MAX_CMD_LENGTH] = {0};

	FILE *fd = fopen (cfg_f, "r");

	if ( !fd )
		return 1201;

	while (!feof(fd)) {
		bzero (buf, MAX_CMD_LENGTH);
		if ( fgets (buf, MAX_CMD_LENGTH, fd) ) {
			line++;
			if ( ( r = process_option (oref_t, buf)) ) {
				if ( r != 1006 && r != 1007 )
					print_str
					(FLAG_OUTPUT_TIME|FLAG_OUTPUT_LOG,
						"parsing options failed on line %d,"
						" code %d: '%s'\n", line, r,
						strerror_int(r));
			}
			else
				ok++;
		}
	}

	fclose (fd);

	if ( critical_options_passed <
			num_critical_options((p_ora) oref_t) )
		return 1005;

	return 0;
}
