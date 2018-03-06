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
#include<ctype.h>
#include<sys/types.h>
#include"errcodes.h"
#include"utility.h"

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte) \
	(byte & 0x80 ? 1 : 0), \
	(byte & 0x40 ? 1 : 0), \
	(byte & 0x20 ? 1 : 0), \
	(byte & 0x10 ? 1 : 0), \
	(byte & 0x08 ? 1 : 0), \
	(byte & 0x04 ? 1 : 0), \
	(byte & 0x02 ? 1 : 0), \
	(byte & 0x01 ? 1 : 0)

/* number of digits in integer */
int num_digs(int num)
{
	int test = 1;
	int digits = 0;
	while (num >= test) {
		++digits;
		test *= 10;
	}
	return digits+1;
}

/* count the lines in a file, including record seperator */
int count_lines_file(char *s)
{
	int numlines = 0;
	char c;
	FILE *fp;
	char prevchar = ' ';
	int recsep = 0;
	ssize_t read; size_t len = 0;
	char *line = NULL;

	if ((fp = fopen(s,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file %s, "
		"with the following error:\n\t%d:  %s\n",s,errno,
		strerror(errno));
		exit(BAD_INPUT_FILE);
	}
	while ((read = getline(&line,&len,fp)) != -1) {
		if (validate_line(line)) {
			++numlines;
		}
	}
	free(line);
	fclose(fp);
	return numlines;
}

/* count the number of records in a file */
int count_recs_file(char *s)
{
	int numlines = 0;
	char *line = NULL;
	ssize_t read; size_t len = 0;
	FILE *fp;

	if ((fp = fopen(s,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file %s, "
		"with the following error:\n\t%d:  %s\n",s,errno,
		strerror(errno));
		exit(BAD_INPUT_FILE);
	}
	while ((read = getline(&line,&len,fp)) != -1) {
		if (strstr(line,"%%"))
			++numlines;
	}
	fclose(fp);
	free(line);
	return numlines;
}

int chomp(char *s)
{
	int i;
	for (i = 0; *(s+i) != '\n' && *(s+i) != '\0'; ++i);
	if (*(s+i) == '\n')
		*(s+i) = '\0';
	return 0;
}

int front_chomp(char *s)
{
	int len, i;
	len = strlen(s) + 1;
	for (i = 0; isspace(*(s+i)); ++i);
	memmove(s,s+i,strlen(s+i)+1);
	return 0;
}

int new_strsep(char *s, char *t, char c)
{
	int i; int len;
	for (i = 0; *(s+i) != '\0'; ++i) {
		*(t+i) = *(s+i);
		if (*(s+i) == c) {
			break;
		}
	}
	*(t+i) = '\0';
	memmove(s,s+i+1,strlen(s+i)+1);
	front_chomp(s);
	return 0;
}

int new_strcpy(char *s, char *t, int num)
{
	int i;

	for (i = 0; (*(t+i) != '\0') && (i < num); ++i)
		*(s+i) = *(t+i);
	*(s+i) = '\0';
	return 0;
}

int new_strcat(char *s, char *t, int num)
{
	int i, j;

	for (j = 0; *(s+j) != '\0'; ++j);
	for (i = 0; (*(t+i) != '\0') && (i < num); ++i, ++j) {
		*(s+j) = *(t+i);
		if (CHECK_BIT(*(t+i),7) && !CHECK_BIT(*(t+i),6))
			++num;
	}
	for (i = i; i < num; ++i, ++j)
		*(s+j) = ' ';
	*(s+j) = '\0';
	return 0;
}

/* following two functions are modified from a Stack
 * Overflow answer by WhozCraig, on question 19612152. */
int quicksort(char **formrecs, int *recnums, int len)
{
	int i, pvt = 0;
	int j = 0;
	int temp = 0;

	if (len <= 1)
		return 0;
	j = rand() % len;
	swap_str_ptrs(recnums+j,recnums+len-1,formrecs+j,formrecs+len-1);
	for (i = 0; i < len - 1; ++i) {
		if (strcmp(*(formrecs+i),*(formrecs+len-1)) < 0) {
			swap_str_ptrs(recnums+i,recnums+pvt,formrecs+i,formrecs+pvt);
			pvt++;
		}
	}
	swap_str_ptrs(recnums+pvt,recnums+len-1,formrecs+pvt,formrecs+len-1);
	quicksort(formrecs,recnums,pvt++);
	quicksort(formrecs+pvt,recnums+pvt,len-pvt);
	return 0;
}

int swap_str_ptrs(int *int1,int *int2, char **arg1,char **arg2)
{
	char *tmp = *arg1;
	*arg1 = *arg2;
	*arg2 = tmp;
	int temp = *int1;
	*int1 = *int2;
	*int2 = temp;
	return 0;
}

int get_size(char **ptr)
{
	int i = 0;

	for (i = 0; *(ptr+i) != NULL; ++i);
	return i;
}

/* takes the database and line number, then backs up to find
 * the record number */
int get_record_num(char **ptr, int matchedrec)
{
	int i;
	int recnum = -1;

	for (i = matchedrec; i >= 0; --i) {
		if (strstr(*(ptr+i),"%%")) {
			recnum = atoi(*(ptr+i));
			break;
		}
	}
	if (recnum >= 0)
		return recnum;
	else
		return -1;
}

/* returns 1 if valid, 0 if not */
int validate_line(char *s)
{
	int i = 0;

	chomp(s);
	if (!strcmp(s,"%%"))
		return 1;
	while ((isupper(s[i]) || isspace(s[i]) || s[i] == '.') && (s[i] != '\0'))
		++i;
	if (i == 0)
		return 0;
	if (s[i] != ':')
		return 0;
	return 1;
}

/* return 0 if found, 1 if not */
int include_substr(char *s, char *t)
{
	char *token;
	char *copy;

	if ((copy = malloc((strlen(t) + 1) * sizeof(char))) == NULL) {
		fprintf(stderr,"minlib:  insufficient memory for "
		"storing the filetype string\n");
		exit(INSUFF_INTERNAL_MEMORY);
	}
	strcpy(copy,t);
	token = strtok(copy,",");
	while (token != NULL) {
		if (strstr(s,token)) {
			free(copy);
			return 0;
		}
		token = strtok(NULL,",");
	}
	free(copy);
	return 1;
}

/* returns 1 if all blank, 0 is not */
int is_blank_line(char *s)
{
	chomp(s);
	front_chomp(s);
	if (strlen(s) == 0)
		return 1;
	return 0;
}

