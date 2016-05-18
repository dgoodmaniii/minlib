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
#include<ncurses.h>
#include<menu.h>
#include"errcodes.h"

/* stores our position in search results */
int ind = 0;

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
	int lastnum = -1;
	int currnum = -2;

	*matched = realloc(*matched,1 * sizeof(int));
	if ((errornum = regcomp(&comppat,pattern,REG_EXTENDED)) != 0) {
		regerror(errornum,&comppat,err,MAX_ERR_LENGTH);
		return -1;
	}
	arrsize = get_size(ptr) - 1;
	for (i = 0; i < arrsize; ++i) {
		result = regexec(&comppat,*(ptr+i),0,NULL,0);
		if (result != 0) {
			regerror(result,&comppat,err,MAX_ERR_LENGTH);
		} else {
			currnum = get_record_num(ptr,i) - 1;
			if (currnum != lastnum) {
				*matched = realloc(*matched,(j+1) * sizeof(int));
				*(*matched+(j++)) = currnum;
				lastnum = currnum;
			}
		}
	}
	regfree(&comppat);
	return j;
}
/* NOTE:  array *matched is free()d in gui.c if necessary,
 * because it's originally allocated there */

int proc_fsearch(MENU *menu, int matchnum, ITEM **lib_list, 
	int *matched, int *recnums, int numrecs, char action)
{
	int i = 0;
	int curr;

	curr = item_index(current_item(menu));
	if (action == 'n') {
		if (ind < (matchnum-1)) {
			ind += 1;
		} else {
			ind = 0;
		}
	} else if (action == 'N') {
		if (ind > 0) {
			ind -= 1;
		} else {
			ind = matchnum - 1;
		}
	} else if (action == 'f') {
		ind = 0;
	}
	for (i = curr; (*(recnums+i) != *(matched+ind)); ++i) {
		if (i == numrecs)
			i = -1;
	}
	set_current_item(menu,lib_list[i]);
	return 0;
}
