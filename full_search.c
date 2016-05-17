/*
 * +AMDG
 */
/*
 * This document was begun on 14 May 1200, the feast of St.
 * Ubald, EC, and it is humbly dedicated to him and to the
 * Immaculate Heart of Mary for their prayers, and to the
 * Sacred Heart of Jesus for His mercy.
 */

#include<regex.h>
#include<stdio.h>
#include<stdlib.h>
#include"errcodes.h"

/* handles the full-text search functions, accessed by
 * gui.c; alters an array of ints containing the record
 * numbers of the matched records.  Returns 0 if matches
 * made, 1 if not, 2 if the regexp didn't compile.  Takes
 * the full database; the array for matches; and the regexp
 * string. */
int full_search(char **ptr, int **matched, char *pattern, char *err) {
	
	regex_t comppat;
	int errornum = 0;
	int result;
	int i; int j = 0;
	int arrsize;

	if ((errornum = regcomp(&comppat,pattern,REG_EXTENDED)) != 0) {
//		regerror(errornum,&comppat,err,MAX_ERR_LENGTH);
		return 2;
	}
	arrsize = get_size(ptr) - 1;
	for (i = 0; i < arrsize; ++i) {
		result = regexec(&comppat,*(ptr+i),0,NULL,0);
		if (result != 0) {
			regerror(result,&comppat,err,MAX_ERR_LENGTH);
		} else {
			*matched = realloc(*matched,(j+1) * sizeof(int));
			*(*matched+(j++)) = i;
		}
	}
	regfree(&comppat);
	return 0;
}
/* NOTE:  array *matched is free()d in gui.c if necessary */
