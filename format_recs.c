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

int populate(char **raw, int stind, char letter);

int format_recs(char **ptr, char *formstring, char **formed, int linenums, int *recnums)
{
	int i;
	int j = -1;
	int num;
	int lastrec = 0;
	int currrec = 0;

	j = make_string(ptr,formed,formstring);
	quicksort(formed,recnums,j);
	return 0;
}

/* allocate memory for the formatted strings and fill them */
int make_string(char **raw, char **format, char *formstring)
{
	int i, j, k, l = 0;
	int recnum;
	int num = 0;
	int lastrec = 0;
	int corrrec = 0;
	char buf[12];

	for (i = 0; *(raw+i) != NULL; ++i) {
		if (strstr(*(raw+i),"%%")) {
			recnum = atoi(*(raw+i)) - 1;
			lastrec = recnum;
			++j;
			for (k = 0; *(formstring+k) != '\0'; ++k) {
				if (*(formstring+k) == '%') {
					while (isdigit(*(formstring+(++k))))
						buf[l++] = *(formstring+k);
					buf[l] = '\0';
					num = atoi(buf);
					corrrec = populate(raw,i+1,*(formstring+k));
					if (corrrec != 0)
						new_strcat(*(format+recnum),*(raw+corrrec),num);
					else
						new_strcat(*(format+recnum)," ",num);
					l = 0;
				} else {
					new_strcat(*(format+recnum),formstring+k,1);
				}
			}
		}
	}
	return recnum;
}

int populate(char **raw, int stind, char letter)
{
	for (stind = stind; (*(raw+stind) != NULL) && 
	(!strstr(*(raw+stind),"%%"));++stind) {
		if (letter == 't') {
			if (!strcmp(*(raw+stind),"TITLE"))
				return stind+1;
		} else if (letter == 'a') {
			if (!strcmp(*(raw+stind),"AUTHOR"))
				return stind+1;
		} else if (letter == 'y') {
			if (!strcmp(*(raw+stind),"YEAR"))
				return stind+1;
		} else if (letter == 'l') {
			if (!strcmp(*(raw+stind),"LANG"))
				return stind+1;
		} else if (letter == 'g') { /* "g"enre */
			if((!strcmp(*(raw+stind),"TYPE"))||(!strcmp(*(raw+stind),"GENRE")))
				return stind+1;
		} else if (letter == 'p') {
			if (!strcmp(*(raw+stind),"PUBLISHER"))
				return stind+1;
		} else if (letter == 'L') {
			if (!strcmp(*(raw+stind),"AUTHLAST"))
				return stind+1;
		} else if (letter == 'M') {
			if (!strcmp(*(raw+stind),"AUTHMID"))
				return stind+1;
		} else if (letter == 'F') {
			if (!strcmp(*(raw+stind),"AUTHFIRST"))
				return stind+1;
		}
	}
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
