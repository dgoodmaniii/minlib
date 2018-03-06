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

/* to get getline() defined */
#define _POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700
#define _GNU_SOURCE

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include"errcodes.h"
#include"options.h"
#include"utility.h"
#include"read_optfile.h"

int read_optfile(char **filename, char **formstring, char *configname,
	struct options **globopts)
{
	FILE *fp;
	char *line = NULL;
	ssize_t read; size_t len = 0;
	char *ptr;
	char *newconfname;
	int optlen;

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
		free(newconfname);
		fill_def_colors(globopts);
		fill_def_apps(globopts);
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
		if (strstr(line,"RECORDFORM:")) {
			ptr = line+12;
			while (isspace(*ptr)) ++ptr;
			if ((*formstring = malloc((strlen(ptr)+1)*sizeof(char))) == NULL) {
				fprintf(stderr,"minlib:  insufficient memory to "
				"store recordform string specified in config file\n");
				exit(INSUFF_MEMORY_FORMSTRING);
			}
			strcpy(*formstring,ptr);
			chomp(*formstring);
		}
		if (strstr(line,"TOP_FORE_COLOR:")) {
			get_color(line,TOP_FORE_COLOR,globopts);
		} if (strstr(line,"TOP_BACK_COLOR:")) {
			get_color(line,TOP_BACK_COLOR,globopts);
		} if (strstr(line,"BOT_FORE_COLOR:")) {
			get_color(line,BOT_FORE_COLOR,globopts);
		} if (strstr(line,"BOT_BACK_COLOR:")) {
			get_color(line,BOT_BACK_COLOR,globopts);
		} if (strstr(line,"MEN_FORE_COLOR:")) {
			get_color(line,MEN_FORE_COLOR,globopts);
		} if (strstr(line,"MEN_BACK_COLOR:")) {
			get_color(line,MEN_BACK_COLOR,globopts);
		} if (strstr(line,"DET_FIELD_FORE_COLOR:")) {
			get_color(line,DET_FIELD_FORE_COLOR,globopts);
		} if (strstr(line,"DET_FIELD_BACK_COLOR:")) {
			get_color(line,DET_FIELD_BACK_COLOR,globopts);
		} if (strstr(line,"DET_TXT_FORE_COLOR:")) {
			get_color(line,DET_TXT_FORE_COLOR,globopts);
		} if (strstr(line,"DET_TXT_BACK_COLOR:")) {
			get_color(line,DET_TXT_BACK_COLOR,globopts);
		} if (strstr(line,"DET_BACK_COLOR:")) {
			get_color(line,DET_BACK_COLOR,globopts);
		} if (strstr(line,"DEF_PDF_VIEWER:")) {
			get_app(line,PDF_VIEWER,globopts);
		} if (strstr(line,"DEF_HTML_VIEWER:")) {
			get_app(line,HTML_VIEWER,globopts);
		} if (strstr(line,"DEF_EPUB_VIEWER:")) {
			get_app(line,EPUB_VIEWER,globopts);
		} if (strstr(line,"DEF_OGV_VIEWER:")) {
			get_app(line,OGV_VIEWER,globopts);
		} if (strstr(line,"DEF_OGG_VIEWER:")) {
			get_app(line,OGG_VIEWER,globopts);
		} if (strstr(line,"DEF_DVI_VIEWER:")) {
			get_app(line,DVI_VIEWER,globopts);
		} if (strstr(line,"DEF_PS_VIEWER:")) {
			get_app(line,PS_VIEWER,globopts);
		} if (strstr(line,"DEF_OFFICE_VIEWER:")) {
			get_app(line,OFFICE_VIEWER,globopts);
		}
	}
	fill_def_colors(globopts);
	fill_def_apps(globopts);
	free(newconfname);
	free(line);
	return 0;
}

