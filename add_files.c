/*
 * +AMDG
 */
/*
 * This document was begun on 1X May 1200, Holy Trinity
 * Sunday, and it is humbly dedicated to the Father, the
 * Son, and the Holy Spirit, for their mercy.
 */

#define _XOPEN_SOURCE 500
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<ftw.h>
#include<unistd.h>
#include<limits.h>
#include<sys/stat.h>
#include"errcodes.h"

/* returns 0 on success, 1 on failure */
int add_files(char *dirname)
{
	int status;
	struct stat st_buf;

	status = stat(dirname,&st_buf);
	if (status != 0) {
		fprintf(stderr,"minlib:  couldn't get status of \"%s\", "
		"with error number %d:\n\t%s",dirname,errno,strerror(errno));
		exit(BAD_ADDED_FILE);
	}
	if (S_ISREG(st_buf.st_mode))
		add_file(dirname);
	else if (S_ISDIR(st_buf.st_mode))
		add_dir(dirname);
	return 0;
}

int add_file(char *s)
{
	fprintf(stderr,"Currently adding %s...\n",s);
	if (strstr(s,".pdf"))
		pdf_metadata(s);
	if (strstr(s,".epub"))
		epub_metadata(s);
	return 0;
}

int descend(const char *fpath, const struct stat *sb,
	int tflag, struct FTW *ftwbuf)
{
	char *s;

	if ((s = malloc((strlen(fpath) + 1) * sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to store "
		"the next point of the directory tree\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	strcpy(s,fpath);
	if (S_ISREG(sb->st_mode))
		add_file(s);
	free(s);
	return 0;
}

/* the following calls add_file for each file it sees */
int add_dir(char *s)
{
	if (nftw(s,descend,20,0) != 0);
	return 0;
}


int epub_metadata(char *s)
{
	int i, j, c;
	char *line = NULL;
	ssize_t read; size_t len = 0;
	FILE *p;
	char *ptr;
	char *t;
	char fhead[MAX_FIELD_LEN+1];
	char text[MAX_TITLE_LEN+1];
	char actpath[MAX_TITLE_LEN+1];
	int ind, oind;

	if ((t = malloc((strlen(s) + 26) * sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to store "
		"command necessary to extra epub metadata\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	*t = '\0';
	printf("%%%%\n");
	strcat(t,"unzip -c ");
	strcat(t,s);
	strcat(t," *content.opf");
	if ((p = popen(t,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file \"%s\" "
		"with error code %d:\n\t%s\n",s,errno,strerror(errno));
		exit(ERROR_OPEN_ADDFILE);
	}
	while ((read = getline(&line,&len,p)) != -1) {
		if (ptr = strstr(line,"dc:")) {
			ind = 0;
			for (i = 3; ptr[i] != '\0' && isalpha(ptr[i]); ++i) {
				fhead[ind++] = ptr[i];
			}
			fhead[ind] = '\0';
			if (!strcmp(fhead,"contributor")) {
				if (strstr(line,"edt"))
					strcpy(fhead,"EDITOR");
			}
			if (!strcmp(fhead,"contributor")) {
				if (strstr(line,"trl"))
					strcpy(fhead,"TRANS");
			}
			if (!strcmp(fhead,"creator"))
				strcpy(fhead,"AUTHOR");
			if (!strcmp(fhead,"AUTHOR") || !strcmp(fhead,"title")
			|| !strcmp(fhead,"language") || !strcmp(fhead,"subject")
			|| !strcmp(fhead,"rights") || !strcmp(fhead,"contributor")
			|| !strcmp(fhead,"EDITOR") ||  !strcmp(fhead,"source")
			|| !strcmp(fhead,"TRANS")) {
				while(line[i++] != '>');
				oind = 0;
				while((line[i] != '<') && (line[i] != '\0'))
					text[oind++] = line[i++];
				text[oind] = '\0';
				for (j = 0; fhead[j] != '\0'; ++j)
					printf("%c",toupper(fhead[j]));
				printf(":\t%s\n",text);
				fhead[0] = '\0'; text[0] = '\0';
			}
		}
	}
	printf("PATH:\t%s\n",realpath(s,actpath));
	free(t);
	free(line);
	pclose(p);
	return 0;
}

int pdf_metadata(char *s)
{
	int i, j, c;
	char *line = NULL;
	ssize_t read; size_t len = 0;
	FILE *p;
	char *ptr;
	char *t;
	char fhead[MAX_FIELD_LEN+1];
	char text[MAX_TITLE_LEN+1];
	char actpath[MAX_TITLE_LEN+1];
	int ind, oind;

	if ((t = malloc((strlen(s) + 16) * sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory to store "
		"command necessary to extra pdf metadata\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	*t = '\0';
	printf("%%%%\n");
	strcat(t,"pdfinfo -meta "); strcat(t,s);
	if ((p = popen(t,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file \"%s\" "
		"with error code %d:\n\t%s\n",s,errno,strerror(errno));
		exit(ERROR_OPEN_ADDFILE);
	}
	fhead[0] = '\0'; text[0] = '\0';
	while ((read = getline(&line,&len,p)) != -1) {
		if (strstr(line,"Title:"))
			strcpy(fhead,"TITLE");
		if (strstr(line,"Author:"))
			strcpy(fhead,"AUTHOR");
		if (strstr(line,"Subject:"))
			strcpy(fhead,"SUBJECT");
		if (strstr(line,"Keywords:"))
			strcpy(fhead,"KEYWORDS");
		if (strstr(line,"Producer:"))
			strcpy(fhead,"PRODUCER");
		if (strstr(line,"CreationDate:"))
			strcpy(fhead,"CREATIONDATE");
		if (strstr(line,"ModDate:"))
			strcpy(fhead,"MODDATE");
		if (strstr(line,"Tagged:"))
			strcpy(fhead,"TAGGED");
		if (strstr(line,"Encrypted:"))
			strcpy(fhead,"ENCRYPTED");
		if (strstr(line,"Page size:"))
			strcpy(fhead,"PAGESIZE");
		if (strstr(line,"Page rot:"))
			strcpy(fhead,"PAGEROT");
		if (strstr(line,"PDF version:"))
			strcpy(fhead,"PDFVERSION");
		if (strstr(line,"Creator:"))
			strcpy(fhead,"CREATOR");
		if (fhead[0] != '\0') {
			ptr = (strstr(line,":")); ++ptr;
			while(isspace(*(ptr))) ++ptr;
			ind = 0;
			for (i = 0; ptr[i] != '\n' && ptr[i] != '\0' 
			&& ind < MAX_TITLE_LEN; ++i)
				text[ind++] = ptr[i];
			text[ind] = '\0';
			printf("%s:\t%s\n",fhead,text);
			fhead[0] = '\0'; text[0] = '\0';
		}
	}
	printf("PATH:\t%s\n",realpath(s,actpath));
	free(line);
	pclose(p);
	return 0;
}