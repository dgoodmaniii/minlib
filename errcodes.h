/*
 * +AMDG
 */
/*
 * This document was begun on 10 May 1200, the feast of Ss.
 * Nereus, Achilleus, Domitilla V, and Pancras, MM, and it
 * is humbly dedicated to them and to the Immaculate Heart
 * of Mary for their prayers and to the Sacred Heart of
 * Jesus for His mercy.
 */

/* defines constants for the error codes */

#define ALLGOOD 0
#define BAD_OPTION 1
#define NEED_ARGUMENT_ARG 2
/* insufficient memory errors; all exceedingly unlikely */
/* insufficient memory to load all the lines from the data file */
#define INSUFF_MEMORY_LOAD 3
/* insufficient memory to load formatted lines built from
 * information in data file */
#define INSUFF_MEMORY_FORMAT 4
/* insufficient memory to track formatted lines built from
 * information in data file once it's sorted */
#define INSUFF_MEMORY_TRACK 5
/* insufficient memory to store format string */
#define INSUFF_MEMORY_FORMSTRING 6
/* insufficient memory for various internal deals */
#define INSUFF_INTERNAL_MEMORY 7
/* file errors */
#define BAD_FILE 8