int fill_def_apps(struct options **globopts)
{
	int i = NUM_COLORS;

	if (!strcmp((*globopts+NUM_COLORS)->optval,""))
		get_app("DEFAULT:  xpdf \"%s\"",NUM_COLORS,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+1))->optval,""))
		get_app("DEFAULT:  firefox \"%s\"",NUM_COLORS+1,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+2))->optval,""))
		get_app("DEFAULT:  fbreader \"%s\"",NUM_COLORS+2,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+3))->optval,""))
		get_app("DEFAULT:  vlc \"%s\"",NUM_COLORS+3,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+4))->optval,""))
		get_app("DEFAULT:  ogg123 \"%s\"",NUM_COLORS+4,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+5))->optval,""))
		get_app("DEFAULT:  xdvi \"%s\"",NUM_COLORS+5,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+6))->optval,""))
		get_app("DEFAULT:  gs \"%s\"",NUM_COLORS+6,globopts);
	if (!strcmp((*globopts+(NUM_COLORS+7))->optval,""))
		get_app("DEFAULT:  libreoffice \"%s\"",NUM_COLORS+7,globopts);
	return 0;
}

int get_app(char *s, int ind,struct options **globopts)
{
	char *ptr;
	int optlen;

	ptr = strchr(s,':'); ++ptr;
	while (isspace(*ptr)) ++ptr;
	optlen = strlen(ptr);
	if (((*globopts+ind)->optval = malloc((optlen+1) *
	sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory "
		"to store requested custom color name\n");
	} else {
		strcpy((*globopts+ind)->optval,ptr);
		chomp((*globopts+ind)->optval);
	}
}

int fill_def_colors(struct options **globopts)
{
	int i;

	for (i = 0; i < NUM_COLORS; ++i) {
		if (!strcmp((*globopts+i)->optval,"")) {
			if (i == TOP_FORE_COLOR)
				get_color("DEFAULT: COLOR_BLACK",TOP_FORE_COLOR,globopts);
			if (i == TOP_BACK_COLOR)
				get_color("DEFAULT: COLOR_WHITE",TOP_BACK_COLOR,globopts);
			if (i == BOT_FORE_COLOR)
				get_color("DEFAULT: COLOR_BLACK",BOT_FORE_COLOR,globopts);
			if (i == BOT_BACK_COLOR)
				get_color("DEFAULT: COLOR_WHITE",BOT_BACK_COLOR,globopts);
			if (i == MEN_FORE_COLOR)
				get_color("DEFAULT: COLOR_WHITE",MEN_FORE_COLOR,globopts);
			if (i == MEN_BACK_COLOR)
				get_color("DEFAULT: COLOR_BLACK",MEN_BACK_COLOR,globopts);
			if (i == DET_FIELD_FORE_COLOR)
				get_color("DEFAULT: COLOR_WHITE",DET_FIELD_FORE_COLOR,globopts);
			if (i == DET_FIELD_BACK_COLOR)
				get_color("DEFAULT: COLOR_BLACK",DET_FIELD_BACK_COLOR,globopts);
			if (i == DET_TXT_FORE_COLOR)
				get_color("DEFAULT: COLOR_WHITE",DET_TXT_FORE_COLOR,globopts);
			if (i == DET_TXT_BACK_COLOR)
				get_color("DEFAULT: COLOR_BLACK",DET_TXT_BACK_COLOR,globopts);
			if (i == DET_BACK_COLOR)
				get_color("DEFAULT: COLOR_BLACK",DET_BACK_COLOR,globopts);
		}
	}
	return 0;
}

int get_color(char *s, int ind,struct options **globopts)
{
	char *ptr;
	int optlen;

	ptr = strchr(s,':'); ++ptr;
	while (isspace(*ptr)) ++ptr;
	optlen = strlen(ptr);
	if (!is_color(ptr)) {
		fprintf(stderr,"minlib:  bad color definition "
		"\"%s\"; see the manual for acceptable "
		"colors\n",ptr);
	} else {
		if (((*globopts+ind)->optval = malloc((optlen+1) *
		sizeof(char))) == NULL) {
			fprintf(stderr,"minlib:  insufficient memory "
			"to store requested custom color name\n");
		} else {
			strcpy((*globopts+ind)->optval,ptr);
			chomp((*globopts+ind)->optval);
		}
	}
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

int is_color(char *s)
{
	if (strcmp(s,"COLOR_BLACK") || strcmp(s,"COLOR_RED") ||
	strcmp(s,"COLOR_GREEN") || strcmp(s,"COLOR_YELLOW") ||
	strcmp(s,"COLOR_BLUE") || strcmp(s,"COLOR_MAGENTA") ||
	strcmp(s,"COLOR_CYAN") || strcmp(s,"COLOR_WHITE"))
		return 1;
	else
		return 0;
}
