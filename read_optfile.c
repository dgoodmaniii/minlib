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

int read_optfile(char **filename, char *formstring, char *configname)
{
	FILE *fp;
	char *line = NULL;
	ssize_t read; size_t len = 0;
	char *ptr;
	char *newconfname;

	if (configname == NULL) {
		get_configname(&newconfname);
	} else {
		if ((newconfname=malloc((strlen(configname)+1)*sizeof(char)))==NULL) {
			fprintf(stderr,"minlib:  insufficient memory to store "
			"the configuration file name\n");
			exit(INSUFF_INTERNAL_MEMORY);
		}
		strcpy(newconfname,configname);
	}
	if ((fp = fopen(newconfname,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file %s, "
		"with the following error:\n\t%d: %s\n",newconfname,
		errno,strerror(errno));
		return 1;
	}
	while ((read = getline(&line,&len,fp)) != -1) {
		if (strstr(line,"DATAFILE:")) {
			ptr = line+9;
			while (isspace(*ptr)) ++ptr;
			if ((*filename = realloc(*filename,(strlen(ptr)+1) * 
			sizeof(char))) == NULL) {
				fprintf(stderr,"minlib:  insufficient memory to "
				"store datafile specified in config file\n");
				exit(INSUFF_MEMORY_FILENAME);
			}
			strcpy(*filename,ptr);
			chomp(*filename);
		}
		if (strstr(line,"RECORDFORM:") && !strcmp(formstring,"")) {
			ptr = line+12;
			while (isspace(*ptr)) ++ptr;
			if ((formstring = malloc((strlen(ptr)+1)*sizeof(char))) == NULL) {
				fprintf(stderr,"minlib:  insufficient memory to "
				"store recordform string specified in config file\n");
				exit(INSUFF_MEMORY_FORMSTRING);
			}
			strcpy(formstring,ptr);
			fprintf(stderr,"RECORDFORM:  %s\n",formstring);
		}
	}
	free(newconfname);
	free(line);
	return 0;
}

int get_configname(char **newconfname)
{
	char *home;
	char cwd[3] = "./";
	int preflen;
	char *defconfname = "/.minlibrc";

	home = getenv("HOME");
	if (home == NULL) {
		preflen = 2;
	} else {
		preflen = strlen(home) + 1;
	}
	if ((*newconfname = malloc((preflen+strlen(defconfname) + 1) * 
	sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to "
		"store the name of the config file\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	**newconfname = '\0';
	if (home == NULL) {
		strcat(*newconfname,cwd); strcat(*newconfname,defconfname);
	} else {
		strcat(*newconfname,home); strcat(*newconfname,defconfname);
	}
	return 0;
}
