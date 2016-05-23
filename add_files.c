/*
 * +AMDG
 */
/*
 * This document was begun on 1X May 1200, Holy Trinity
 * Sunday, and it is humbly dedicated to the Father, the
 * Son, and the Holy Spirit, for their mercy.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
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
	if (strstr(s,".pdf"))
		pdf_metadata(s);
	return 0;
}

int get_pdf_meta(char *s);

/* the following calls add_file for each file it sees */
int add_dir(char *s)
{
	return 0;
}

int pdf_metadata(char *s)
{
	int i, j, c;
	int curr_byte = 0;
	int ind = 0; int oind = 0;
	FILE *fp;
	char fhead[MAX_FIELD_LEN+1];
	char text[MAX_TITLE_LEN+1];
	char actpath[MAX_TITLE_LEN+1];
	struct stat st;
	off_t file_size;

	if ((fp = fopen(s,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file \"%s\" "
		"with error code %d:\n\t%s\n",s,errno,strerror(errno));
		exit(ERROR_OPEN_ADDFILE);
	}
	printf("%%%%\n");
	if ((fstat(fileno(fp),&st) != 0)) {
		fprintf(stderr,"minlib:  cannot determine size of file "
		"%s\n",s);
		exit(ERROR_SIZE_ADDFILE);
	}
	file_size = st.st_size;
	while (curr_byte < file_size) {
		++curr_byte;
		c = fgetc(fp);
		if ((c == '%') && ((c = fgetc(fp)) == '%') && ((c =
		fgetc(fp)) == 'E') && ((c = fgetc(fp)) == 'O') && ((c
		= fgetc(fp)) == 'F') && ((c = fgetc(fp)) != '\n')) {
			break;
		}
		if (c == '/') {
			while(isalpha(c = fgetc(fp)) && (ind < MAX_FIELD_LEN))
				fhead[ind++] = c;
			fhead[ind] = '\0';
			ind = 0;
			if (!strcmp(fhead,"Title") || !strcmp(fhead,"Author") || 
			!strcmp(fhead,"Subject") || !strcmp(fhead,"Keywords") ||
			!strcmp(fhead,"Producer")) {
				if (c != '(')
					while ((c = fgetc(fp)) != '(');
				while (oind < MAX_TITLE_LEN) {
					if ((c = fgetc(fp)) == '\\') {
						text[oind++] = fgetc(fp);
					} else if (c == ')') {
						break;
					} else {
						text[oind++] = c;
					}
				}
				text[oind] = '\0';
				for (j = 0; fhead[j] != '\0'; ++j)
					printf("%c",toupper(fhead[j]));
				printf(":\t%s\n",text);
				ind = 0; oind = 0;
				fhead[0] = '\0'; text[0] = '\0';
			}
		}
	}
	printf("PATH:\t%s\n",realpath(s,actpath));
	close(fp);
	return 0;
}
