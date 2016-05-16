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
	char deffile[] = "libtext";
	char defform[] = "%30t | %20a | %4l | %10q";
	char *formstring;
	char *filename;
	char template[] = "mlibtmpXXXXXX";
	int fdval;

	opterr = 0;
	if ((filename = malloc((strlen(deffile)+1)*sizeof(char)))==NULL) {
		fprintf(stderr,"minlib:  insufficient memory for "
		"default filename string\n");
		exit(INSUFF_MEMORY_FILENAME);
	}
	strcpy(filename,deffile);
	if ((formstring = malloc((strlen(defform)+1)*sizeof(char)))==NULL) {
		fprintf(stderr,"minlib:  insufficient memory for "
		"default format string\n");
		exit(INSUFF_MEMORY_FORMSTRING);
	}
	strcpy(formstring,defform);
	while ((c = getopt(argc,argv,"Vf:r:")) != -1) {
		switch (c) {
		case 'V':
			printf("minlib v0.9\n");
			printf("Copyright (C) 2014  Donald P. Goodman III\n");
			printf("License GPLv3+:  GNU GPL version 3 or "
			"later <http://gnu.org/licenses/gpl.html>\n");
			printf("This is free software:  you are free "
			"to change and redistribute it.  There is NO "
			"WARRANTY, to the extent permitted by law.\n");
			exit(ALLGOOD);
			break;
		case 'r':
			if (strlen(optarg) > strlen(formstring)) {
				if ((formstring = realloc(formstring,
				(strlen(optarg)+1)*sizeof(char)))==NULL) {
					fprintf(stderr,"minlib:  insufficient memory for "
					"format string specified with -r\n");
					exit(INSUFF_MEMORY_FORMSTRING);
				}
			}
			strcpy(formstring,optarg);
			break;
		case 'f':
			if (strstr(optarg,",")) {
				fdval = mkstemp(template);// unlink(template);
				readfile(ptr,optarg,filename,fdval);
				if (strlen(template) > strlen(filename)) {
					if ((filename = realloc(filename,
					(strlen(template)+1)*sizeof(char)))==NULL) {
						fprintf(stderr,"minlib:  insufficient memory for "
						"filename\n");
						exit(INSUFF_MEMORY_FILENAME);
					}
				}
				strcpy(filename,template);
			} else {
				if (strlen(optarg) > strlen(filename)) {
					if ((filename = realloc(filename,
					(strlen(optarg)+1)*sizeof(char)))==NULL) {
						fprintf(stderr,"minlib:  insufficient memory for "
						"filename specified with -f\n");
						exit(INSUFF_MEMORY_FILENAME);
					}
				}
				strcpy(filename,optarg);
			}
			fdval = 1;
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
	free(formstring);
	free(filename);
	if (!strstr(template,"XXXXXX"))
		unlink(template);
	return 0;
}

int populate_recnums(int *recnums, int len)
{
	int i;
	
	for (i = 0; i < len; ++i)
		*(recnums+i) = i;
	return 0;
}
