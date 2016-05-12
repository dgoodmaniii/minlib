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

/* fills the db from the file; allocates memory */
int fill_db(char **ptr, char *filename)
{
	FILE *fp; char *line = NULL;
	ssize_t read; size_t len = 0;
	int numlines = 0; int numrecs = 0;
	char numstr[12];
	char *token;

	token = malloc(12 * sizeof(char));
	fp = fopen(filename,"r");
	while ((read = getline(&line,&len,fp)) != -1) {
		if (strstr(line,"%%")) {
			numrecs++;
			*(ptr+numlines) = malloc((read+1+num_digs(numrecs)) 
				* sizeof(char));
			sprintf(numstr,"%d",numrecs); strcpy(*(ptr+numlines),numstr);
			strcat(*(ptr+numlines),line); ++numlines;
			continue;
		} else {
			new_strsep(line,token,':');
		}
		*(ptr+numlines) = malloc((strlen(token)+1) * sizeof(char));
		chomp(token);
		strcpy(*(ptr+(numlines++)),token);
		*(ptr+numlines) = malloc((strlen(line)+1) * sizeof(char));
		chomp(line);
		strcpy(*(ptr+numlines),line);
		numstr[0] = '\0';
		++numlines;
	}
	*(ptr+numlines) = NULL;
	fclose(fp);
	free(line);
	free(token);
	return numrecs;
}

/* frees the memory used for the database */
int free_db(char **ptr,int numlines)
{
	int i;

	for (i = 0; i < numlines; ++i)
		free(*(ptr + i));
	return 0;
}