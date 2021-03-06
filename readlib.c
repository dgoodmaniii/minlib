/*
 * +AMDG
 */
/*
 * This document was begun on 9 May 1200, the feast of St.
 * Gregory Nazianzen, ECD, and it is humbly dedicated to
 * him, to St.  Wulfric of Haselbury, and to the Immaculate
 * Heart of Mary for their prayers, and to the Sacred Heart
 * of Jesus for His mercy.
*/

/* to get getline() defined */
#define _POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700
#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"errcodes.h"
#include"utility.h"

/* fills the db from the file; allocates memory */
int fill_db(char **ptr, char *filename)
{
	FILE *fp; char *line = NULL;
	ssize_t read; size_t len = 0;
	int numlines = 0; int numrecs = 0;
	char numstr[12];
	char *token;
	int lineno = 0;

	if ((token = malloc(24 * sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory for an"
		"internal function\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	if ((fp = fopen(filename,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file %s, "
		"with the following error:\n\t%d:  "
		"%s\n",filename,errno,strerror(errno));
		exit(BAD_INPUT_FILE);
	}
	while ((read = getline(&line,&len,fp)) != -1) {
		lineno++;
		if (strstr(line,"%%")) {
			numrecs++;
			if ((*(ptr+numlines) = malloc((read+1+num_digs(numrecs)) * 
			sizeof(char))) == NULL) {
				fprintf(stderr,"minlib:  insufficient memory to"
				"load all formatted records\n");
				exit(INSUFF_MEMORY_FORMAT);
			}
			sprintf(numstr,"%d",numrecs); strcpy(*(ptr+numlines),numstr);
			strcat(*(ptr+numlines),line); ++numlines;
			continue;
		} else { /* persuade to ignore badly formatted lines */
			if (validate_line(line)) {
				new_strsep(line,token,':');
			} else if (is_blank_line(line)) {
				continue;
			} else {
				fprintf(stderr,"minlib:  error in input file "
				"\"%s\" on line %d; proceeding anyway...\n",filename,lineno);
				fprintf(stderr,"\tproblem line:  \"%s\"",line);
				continue;
			}
		}
		if ((*(ptr+numlines) = malloc((strlen(token)+1) *
		sizeof(char))) == NULL) {
			fprintf(stderr,"minlib:  insufficient memory to"
			"load all formatted records\n");
			exit(INSUFF_MEMORY_FORMAT);
		}
		chomp(token);
		strcpy(*(ptr+(numlines++)),token);
		if ((*(ptr+numlines) = malloc((strlen(line)+1) * 
		sizeof(char))) == NULL) {
			fprintf(stderr,"minlib:  insufficient memory to"
			"load all formatted records\n");
			exit(INSUFF_MEMORY_FORMAT);
		}
		chomp(line);
		strcpy(*(ptr+numlines),line);
		numstr[0] = '\0';
		++numlines;
	}
	*(ptr+numlines) = NULL;
	fclose(fp);
	free(line);
	free(token);
	return numrecs;
}

/* frees the memory used for the database */
int free_db(char **ptr,int numlines)
{
	int i;

	for (i = 0; i < numlines; ++i)
		free(*(ptr + i));
	return 0;
}
