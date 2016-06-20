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

int stats_report(int *recnums, char **ptr, char **formlist,
					int numrecs, int statsval)
{
	int i;

	for (i = 0; i < numrecs; ++i)
		printf("%s\n",*(formlist+i));
	if (statsval == 1)
		print_stats(ptr);
	return 0;
}

int print_stats(char **ptr)
{
	return 0;
}
