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

struct vals {
	char tag[24];
	char *value;
};

int print_stats(char **ptr, int numrecs);
int in_db(char *s, struct tags *datalines, int numtags);
int in_full_db(char *s, char *t, struct vals *datavals, int totalrecs);

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
	int i; int j; int k = 0;
	struct tags *datalines;
	struct tags *uniqlines;
	struct vals *datavals;
	char tag[24] = "";
	int numtags = 0;
	int tagind = 0;
	int valind = 0;

	datalines = malloc(numtags * sizeof(struct tags));
	uniqlines = malloc(numtags * sizeof(struct tags));
	datavals = malloc(numtags * sizeof(struct vals));
	printf("TOTAL RECORDS:\t\t%6d\n",numrecs);
	for (i = 0, j = 0; *(ptr+i) != NULL; ++i, ++j) {
		if (strstr(*(ptr+i),"%%"))
			--j;
		if ((j % 2) == 0) {
			strcpy(tag,*(ptr+i));
			if ((tagind = in_db(tag,datalines,numtags)) == -1) {
				++numtags;
				datalines = realloc(datalines,numtags*sizeof(struct tags));
				strcpy(datalines[numtags-1].tag,tag);
				uniqlines = realloc(uniqlines,numtags*sizeof(struct tags));
				strcpy(uniqlines[numtags-1].tag,tag);
				datalines[numtags-1].number = 1;
				uniqlines[numtags-1].number = 1;
				datavals = realloc(datavals,(k+1)*sizeof(struct vals));
				strcpy(datavals[k].tag,tag);
				datavals[k].value = *(ptr+i+1);
				++k;
			} else {
				++(datalines[tagind].number);
				if ((valind = in_full_db(tag,*(ptr+i+1),datavals,k)) == -1) {
					++(uniqlines[tagind].number);
					datavals = realloc(datavals,(k+1) * sizeof(struct vals));
					strcpy(datavals[k].tag,tag);
					datavals[k].value = *(ptr+i+1);
					++k;
				}
			}
		}
	}
	printf("TAGS\t\tNUMBER\t\tUNIQUE\n");
	printf("----\t\t------\t\t------\n");
	for (i = 0; i < numtags; ++i) {
		printf("%.12s:",datalines[i].tag);
		(strlen(datalines[i].tag) < 7) ? printf("\t\t") : printf("\t");
		printf("%6d\t\t",datalines[i].number);
		printf("%6d\n",uniqlines[i].number);
	}
	free(datalines);
	free(uniqlines);
	free(datavals);
	return 0;
}

/* returns number of tag's index if found, -1 if not  */
int in_db(char *s, struct tags *datalines, int numtags)
{
	int i;

	for (i = 0; i < numtags; ++i) {
		if (!strcmp(datalines[i].tag,s)) {
			return i;
		}
	}
	return -1;
}

int in_full_db(char *s, char *t, struct vals *datavals, int totalrecs)
{
	int i;

	for (i = 0; i < totalrecs; ++i) {
		if (!strcmp(datavals[i].tag,s) && !strcmp(datavals[i].value,t)) {
			return i;
		}
	}
	return -1;
}
