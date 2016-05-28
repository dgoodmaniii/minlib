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
#include<extractor.h>
#include"errcodes.h"

/* function that libextractor passes its data to */
int print_metadata(void *cls,const char *plugin_name, enum
	EXTRACTOR_MetaType type, enum EXTRACTOR_MetaFormat format,
	const char *data_mime_type,const char *data, size_t
	data_len);

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
	if (strstr(s,".pdf")) {
		fprintf(stderr,"Processing %s...\n",s);
		pdf_metadata(s);
	} else if (strstr(s,".epub")) {
		fprintf(stderr,"Processing %s...\n",s);
		epub_metadata(s);
	} else if (strstr(s,".dvi") || strstr(s,".flac") ||
	strstr(s,".html") || strstr(s,".midi") ||
	strstr(s,".mpeg") || strstr(s,".odf") || strstr(s,".ogg")
	|| strstr(s,".ps") || strstr(s,".riff") ||
	strstr(s,".wav") || strstr(s,".doc") || strstr(s,".xls")
	|| strstr(s,".ppt") || strstr(s,".sxw") ||
	strstr(s,".mp3") || strstr(s,".s3m") || strstr(s,".nsf")
	|| strstr(s,".sid") || strstr(s,".real") ||
	strstr(s,".flv") || strstr(s,".avi") || strstr(s,".qt")
	|| strstr(s,".asf") || strstr(s,".ogv") || strstr(s,".ogt")) {
		fprintf(stderr,"Processing %s...\n",s);
		multi_metadata(s);
	} else {
		fprintf(stderr,"%s not a minlib format; skipping...\n",s);
	}
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
				else if (strstr(line,"trl"))
					strcpy(fhead,"TRANS");
				else if (strstr(line,"adp"))
					strcpy(fhead,"ADAPTER");
				else if (strstr(line,"arr"))
					strcpy(fhead,"ARRANGER");
				else if (strstr(line,"art"))
					strcpy(fhead,"ARTIST");
				else if (strstr(line,"aqt"))
					strcpy(fhead,"QUOTEAUTH");
				else if (strstr(line,"aft"))
					strcpy(fhead,"AFTERWORD");
				else if (strstr(line,"aui"))
					strcpy(fhead,"INTRO");
				else if (strstr(line,"ant"))
					strcpy(fhead,"ANTEC.");
				else if (strstr(line,"bkp"))
					strcpy(fhead,"PRODUCER");
				else if (strstr(line,"clb"))
					strcpy(fhead,"COLLAB.");
				else if (strstr(line,"cmm"))
					strcpy(fhead,"COMM.OR");
				else if (strstr(line,"dsr"))
					strcpy(fhead,"DESIGNER");
				else if (strstr(line,"ill"))
					strcpy(fhead,"ILLUS.OR");
				else if (strstr(line,"lyr"))
					strcpy(fhead,"LYRICIST");
				else if (strstr(line,"mus"))
					strcpy(fhead,"MUSICIAN");
				else if (strstr(line,"nar"))
					strcpy(fhead,"NARRATOR");
				else if (strstr(line,"pht"))
					strcpy(fhead,"PHOTO.ER");
				else if (strstr(line,"prt"))
					strcpy(fhead,"PRINTER");
				else if (strstr(line,"red"))
					strcpy(fhead,"REDACTOR");
				else if (strstr(line,"rev"))
					strcpy(fhead,"REVIEWER");
				else if (strstr(line,"spn"))
					strcpy(fhead,"SPONSOR");
				else if (strstr(line,"ths"))
					strcpy(fhead,"ADVISOR");
				else if (strstr(line,"trc"))
					strcpy(fhead,"TRANSCR.ER");
				else
					strcpy(fhead,"OTHER");
			}
			if (!strcmp(fhead,"identifier")) {
				if (strstr(line,"ISBN"))
					strcpy(fhead,"ISBN");
			}
			if (!strcmp(fhead,"creator"))
				strcpy(fhead,"AUTHOR");
			if (!strcmp(fhead,"AUTHOR") || !strcmp(fhead,"title")
			|| !strcmp(fhead,"language") || !strcmp(fhead,"subject")
			|| !strcmp(fhead,"rights") || !strcmp(fhead,"contributor")
			|| !strcmp(fhead,"EDITOR") ||  !strcmp(fhead,"source")
			|| !strcmp(fhead,"TRANS") || !strcmp(fhead,"ISBN")
			|| !strcmp(fhead,"description") || !strcmp(fhead,"publisher")
			|| !strcmp(fhead,"ADAPTER") || !strcmp(fhead,"ARRANGER")
			|| !strcmp(fhead,"ARTIST") || !strcmp(fhead,"QUOTEAUTH")
			|| !strcmp(fhead,"AFTERWORD") || !strcmp(fhead,"INTRO")
			|| !strcmp(fhead,"ANTEC.") || !strcmp(fhead,"PRODUCER")
			|| !strcmp(fhead,"COLLAB.") || !strcmp(fhead,"COMM.OR")
			|| !strcmp(fhead,"DESIGNER") || !strcmp(fhead,"ILLUS.OR")
			|| !strcmp(fhead,"LYRICIST") || !strcmp(fhead,"MUSICIAN")
			|| !strcmp(fhead,"NARRATOR") || !strcmp(fhead,"PHOTO.ER")
			|| !strcmp(fhead,"PRINTER") || !strcmp(fhead,"REDACTOR")
			|| !strcmp(fhead,"REVIEWER") || !strcmp(fhead,"SPONSOR")
			|| !strcmp(fhead,"ADVISOR") || !strcmp(fhead,"TRANSCR.ER")
			|| !strcmp(fhead,"OTHER") || !strcmp(fhead,"TRANSCR.ER")
			|| !strcmp(fhead,"date") || !strcmp(fhead,"type")
			|| !strcmp(fhead,"format") || !strcmp(fhead,"relation")
			|| !strcmp(fhead,"coverage")) {
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

int multi_metadata(char *s)
{
	char actpath[MAX_TITLE_LEN+1];
	struct EXTRACTOR_PluginList *plugins =
		EXTRACTOR_plugin_add_defaults (EXTRACTOR_OPTION_DEFAULT_POLICY);
	printf("%%%%\n");
	EXTRACTOR_extract (plugins, s,
		NULL,0,&print_metadata,stdout);
	EXTRACTOR_plugin_remove_all(plugins);
	printf("PATH:\t%s\n",realpath(s,actpath));
	return 0;
}

int print_metadata(void *cls,const char *plugin_name, enum
EXTRACTOR_MetaType type, enum EXTRACTOR_MetaFormat format,
const char *data_mime_type,const char *data, size_t
data_len)
{
	int i;

	for (i = 0; EXTRACTOR_metatype_to_string(type)[i] != '\0'; ++i) {
		printf("%c",toupper(EXTRACTOR_metatype_to_string(type)[i]));
	}
	printf(":  ");
	for (i = 0; data[i] != '\0'; ++i)
		if (data[i] != '\n')
		printf("%c",data[i]);
	printf("\n");
	return 0;
}
