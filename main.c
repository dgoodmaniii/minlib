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
#include<unistd.h>
#include"errcodes.h"

int main(int argc, char **argv)
{
	char **ptr; char **formlist; int *recnums;
	int numlines = 0;
	int numrecs = 0;
	int i = 0;
	char c;
	char filename[] = "text";
	char formstring[] = "%30t | %19a | %4l | %10q |";

	opterr = 0;
	while ((c = getopt(argc,argv,"vf:r:")) != -1) {
		switch (c) {
		case 'v':
			printf("minlib v0.9\n");
			printf("Copyright (C) 2014  Donald P. Goodman III\n");
			printf("License GPLv3+:  GNU GPL version 3 or "
			"later <http://gnu.org/licenses/gpl.html>\n");
			printf("This is free software:  you are free "
			"to change and redistribute it.  There is NO "
			"WARRANTY, to the extent permitted by law.\n");
			return ALLGOOD;
			break;
		case '?':
			if ((optopt == 'f') || (optopt == 'r')) {
				fprintf(stderr,"minlib:  option \"%c\" requires "
				"an argument\n",optopt);
				exit(NEED_ARGUMENT_ARG);
			}
			fprintf(stderr,"minlib:  unrecognized option \"%c\"\n",optopt);
			exit(BAD_OPTION);
			break;
		}
	}
	numlines = count_lines_file(filename);
	if ((ptr = malloc(((numlines+1)*2-count_recs_file(filename)) 
		* sizeof(char*))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to load "
		"%d records from file \"%s\"\n",numlines,filename);
		exit(INSUFF_MEMORY_LOAD);
	}
	numrecs = fill_db(ptr,filename);
	if ((formlist = malloc((numrecs+1) * sizeof(char*))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to load "
		"%d summarized records from file \"%s\"\n",numrecs,filename);
		exit(INSUFF_MEMORY_FORMAT);
	}
	*(formlist+numrecs) = NULL;
	if ((recnums = malloc((numrecs+1) * sizeof(int))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to track "
		"%d summarized records from file \"%s\"\n",numrecs,filename);
		exit(INSUFF_MEMORY_TRACK);
	}
	populate_recnums(recnums,numrecs+1);
	for (i = 0; i < numrecs; ++i) {
		*(formlist+i) = malloc(81 * sizeof(char));
		*(*(formlist+i+0)) = '\0';
	}
	format_recs(ptr,formstring,formlist,numrecs,recnums);
/*	for (i = 0; i < ((numlines)*2-count_recs_file(filename)); ++i) {
		printf("%s",*(ptr+i));
	}*/
	for (i = 0; i < numrecs; ++i)
		printf("%3d: %s\n",*(recnums+i),*(formlist+i));
	load_gui(ptr,formlist,recnums,numrecs);
	free_db(ptr,numlines*2-count_recs_file(filename));
	free(ptr);
	for (i=0; i <= numrecs; ++i)
		free(*(formlist+i));
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
