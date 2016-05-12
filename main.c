/*
 * +AMDG
 */
/*
 * This document was begun on 7 May 1200, the feast of St.
 * Stanilaus, EM, and it is humbly dedicated to him, to St.
 * Wulfric of Haselbury, and to the Immaculate Heart of Mary
 * for their prayers, and to the Sacred Heart of Jesus for
 * His mercy.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char **ptr; char **formlist; int *recnums;
	int numlines = 0;
	int numrecs = 0;
	int i = 0;
	char filename[] = "text";
	char formstring[] = "%20t | %15a | %10y | %10q |";

	numlines = count_lines_file(filename);
	ptr = malloc(((numlines+1)*2-count_recs_file(filename)) * sizeof(char*));
	numrecs = fill_db(ptr,filename);
	formlist = malloc((numrecs+1) * sizeof(char*));
	*(formlist+numrecs) = NULL;
	recnums = malloc((numrecs+1) * sizeof(int));
	populate_recnums(recnums,numrecs+1);
	format_recs(ptr,formstring,formlist,numrecs,recnums);
/*	for (i = 0; i < ((numlines)*2-count_recs_file(filename)); ++i) {
		printf("%s",*(ptr+i));
	}*/
	free_db(ptr,numlines*2-count_recs_file(filename));
	free(ptr);
	free(formlist);
	free(recnums);
	return 0;
}

int populate_recnums(int *recnums, int len)
{
	int i;
	
	for (i = 0; i < len; ++i)
		*(recnums+i) = i;
	return 0;
}
