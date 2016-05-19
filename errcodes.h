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
/* error with the input file (data file) */
#define BAD_INPUT_FILE 8
/* insufficient memory for filename */
#define INSUFF_MEMORY_FILENAME 9
/* bad config file */
#define BAD_CONFIG_FILE 10
/* no data file specified */
#define NO_DATA_FILE 11

/* define length constants */
#define MAX_ERR_LENGTH 67
#define MAX_REGEXP_LEN 432

/* define option identifiers */
#define COL_TOPBAR_FORE 0
#define COL_TOPBAR_BACK 1
#define COL_BOTBAR_FORE 2
#define COL_BOTBAR_BACK 3
#define COL_TEXT_FORE 4
#define COL_TEXT_BACK 5
