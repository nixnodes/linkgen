#include <sys/types.h>
#include <regex.h>

#include <regex_int.h>

int reg_match
(char *expression,
		char *match)
{	
	regex_t preg; size_t r;
	regmatch_t pmatch[REG_MATCHES_MAX];

	if ((r = regcomp (&preg, expression, REG_EXTENDED)))  
		 return r;
	
	r = regexec (&preg, match,REG_MATCHES_MAX, pmatch, REG_EXTENDED);

	regfree (&preg);

	return r;
}
