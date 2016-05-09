/* +AMDG */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char **ptr;
	int numlines = 0;
	int numrecs = 0;
	int i = 0;
	char filename[] = "text";

	numlines = count_lines_file(filename);
	ptr = malloc(numlines * sizeof(char*));
	numrecs = fill_db(ptr,filename);
	for (i = 0; i < count_lines_file(filename); ++i) {
		printf("%s",*(ptr+i));
	}
	free_db(ptr,numlines);
	free(ptr);
	printf("%d\n",numrecs);
	printf("%d\n",numlines);
	return 0;
}

/* fills the db from the file; allocates memory */
int fill_db(char **ptr, char *filename)
{
	FILE *fp; char *line = NULL;
	ssize_t read; size_t len = 0;
	int numlines = 0; int numrecs = 0;
	char numstr[12];

	fp = fopen(filename,"r");
	while ((read = getline(&line,&len,fp)) != -1) {
		if (strstr(line,"%%")) {
			numrecs++;
			*(ptr+numlines) = malloc((read+1+num_digs(numrecs)) 
				* sizeof(char));
			sprintf(numstr,"%d",numrecs); strcpy(*(ptr+numlines),numstr);
			strcat(*(ptr+numlines),line); ++numlines;
			continue;
		}
		*(ptr+numlines) = malloc((read+1) * sizeof(char));
		strcpy(*(ptr+numlines),line);
		++numlines;
		numstr[0] = '\0';
	}
	fclose(fp);
	free(line);
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

/* count the lines in a file, not including record seperator */
int count_lines_file(char *s)
{
	int numlines = 0;
	char c;
	FILE *fp;
	char prevchar = ' ';
	int recsep = 0;

	fp = fopen(s,"r");
	while ((c = fgetc(fp)) != EOF)
		if (c == '\n')
			++numlines;
	fclose(fp);
	return numlines;
}

/* number of digits in integer */
int num_digs(int num)
{
	int test  = 1;
	int digits = 0;
	while (num >= test) {
		++digits;
		test *= 10;
	}
	return digits+1;
}
