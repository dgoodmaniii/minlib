/*
 * +AMDG
 */
/*
 * This document was begun on 13 May 1200, the feast of
 * Pentecost; and it is humbly dedicated to the Holy Spirit
 * and to the Sacred Heart of Jesus, for Their mercy.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"errcodes.h"

int readfile(char **ptr, char *filelist, char *filename, int tempfile)
{
	FILE *currfp;
	FILE *tmpfile;
	char *token;
	char c;
	int numrecs;

	if ((tmpfile = fdopen(tempfile,"w")) == NULL) {
		fprintf(stderr,"minlib:  unable to open temporary "
		"file; please check file permissions in your "
		"directory\n");
		exit(NO_TEMP_FILE);
	}
	token = strtok(filelist,",");
	while (token != NULL) {
		if ((currfp = fopen(token,"r")) == NULL) {
			fprintf(stderr,"minlib:  cannot open file \"%s\" "
			"due to the following error:\n\t%d:  %s\n",
			token,errno,strerror(errno));
			exit(BAD_INPUT_FILE);
		}
		while ((c = fgetc(currfp)) != EOF) {
			fprintf(tmpfile,"%c",c);
		}
		fclose(currfp);
		token = strtok(NULL,",");
	}
	fclose(tmpfile);
	return numrecs;
}
