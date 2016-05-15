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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include"errcodes.h"

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

	if ((fp = fopen(s,"r")) == NULL) {
		fprintf(stderr,"minlib:  error opening file %s, "
		"with error number %d; see \"man (3) open\" for "
		"details\n",s,errno);
			exit(BAD_FILE);
	}
	while ((c = fgetc(fp)) != EOF)
		if (c == '\n')
			++numlines;
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
		"with error number %d; see \"man (3) open\" for "
		"details\n",s,errno);
			exit(BAD_FILE);
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
	for (i = 0; (*(t+i) != '\0') && (i < num); ++i, ++j)
		*(s+j) = *(t+i);
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

int swap_str_ptrs(int *int1,int *int2,char const **arg1,char const **arg2)
{
	const char *tmp = *arg1;
	*arg1 = *arg2;
	*arg2 = tmp;
	int temp = *int1;
	*int1 = *int2;
	*int2 = temp;
	return 0;
}
