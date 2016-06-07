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
#include<locale.h>
#include<langinfo.h>
#include"errcodes.h"
#include"options.h"

struct options *globopts;

int main(int argc, char **argv)
{
	char *locale;
	locale = setlocale(LC_ALL,"");
	globopts = malloc((NUM_COLORS + NUM_APPS) * sizeof(struct options));
	assign_to_opts(TOP_FORE_COLOR);
	assign_to_opts(TOP_BACK_COLOR);
	assign_to_opts(BOT_FORE_COLOR);
	assign_to_opts(BOT_BACK_COLOR);
	assign_to_opts(MEN_FORE_COLOR);
	assign_to_opts(MEN_BACK_COLOR);
	assign_to_opts(DET_FIELD_FORE_COLOR);
	assign_to_opts(DET_FIELD_BACK_COLOR);
	assign_to_opts(DET_TXT_FORE_COLOR);
	assign_to_opts(DET_TXT_BACK_COLOR);
	assign_to_opts(DET_BACK_COLOR);
	assign_to_opts(PDF_VIEWER);
	assign_to_opts(HTML_VIEWER);
	assign_to_opts(EPUB_VIEWER);
	assign_to_opts(OGV_VIEWER);
	assign_to_opts(OGG_VIEWER);
	assign_to_opts(DVI_VIEWER);
	assign_to_opts(PS_VIEWER);
	assign_to_opts(OFFICE_VIEWER);
	char **ptr; char **formlist; int *recnums;
	int numlines = 0;
	int numrecs = 0;
	int i = 0;
	char c;
	char deffile[] = "";
	char defform[] = "%30t | %24a | %8l | %10g";
	char *formstring = NULL;
	char *filename;
	char template[] = "mlibtmpXXXXXX";
	int fdval = 0; /* whether we've done a command-line file */
	int fsval = 0; /* whether we've done a command-line format */
	int didconfigfile = 1;
	int add_succ;
	char **args;

	args = argv;
	opterr = 0;
	if ((filename = malloc((strlen(deffile)+1)*sizeof(char)))==NULL) {
		fprintf(stderr,"minlib:  insufficient memory for "
		"default filename string\n");
		exit(INSUFF_MEMORY_FILENAME);
	}
	strcpy(filename,deffile);
	while ((c = getopt(argc,argv,"Vf:r:c:a:")) != -1) {
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
		case 'a':
			add_succ = add_files(optarg);
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
			fsval = 1;
			break;
		case 'f':
			get_input_filename(ptr,optarg,&filename,template);
			fdval = 1;
			break;
		case 'c':
			didconfigfile=read_optfile(&filename,&formstring,optarg,&globopts);
			break;
		case '?':
			if ((optopt == 'f') || (optopt == 'r') || (optopt == 'c')
			|| (optopt == 'a')) {
				fprintf(stderr,"minlib:  option \"%c\" requires "
				"an argument\n",optopt);
				exit(NEED_ARGUMENT_ARG);
			}
			fprintf(stderr,"minlib:  unrecognized option \"%c\"\n",optopt);
			exit(BAD_OPTION);
			break;
		}
	}
	if (didconfigfile == 1)
		read_optfile(&filename,&formstring,NULL,&globopts);
	if (!strcmp(filename,"")) {
		fprintf(stderr,"minlib:  no input file specified\n");
		exit(NO_DATA_FILE);
	}
	if (fdval == 0)
		get_input_filename(ptr,filename,&filename,template);
	if (formstring == NULL) {
		if ((formstring = malloc((strlen(defform)+1)*sizeof(char)))==NULL) {
			fprintf(stderr,"minlib:  insufficient memory for "
			"default format string\n");
			exit(INSUFF_MEMORY_FORMSTRING);
		}
		strcpy(formstring,defform);
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
	for (i = 0; i < numrecs; ++i) { /* FIXME size of malloc */
		*(formlist+i) = malloc(630 * sizeof(char));
		*(*(formlist+i+0)) = '\0';
	}
	format_recs(ptr,formstring,formlist,numrecs,recnums);
/*	for (i = 0; i < numrecs; ++i)
		printf("%3d: %s\n",*(recnums+i),*(formlist+i));*/
	if (load_gui(ptr,formlist,recnums,numrecs,globopts) == 1) {
		fprintf(stderr,"ONE:  %s\n",args[0]);
		for (i = 0; i < argc; ++i)
			printf("\t%s\n",args[i]);
		execv(args[0],args);
	}
	free_db(ptr,numlines*2-count_recs_file(filename));
	free(ptr);
	for (i=0; i <= numrecs; ++i)
		free(*(formlist+i));
	free(formlist);
	free(recnums);
	free(formstring);
	free(filename);
	for (i = 0; i < (NUM_COLORS + NUM_APPS); ++i)
		free((globopts+i)->optval);
	free(globopts);
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

int get_input_filename(char **ptr,char *namestr,char **filename, 
char *template)
{
	int fdval = 0;
	if (strstr(namestr,",")) {
		fdval = mkstemp(template);
		readfile(ptr,namestr,*filename,fdval);
		if (strlen(template) > strlen(*filename)) {
			if ((*filename = realloc(*filename,
			(strlen(template)+1)*sizeof(char)))==NULL) {
				fprintf(stderr,"minlib:  insufficient memory for "
				"*filename\n");
				exit(INSUFF_MEMORY_FILENAME);
			}
		}
		strcpy(*filename,template);
	} else {
		if (strlen(namestr) > strlen(*filename)) {
			if ((*filename = realloc(*filename,
			(strlen(namestr)+1)*sizeof(char)))==NULL) {
				fprintf(stderr,"minlib:  insufficient memory for "
				"*filename specified with -f\n");
				exit(INSUFF_MEMORY_FILENAME);
			}
		}
		strcpy(*filename,namestr);
	}
	return 0;
}

int assign_to_opts(int ind)
{
	(globopts+ind)->optcode = ind;
	(globopts+ind)->optval = "";
	return 0;
}
