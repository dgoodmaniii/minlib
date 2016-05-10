/*
 * +AMDG
 */
/*
 * This document was begun on 9 May 1200, the feast of St.
 * Gregory Nazianzen, ECD, and it is humbly dedicated to
 * him, to St. Wulfric of Haselbury, and to the Immaculate
 * Heart of Mary for their prayers, and to the Sacred Heart
 * of Jesus for His mercy.
*/
/*
 * This file takes the list made from the records in
 * readlib.c and formats the output strings.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int format_recs(char **ptr, char *formstring, char **formed)
{
	int i;
	int j = -1;
	int num;
	int lastrec = 0;
	int currrec = 0;

	j = make_string(ptr,formed,formstring);
	for (i=0; i <= j; ++i)
		free(*(formed+i));
	return 0;
}

/* allocate memory for the formatted strings and fill them */
int make_string(char **raw, char **format, char *formstring)
{
	int i, currrec, lastrec, k = 0;
	int j = -1;

	for (i = 0; *(raw+i) != NULL; ++i) {
		if ((currrec = atoi(*(raw+i))) != 0) {
			lastrec = currrec;
			++j;
			*(format+j) = malloc(75 * sizeof(char));
		} else if (lastrec != currrec) { /*FIXME*/
			get_text(raw,formstring,lastrec);
			new_strcpy(*(format+j),*(raw+i),5);
			printf("%s\n",*(format+j));
		}
	}
	return j;
}

/* gets the text for a given field */
int get_text(char **raw, char **format, char *s, int recnum)
{ /*FIXME*/
	char *t = "%20t";
	int i = 0; int j = 0;
	int recstart = 0;
	int numchars;

	numchars = split_form(t);
/*	for (i = 0; *(raw+i) != NULL; ++i) {
		if (atoi(*(raw+i)) == recnum) {
			recstart = i;
			break;
		}
	}
	printf("%d\n",recnum);
	while ((*(raw+i) != NULL) && ((j = atoi(*(raw+i))) == recnum)) {
		if (strstr("TITLE:\t",*(raw+i))) {
			printf("HERE");
		}
		++i;
	}*/
	return 0;
}

int split_form(char *s)
{
	int i, j = 0;
	char t[4];

	for (i = 0; *(s+i) != '\0'; ++i) {
		if (isdigit(*(s+i)))
			t[j++] = *(s+i);
	}
	t[j] = '\0';
	return atoi(t);
}

int new_strcpy(char *s, char *t, int num)
{
	int i;

	for (i = 0; i < num; ++i)
		*(s+i) = *(t+i);
	*(s+i) = '\0';
	return 0;
}
