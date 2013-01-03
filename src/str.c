#include <string.h>
#include <pthread.h>
#include <stdarg.h>
#include <common.h>
#include <thread.h>
#include <error_int.h>

#include <str.h>

pthread_mutex_t tso
		= PTHREAD_MUTEX_INITIALIZER;

int get_string_offset
(char *object,
		char match,
		size_t len)
{
	int i;

	for ( i = 0; i < len ; i++ ) { 
		if ( object[i] == match ) {
			return i;
		}
	}
	return -1;	
}

int prune_string_alpha_numerical
(char *input,
		char *output,
		int len)
{
	int i, w;
	
	for ( i = 0, w = 0; i < len; i++) {
		if ( (input[i] >= 0x41 && input[i] <= 0x5A) || 
			(input[i] >= 0x61 && input[i] <= 0x7A) || 
			(input[i] >= 0x30 && input[i] <= 0x39) ||
			input[i] == 0x5F || input[i] == 0x2D ) {
				output[w] = input[i]; 
				w++;
		}
	}

	return 0;
}

int trim_string
(char *input,
	char *output,
	char match,
	int len)
{
	int i;
	
	for ( i = 0; input[i] == match ||
			input[i] == 0x20 ; i++ ) {
	}

	memmove (output, &input[i], len - i - 1);
	memset (&output[len - i - 1], 0x0, 1);

	for ( i = len - i - 2 ;
			input[i] == match || input[i] == 0xA ||
			input[i] == 0xD || input[i] == 0x20;
			i-- ) {
		output[i] = 0x0;
	}

	return 0;
}

int print_str
(unsigned short flags,
		char *buf,
		...)
{
	pthread_mutex_lock (&tse);

	size_t wc;
  	char  buffer[MAX_OUTPUT_SIZE] = {0},
	      buffer_2[512] = {0};
	va_list al;
	va_start (al,buf);

	if ( flags & FLAG_OUTPUT_NL_B ) {
		memcpy (buffer_2, "\n", 1);
	}

  	if ( flags & FLAG_OUTPUT_TIME ) {
		struct tm tm = *get_time();
		sprintf(&buffer_2[strlen(buffer_2)],
				"[%.2u-%.2u-%.2u %.2u:%.2u:%.2u] ",
				(tm.tm_year + 1900) % 100,
				tm.tm_mon + 1, tm.tm_mday,
				tm.tm_hour, tm.tm_min, tm.tm_sec);
	}

	sprintf (&buffer_2[strlen(buffer_2)],
			"%s", buf) ;

	vsprintf (buffer, buffer_2, al);



	if ( (flags & FLAG_OUTPUT_LOG) && fd_log ) {

		if ( (wc = fwrite (buffer, 1,
				strlen(buffer), fd_log)) <= 0 ) {
			errno_int = 21;
		}

		fflush (fd_log);
	}

	printf (buffer);
	fflush (stdout);

	pthread_mutex_unlock (&tse);
	return 0;
}

int split_string
(	char *line,
		char dl,
		char *output_t[],
		int max_o)
{
	int i, p, c, llen
			= strlen (line);

	if ( llen > max_o )
		return 1489;

	for ( i = 0, p = 0, c = 0;
			i <= llen;
			i++ ) {

		while ( line[i] == dl
				&& line[i] )
						i++;
		p = i;

		while ( line[i] != dl
				&& line[i] != 0xA
				&& line[i] )
						i++;

		if ( i > p) {
			memcpy (&output_t[c],
					&line[p],
					i - p);
			c++;
		}
	}
	return c;
}

int val_as_bin_str (char *out, uint64 val)
{
	int y;
	uint64 x;

	for ( y = 0; val > 1; y++ ) {
		x = val % 2;
		val >>= 1;
		out[y] = 0x30 + x;
	}

	out[y] = 0x30 + val;

	reverse_byte_array ((byte*) out, y + 1);

	return y + 1;
}


