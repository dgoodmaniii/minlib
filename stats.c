/*
 * +AMDG
 */
/*
 * This document was begun on 18 June 1200, the Comm. of St.
 * Silverius, PM, and it is humbly dedicated to him and to
 * the Immaculate Heart of Mary for their prayers, and to
 * the Sacred Heart of Jesus, for His mercy.
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct tags {
	char tag[24];
	int number;
};

int stats_report(int *recnums, char **ptr, char **formlist,
					int numrecs, int statsval)
{
	int i;

	for (i = 0; i < numrecs; ++i)
		printf("%s\n",*(formlist+i));
	if (statsval == 1)
		print_stats(ptr,numrecs);
	return 0;
}

int print_stats(char **ptr, int numrecs)
{
	int i; int j;
	struct tags *datalines;
	char tag[24] = "";
	int numtags = 0;
	int tagind = 0;

	datalines = malloc(numtags * sizeof(struct tags));
	printf("TOTAL RECORDS:\t\t%4d\n",numrecs);
	printf("TAG STATISTICS:\n");
	for (i = 0, j = 0; *(ptr+i) != NULL; ++i, ++j) {
		if (strstr(*(ptr+i),"%%"))
			--j;
		if ((j % 2) == 0) {
			strcpy(tag,*(ptr+i));
			if ((tagind = in_db(tag,datalines,numtags)) == -1) {
				++numtags;
				datalines = realloc(datalines,numtags*sizeof(struct tags));
				strcpy(datalines[numtags-1].tag,tag);
				datalines[numtags-1].number = 1;
			} else {
				++datalines[tagind].number;
			}
		}
	}
	for (i = 0; i < numtags; ++i) {
		printf("\t%s:",datalines[i].tag);
		(strlen(datalines[i].tag) < 7) ? printf("\t\t") : printf("\t");
		printf("%4d\n",datalines[i].number);
	}
	printf("UNIQUE VALUES:\n");
	return 0;
}

/* returns number of tag's index if found, -1 if not  */
int in_db(char *s, struct tags *datalines,int numtags)
{
	int i;

	for (i = 0; i <= numtags; ++i) {
		if (!strcmp(datalines[i].tag,s)) {
			return i;
		}
	}
	return -1;
}
