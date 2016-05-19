/*
 * +AMDG
 */
/*
 * This document was begun on 17 May 1200, the feast of St.
 * Peter Celestine, PC, and the commemoration of St.
 * Pudentiana, V, and it is humbly dedicated to them and to
 * the Immaculate Heart of Mary for their prayers, and to
 * the Sacred Heart of Jesus for His mercy.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"errcodes.h"

extern struct options globopts;

int read_optfile(char *filename, char *formstring, char *configname)
{
	FILE *fp;
	char *home;
	char cwd[3] = "./";
	int preflen;
	char *defconfname = "/.minlibrc";
	char *line = NULL;
	ssize_t read; size_t len = 0;

	if (configname == NULL) {
		home = getenv("HOME");
		if (home == NULL) {
			preflen = 2;
		} else {
			preflen = strlen(home);
		}
		if ((configname = malloc((preflen+strlen(defconfname) + 1) * 
		sizeof(char))) == NULL) {
			fprintf(stderr,"minlib:  insufficient memory to "
			"store the name of the config file\n");
			exit(INSUFF_INTERNAL_MEMORY);
		}
		if (home == NULL) {
			strcpy(configname,cwd); strcpy(configname,defconfname);
		} else {
			strcpy(configname,home); strcpy(configname,defconfname);
		}
	}
	fprintf(stderr,"HOME:  %s\n",home);
	if ((fp = fopen(configname,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file %s, "
		"with error number %d; see \"man (3) open\" for "
		"details\n",configname,errno);
			exit(BAD_CONFIG_FILE);
	}
	while ((read = getline(&line,&len,fp)) != -1) {
		if (strstr(line,"DATAFILE:") && (filename == NULL)) {
			
		}
		if (strstr(line,"RECORDFORM:") && (formstring == NULL)) {
		}
	}
	return 0;
}
