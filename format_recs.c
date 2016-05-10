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

struct fields {
	char c;
	char s[12];
} fields[] = { 
	't',"TITLE",
	'a',"AUTHOR",
	's',"SUBJECT"
};

int format_recs(char **ptr, char *formstring, char **formed, int linenums)
{
	int i;
	int j = -1;
	int num;
	int lastrec = 0;
	int currrec = 0;

	for (i = 0; i < linenums; ++i) {
		*(formed+i) = malloc(75 * sizeof(char));
		*(*(formed+i+0)) = '\0';
	}
	j = make_string(ptr,formed,formstring);
	for (i=0; i <= linenums; ++i)
		free(*(formed+i));
	return 0;
}

/* allocate memory for the formatted strings and fill them */
int make_string(char **raw, char **format, char *formstring)
{
	int i, j = 0;
	int recnum;
	int subrec = 0;
	int lastrec = 0;
	char s[12];

	for (i = 0; *(raw+i) != NULL; ++i) {
		if (strstr(*(raw+i),"%%")) {
			recnum = atoi(*(raw+i)) - 1;
			lastrec = recnum;
			++j;
			continue;
		}
		new_strcat(*(format+recnum),*(raw+i),5);
	} /* FIXME */
	for (i = 0; i <= recnum; ++i)
		printf("%d:  %s\n",i,*(format+i));
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
